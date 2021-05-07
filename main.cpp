#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <queue>


using namespace std;

unordered_map<string,vector<string>> desired_classes;

// the courses and how many people can be enrolled in a course
unordered_map<string,int> course_list;

// how many classes a person has been assigned
map<string,int> classes_per_person; // A
vector<string> classes_per_person_sorted;

int total_slots;
string answer = "No";

vector<string> graph_order;
//#define V graph_order.size()

unordered_map<string,int> flow_map;
unordered_map<string,string> node_type;

int graph_dimension;



// Amount of nodes in the graph
int dim;
// do i need to pass the residual graph?
// how do I go about making the residual graph
// I have a vector of nodes for indexing
// I have a matrix graph representation of possible flow
// how do I

// keep track of the path
bool bfs(vector<vector<int>> residualGraph, int source, int sink,int parent[]){
    // Visited array with nodes not visited yet
    int sizet = residualGraph.size();
    bool visited_nodes[dim];
    for (int i = 0;i<sizeof(visited_nodes);i++) {
        visited_nodes[i] = false;
    }

    queue<int> node_queue;
    node_queue.push(source);
    visited_nodes[source] = true;
    parent[source] = -1;

    // BFS
    while (!node_queue.empty()) {
        int u = node_queue.front();

        node_queue.pop();

        for (int iteration = 0; iteration < sizet; iteration++) {
            if (visited_nodes[iteration] == false && residualGraph[u][iteration] > 0) {
                if (iteration == sink) {
                    parent[iteration] = u;
                    return true;
                }
                node_queue.push(iteration);
                parent[iteration] = u;
                visited_nodes[iteration] = true;
            }
        }
    }

    return false;
}


int fordFulkerson(vector<vector<int>> graph, int source, int sink) {
    int u, v;

    int sizet = graph.size();
    vector<vector<int>> residualGraph;
    residualGraph.resize(sizet);
    for (int i = 0; i < sizet; i++) {
        residualGraph[i].resize(sizet);
    }

        for (u = 0; u < sizet; u++) {
            for (v = 0; v < sizet; v++) {
                residualGraph[u][v] = graph[u][v];
            }
        }
        int parent[sizet];
        int max_flow = 0;


        while (bfs(residualGraph, source, sink, parent)) {
            int path_flow = 99999999;
            for (v = sink; v != source; v = parent[v]) {
                u = parent[v];
                path_flow = min(path_flow, residualGraph[u][v]);

            }

            for (v = sink; v != source; v = parent[v]) {
                u = parent[v];
                residualGraph[u][v] -= path_flow;
                residualGraph[v][u] += path_flow;
            }

            max_flow += path_flow;
        }


        return max_flow;
}


// should I implement BFS, how do I approach BFS with

int main() {

    bool begin = true;
    int requests =1;
    int num_courses =1;
    int number_enrolled = 1;


    while (requests!=0 && num_courses!=0 && number_enrolled !=0) {



        graph_order.emplace_back("source");
        node_type.emplace("source","source");
        node_type.emplace("sink","sink");

        if (begin) {
            cin >> requests;
            cin >> num_courses;
            cin >> number_enrolled;
        }
        begin = false;

        int temp1 = requests;
        while (temp1 != 0) {
            string str1;
            string str2;
            string str3;
            string str4;

            // student name
            cin >> str1;

            // class name
            cin >> str4;

          //  cin >> str3;
         //   str4.append( str2 + str3);



            auto it = desired_classes.find(str1);
            if (it == desired_classes.end()) {
                vector<string> tmp = {str4};
                typedef pair<string, vector<string>> p1;
                classes_per_person[str1] = 1;
                desired_classes.insert(p1(str1,tmp));
                graph_dimension+=1;
                graph_order.push_back(str1);
                flow_map.emplace(str1,number_enrolled);
                node_type.emplace(str1,"Student");
            }
            else if (it != desired_classes.end()){
                desired_classes[str1].push_back(str4);
                classes_per_person[str1] += 1;
              //  flow_map[str1] += 1;


            }
            temp1--;
        }

        int temp2 = num_courses;
        while (temp2 != 0) {
            string str1;
            string str2;
            int str3;
            string str4;
            cin >> str4;
         //   cin >> str2;
            cin >> str3;
          //  str4.append(str1 + str2);
            course_list.emplace(str4,str3);
            total_slots += str3;
            graph_order.push_back(str4);
            flow_map.emplace(str4,str3);
            node_type.emplace(str4,"Class");
            temp2--;


        }

        // push back the sink for final node
        graph_order.emplace_back("sink");

        // add 2 because, always source and sink
        // flow_map size is all the possible nodes minus source and sink
        graph_dimension = flow_map.size() + 2;




/*
        auto it = flow_map.begin();


        while (it!=flow_map.end()) {
            cout << it->first << " " << it->second << endl;
            ++it;
        }
*/



        // Now to populate the graph with the right data, 0 if there is no edge, and the flow rate if there is one

        //populate graph with 0's
        /*
        int flow_graph[graph_dimension][graph_dimension];

        for (int i = 0; i < graph_dimension; i++)
        {
            for (int j = 0; j < graph_dimension; j++)
            {
                flow_graph[i][j] = 0;
            }
        }
         */
        vector<vector<int>> flow_graph; //Represents the residuals
        flow_graph.resize(graph_dimension);
        for (int i = 0; i < graph_dimension; i++){
            flow_graph[i].resize(graph_dimension);
        }
        for (int i = 0; i < graph_dimension; i++)
        {
            for (int j = 0; j < graph_dimension; j++)
            {
                flow_graph[i][j] = 0;
            }
        }




        int xcount = 0;
        int ycount = 0;
        for (string node1: graph_order) {
            for (string s: graph_order) {
                string type1 = node_type[node1];
                string type2 = node_type[s];

                if (type1 == "source" && type2 == "Student") {
                    flow_graph[0][xcount] = flow_map[s];
                }

                else if (type1 == "Student" && type2 == "Class") {
                    vector<string>::iterator it;
                    it = find(desired_classes[node1].begin(), desired_classes[node1].end(), s);
                    if (it != desired_classes[node1].end()) {
                        flow_graph[ycount][xcount] = 1;
                    }
                    //flow_graph[ycount][xcount] =
                }

                else if (type1 == "Class" && type2 == "sink") {
                    flow_graph[ycount][graph_dimension-1] = flow_map[node1];
                }


                xcount++;
            }
            ycount++;
            xcount = 0;

        }

        /*
        for (int i = 0; i < graph_dimension; i++)
        {
            for (int j = 0; j < graph_dimension; j++)
            {
                cout << flow_graph[i][j] << " ";
            }
            cout << endl;
        }
*/


        // do I need to make a residual capacity graph now?
        // how do i go about implementing ford fulkerson


        // run bfs / dfs ?

/*
        cout << "The maximum possible flow is "
             << fordFulkerson(flow_graph, 0, 8);

        cout << endl;
*/

        if (fordFulkerson(flow_graph,0,graph_dimension-1) == desired_classes.size() * number_enrolled) {
            cout << "Yes" << endl;
        }
        else {
            cout << "No" << endl;
        }

        cin >> requests;
        cin >> num_courses;
        cin >> number_enrolled;
    }





    return 0;
}











