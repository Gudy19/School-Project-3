//Student Name: Obadiah Goodnews Chukwu
// School Project

//Topic: Transport Connectivity in Southern Africa
//NOTE: The distances are approximate only for this demonstration.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <iomanip>
#include <algorithm>
#include <utility>
#include <limits>

using namespace std;

struct Edge {
    int to;
    int w;
};

using Graph = vector<vector<Edge>>;

static void printRule(char ch = '-', int n = 70) {
    for (int i = 0; i < n; ++i) cout << ch;
    cout << "\n";
}

static vector<vector<int>> toAdjacencyMatrix(const Graph& g) {
    int n = static_cast<int>(g.size());
    vector<vector<int>> M(n, vector<int>(n, 0));
    for (int u = 0; u < n; ++u) {
        M[u][u] = 0;
        for (size_t k = 0; k < g[u].size(); ++k) {
            const Edge& e = g[u][k];
            M[u][e.to] = e.w;
        }
    }
    return M;
}

static void printMatrix(const vector<vector<int>>& M, const vector<string>& city) {
    int n = static_cast<int>(M.size());
    cout << setw(12) << "";
    for (int j = 0; j < n; ++j) cout << setw(12) << city[j].substr(0, 10);
    cout << "\n";
    for (int i = 0; i < n; ++i) {
        cout << setw(12) << city[i].substr(0, 10);
        for (int j = 0; j < n; ++j) cout << setw(12) << M[i][j];
        cout << "\n";
    }
}

static void BFS(const Graph& g, int s, const vector<string>& city) {
    int n = static_cast<int>(g.size());
    if (s < 0 || s >= n) { cerr << "Invalid start city index.\n"; return; }
    vector<bool> vis(n, false);
    queue<int> q;
    vis[s] = true;
    q.push(s);
    cout << city[s];
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (size_t i = 0; i < g[u].size(); ++i) {
            int v = g[u][i].to;
            if (!vis[v]) {
                vis[v] = true;
                q.push(v);
                cout << " -> " << city[v];
            }
        }
    }
    cout << "\n";
}
static void dijkstra(const Graph& g, int s, int t, const vector<string>& city) {
    int n = static_cast<int>(g.size());
    const long long INF = (numeric_limits<long long>::max)() / 4;
    vector<long long> dist(n, INF);
    vector<int> parent(n, -1);
    typedef pair<long long, int> P;
    priority_queue<P, vector<P>, greater<P>> pq;
    dist[s] = 0;
    pq.push(P(0, s));
    while (!pq.empty()) {
        P cur = pq.top(); pq.pop();
        long long d = cur.first;
        int u = cur.second;
        if (d != dist[u]) continue;
        if (u == t) break;
        for (size_t i = 0; i < g[u].size(); ++i) {
            const Edge& e = g[u][i];
            int v = e.to;
            long long nd = d + e.w;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pq.push(P(nd, v));
            }
        }
    }
    if (dist[t] == INF) {
        cout << "No route from " << city[s] << " to " << city[t] << ".\n";
        return;
    }
    vector<int> path;
    for (int cur = t; cur != -1; cur = parent[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());
    cout << "Shortest path from " << city[s] << " to " << city[t] << ":\n";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << city[path[i]];
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\nTotal distance: " << dist[t] << " km\n";
}

static void buildDemo(Graph& g, vector<string>& city) {
    city = {"Johannesburg", "Gaborone", "Windhoek", "Harare", "Maputo"};
    int n = city.size();
    g.assign(n, vector<Edge>());
    struct Add {
        static void u(Graph& gg, int a, int b, int w) {
            gg[a].push_back(Edge{b,w});
            gg[b].push_back(Edge{a,w});
        }
    };
    Add::u(g, 0, 1, 290);
    Add::u(g, 0, 4, 540);
    Add::u(g, 0, 3, 1120);
    Add::u(g, 1, 2, 950);
    Add::u(g, 2, 0, 1400);
    Add::u(g, 3, 4, 1000);
}

static void printAdjList(const Graph& g, const vector<string>& city) {
    for (int u = 0; u < (int)g.size(); ++u) {
        cout << setw(12) << city[u] << " : ";
        for (size_t i = 0; i < g[u].size(); ++i) {
            const Edge& e = g[u][i];
            cout << "(" << city[e.to] << ", " << e.w << "km)";
            if (i + 1 < g[u].size()) cout << ", ";
        }
        cout << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Graph g;
    vector<string> city;
    buildDemo(g, city);
    cout << "Southern Africa Transport Graph\n";
    printRule('=');
    cout << "Cities (indices):\n";
    for (int i = 0; i < (int)city.size(); ++i)
        cout << "  " << i << " = " << city[i] << "\n";
    printRule();
    cout << "Adjacency List:\n";
    printAdjList(g, city);
    printRule();
    cout << "Adjacency Matrix (km):\n";
    auto M = toAdjacencyMatrix(g);
    printMatrix(M, city);
    printRule();
    cout << "BFS Traversal\nEnter start city index (0-" << (int)city.size()-1 << "): ";
    int s;
    if (!(cin >> s)) { cerr << "Invalid.\n"; return 0; }
    cout << "Order: ";
    BFS(g, s, city);
    printRule();
    cout << "Dijkstra's Shortest Path\nEnter source and target (e.g. 0 3): ";
    int src, dst;
    if (!(cin >> src >> dst)) { cerr << "Invalid.\n"; return 0; }
    dijkstra(g, src, dst, city);
    printRule('=');
    cout << "Done.\n";
    return 0;
}
