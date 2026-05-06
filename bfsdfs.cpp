#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

struct Graph{
    int V;
    vector<vector<int>> adj;
    vector<bool> dfsVisited;

    Graph(int V) {
        this->V = V;
        adj.resize(V);
        dfsVisited.resize(V, false);
    }

    void createGraph(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void bfs(int start) {
        queue<int> q;
        vector<bool> visited(V, false);
        
        q.push(start);
        visited[start] = true;

        while(!q.empty()) {
            int node = q.front();
            cout << node << " ";
            q.pop();

            #pragma omp parallel for
            for(int i = 0; i < adj[node].size(); i++) {
                int neighbor = adj[node][i];

                if(!visited[neighbor]) {
                    #pragma omp critical
                    {
                        if(!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
    }

    void dfs(int start, vector<bool> &visited) {
        visited[start] = true;
        cout << start << " ";

        #pragma omp parallel for
        for(int i = 0; i < adj[start].size(); i++) {
            int neighbor = adj[start][i];

            if (!visited[neighbor]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        dfs(neighbor, visited);
                    }
                }
            }
        }
    }
};

int main() {
    // 0 based indexing
    Graph g(5);
    g.createGraph(1,2);
    g.createGraph(3,2);
    g.createGraph(4,2);
    g.createGraph(0,3);

    cout << "BFS: ";
    g.bfs(0);

    cout << "\n----------\nDFS: ";
    g.dfs(0, g.dfsVisited);

    return 0;
}

// g++ -fopenmp yourfile.cpp -o output
// ./output