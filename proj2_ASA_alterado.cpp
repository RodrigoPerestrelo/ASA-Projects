#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Graph {
    int maxVertex;
    vector<vector<int> > adj;
    vector<vector<int> > reverseAdj;

public:
    Graph(int maxVertex) : maxVertex(maxVertex), adj(maxVertex + 1), reverseAdj(maxVertex + 1) {}

    void buildGraph(int numEdges) {
        for (int i = 1; i <= numEdges; i++) {
            int u, v;
            scanf("%d %d", &u, &v);
            if (u > 0 && u <= maxVertex && v > 0 && v <= maxVertex) {
                adj[u].push_back(v);
                reverseAdj[v].push_back(u);
            } else i--;
        }
    }

    void iterativeDFS(int firstVertex, vector<int>& resultVector) {
        vector<bool> visited(maxVertex + 1, false);
        stack<pair<int, int>> stack;

        for (int i = 1; i <= maxVertex; i++) {
            if (!visited[i]) {
                stack.push({i, 0});

                while (!stack.empty()) {
                    int current = stack.top().first;
                    int &endTime = stack.top().second;
                    if (endTime == 0) {
                        visited[current] = true;
                    }
                    bool explored = true;
                    for (int neighbour : adj[current]) {
                        if (!visited[neighbour]) {
                            stack.push({neighbour, 0});
                            explored = false;
                            break;
                        }
                    }
                    if (explored) {
                        resultVector.push_back(current);
                        stack.pop();
                    } else ++endTime;
                }
            }
        }
    }

    int longestPath(vector<int> stackVector) {
        
        int finalResult = 0;
        bool changed;
        int maxAux;
        vector<bool> visited(maxVertex + 1, false);
        vector<int> jumpsVector(maxVertex + 1, -1);

        for (int i = maxVertex - 1; i >= 0; i--) {
            int vertex = stackVector[i];

            if (!visited[vertex]) {
                stack<int> SCCStack;
                vector<int> auxSCC;
                maxAux = jumpsVector[vertex];
                SCCStack.push(vertex);
                changed = false;
                visited[vertex] = true;

                while (!SCCStack.empty()) {
                    int SCCComponent = SCCStack.top();
                    auxSCC.push_back(SCCComponent);
                    SCCStack.pop();
                    for (int neighbour : reverseAdj[SCCComponent]) {
                        if (jumpsVector[neighbour] > maxAux) {
                            maxAux = jumpsVector[neighbour];
                            //printf("*--vértice: %d, vizinho: %d, valor do vizinho: %d--*\n", SCCComponent, neighbour, maxAux);
                            changed = true;
                        }
                        if (!visited[neighbour]) {
                            visited[neighbour] = true;
                            SCCStack.push(neighbour);
                        }
                    }
                }
                if (changed) {
                    for (int auxiliar : auxSCC) {
                        //printf("vértice a colocar o valor: %d, valor colocado: %d\n", auxiliar, maxAux + 1);
                        jumpsVector[auxiliar] = maxAux + 1;
                    }
                }
                else if (!changed) {
                  for (int auxiliar : auxSCC) jumpsVector[auxiliar] = 0;
                    //printf("-vértice a colocar a zero: %d-\n", vertex);
                }
                if (maxAux + 1 > finalResult) finalResult = maxAux + 1;
            }
        }
        return finalResult;
    }

};

int main() {
    int numEdges, maxVertex;

    scanf("%d %d", &maxVertex, &numEdges);

    Graph graph(maxVertex);
    graph.buildGraph(numEdges);

    vector<int> stackVector;
    graph.iterativeDFS(1, stackVector);

    printf("%d\n", graph.longestPath(stackVector));


    return 0;
}