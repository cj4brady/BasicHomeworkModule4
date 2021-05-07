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
    // Create a queue, enqueue source vertex and mark source
    // vertex as visited
    queue<int> q;
    q.push(source);
    visited_nodes[source] = true;
    parent[source] = -1;

    // Standard BFS Loop
    while (!q.empty()) {
        int u = q.front();
       // cout << u << endl;
        q.pop();

        for (int v = 0; v < sizet; v++) {
            if (visited_nodes[v] == false && residualGraph[u][v] > 0) {
                // If we find a connection to the sink node,
                // then there is no point in BFS anymore We
                // just have to set its parent and can return
                // true
                if (v == sink) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited_nodes[v] = true;
            }
        }
    }

    // We didn't reach sink in BFS starting from source, so
    // return false
    return false;
}


// Returns the maximum flow from s to t in the given graph
int fordFulkerson(vector<vector<int>> graph, int s, int t) {
    int u, v;

    // Create a residual graph and fill the residual graph
    // with given capacities in the original graph as
    // residual capacities in residual graph
    int sizet = graph.size();
    vector<vector<int>> residualGraph;
    residualGraph.resize(sizet);
    for (int i = 0; i < sizet; i++) {
        residualGraph[i].resize(sizet);
    }
        // indicates residual capacity of edge
        // from i to j (if there is an edge. If
        // rGraph[i][j] is 0, then there is not)
        for (u = 0; u < sizet; u++) {
            for (v = 0; v < sizet; v++) {
                residualGraph[u][v] = graph[u][v];
            }
        }
        int parent[sizet]; // This array is filled by BFS and to
        // store path

        int max_flow = 0; // There is no flow initially

        // Augment the flow while tere is path from source to
        // sink
        while (bfs(residualGraph, s, t, parent)) {
            // Find minimum residual capacity of the edges along
            // the path filled by BFS. Or we can say find the
            // maximum flow through the path found.
            int path_flow = INT_MAX;
            for (v = t; v != s; v = parent[v]) {
                u = parent[v];
                path_flow = min(path_flow, residualGraph[u][v]);
               // cout << "Path flow " << path_flow << endl;
            }

            // update residual capacities of the edges and
            // reverse edges along the path
            for (v = t; v != s; v = parent[v]) {
                u = parent[v];
                residualGraph[u][v] -= path_flow;
                residualGraph[v][u] += path_flow;
            }

            // Add path flow to overall flow
            max_flow += path_flow;
        }

        // Return the overall flow
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
            cin >> str1;

            cin >> str2;

            cin >> str3;
            str4.append( str2 + "_" + str3);



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
            cin >> str1;
            cin >> str2;
            cin >> str3;
            str4.append(str1 + "_" + str2);
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





        auto it = flow_map.begin();

        while (it!=flow_map.end()) {
            cout << it->first << " " << it->second << endl;
            ++it;
        }




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

        for (int i = 0; i < graph_dimension; i++)
        {
            for (int j = 0; j < graph_dimension; j++)
            {
                cout << flow_graph[i][j] << " ";
            }
            cout << endl;
        }



        // do I need to make a residual capacity graph now?
        // how do i go about implementing ford fulkerson


        // run bfs / dfs ?


        cout << "The maximum possible flow is "
             << fordFulkerson(flow_graph, 0, 8);

        cout << endl;

        cin >> requests;
        cin >> num_courses;
        cin >> number_enrolled;
    }





    return 0;
}











