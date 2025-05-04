// Delta-TSP Approximation in C++
// Implements: Simulation, Kruskal's MST, Eulerian Tour, Hamiltonian Cycle Conversion

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <stack>
#include <unordered_set>
#include <set>

using namespace std;
struct Edge {
    int u, v;
    double weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

struct DSU {
    vector<int> parent;
    DSU(int n) : parent(n) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (x != parent[x]) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        int xr = find(x), yr = find(y);
        if (xr == yr) return false;
        parent[yr] = xr;
        return true;
    }
};

class Graph {
public:
    int n;
    vector<vector<double>> dist;
    vector<Edge> edges;

    Graph(int n) : n(n), dist(n, vector<double>(n, 0.0)) {}

    void simulate() {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(1.0, 100.0);
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double d = dis(gen);
                dist[i][j] = dist[j][i] = d;
                edges.push_back({i, j, d});
            }
        }
    }

    vector<Edge> kruskal() {
        vector<Edge> mst;
        DSU dsu(n);
        sort(edges.begin(), edges.end());
        for (Edge& e : edges) {
            if (dsu.unite(e.u, e.v)) {
                mst.push_back(e);
            }
        }
        return mst;
    }

    vector<vector<int>> buildMultigraph(const vector<Edge>& mst) {
        vector<vector<int>> adj(n);
        for (const Edge& e : mst) {
            adj[e.u].push_back(e.v);
            adj[e.v].push_back(e.u);
            adj[e.u].push_back(e.v);
            adj[e.v].push_back(e.u);
        }
        return adj;
    }

    vector<int> findEulerianTour(vector<vector<int>>& adj) {
        vector<int> tour;
        stack<int> st;
        vector<multiset<int>> localAdj(n);
        for (int u = 0; u < n; ++u) {
            for (int v : adj[u]) {
                localAdj[u].insert(v);
            }
        }
        st.push(0);
        while (!st.empty()) {
            int u = st.top();
            if (localAdj[u].empty()) {
                tour.push_back(u);
                st.pop();
            } else {
                int v = *localAdj[u].begin();
                localAdj[u].erase(localAdj[u].find(v));
                localAdj[v].erase(localAdj[v].find(u));
                st.push(v);
            }
        }
        return tour;
    }

    vector<int> toHamiltonianCycle(const vector<int>& eulerian) {
        vector<int> cycle;
        unordered_set<int> visited;
        for (int v : eulerian) {
            if (visited.insert(v).second) {
                cycle.push_back(v);
            }
        }
        cycle.push_back(cycle[0]); // make it a cycle
        return cycle;
    }

    double tourLength(const vector<int>& tour) {
        double length = 0.0;
        for (size_t i = 1; i < tour.size(); ++i) {
            length += dist[tour[i-1]][tour[i]];
        }
        return length;
    }
};

int main() {
    vector<int> sizes = {10, 20, 50};
    for (int n : sizes) {
        Graph g(n);
        g.simulate();
        auto mst = g.kruskal();
        auto multigraph = g.buildMultigraph(mst);
        auto eulerian = g.findEulerianTour(multigraph);
        auto hamiltonian = g.toHamiltonianCycle(eulerian);

        double opt_approx = g.tourLength(hamiltonian);

        cout << "Size: " << n << endl;
        cout << "Hamiltonian tour length (2-approximation): " << opt_approx << endl;
        cout << "---" << endl;
    }
    return 0;
}
