#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <mutex>

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
        omp_lock_t lock;
        omp_init_lock(&lock);
        
        q.push(start);
        visited[start] = true;

        while(!q.empty()) {
            omp_set_lock(&lock);
            int node = q.front();
            q.pop();
            omp_unset_lock(&lock);
            
            cout << node << " ";

            #pragma omp parallel for
            for(int i = 0; i < adj[node].size(); i++) {
                int neighbor = adj[node][i];

                if(!visited[neighbor]) {
                    omp_set_lock(&lock);
                    if(!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                    omp_unset_lock(&lock);
                }
            }
        }
        
        omp_destroy_lock(&lock);
    }

    void dfs(int start, vector<bool> &visited) {
        omp_set_lock(&dfsLock);
        visited[start] = true;
        cout << start << " ";
        omp_unset_lock(&dfsLock);

        #pragma omp parallel for
        for(int i = 0; i < adj[start].size(); i++) {
            int neighbor = adj[start][i];

            if (!visited[neighbor]) {
                dfs(neighbor, visited);
            }
        }
    }

    omp_lock_t dfsLock;
};

int main() {
    Graph g(5);
    omp_init_lock(&g.dfsLock);
    
    g.createGraph(0, 1);
    g.createGraph(0, 2);
    g.createGraph(1, 2);
    g.createGraph(1, 3);
    g.createGraph(2, 3);
    g.createGraph(3, 4);

    cout << "BFS: ";
    g.bfs(0);
    cout << "\n";

    cout << "DFS: ";
    g.dfs(0, g.dfsVisited);
    cout << "\n";
    
    omp_destroy_lock(&g.dfsLock);

    return 0;
}