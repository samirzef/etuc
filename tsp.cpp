// tsp.cpp
#include "tsp.h"
#include <algorithm>
#include <stack>

// --- Disjoint Set Union for Kruskal ---
struct DSU {
    std::vector<int> p, r;
    DSU(int n): p(n,-1), r(n,0) { std::iota(p.begin(), p.end(), 0); }
    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }
    bool unite(int a,int b){
        a=find(a); b=find(b);
        if(a==b) return false;
        if(r[a]<r[b]) std::swap(a,b);
        p[b]=a;
        if(r[a]==r[b]) r[a]++;
        return true;
    }
};

TSP::TSP(const Graph& G_) : G(G_) {}

void TSP::kruskal() {
    auto edges = G.edges();
    std::sort(edges.begin(), edges.end(), 
              [](const Edge &a, const Edge &b){ return a.w < b.w; });
    DSU dsu(G.size());
    for (auto &e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mst_edges.push_back(e);
            mst_w += e.w;
            if ((int)mst_edges.size() == G.size()-1) break;
        }
    }
}

void TSP::buildMultigraph() {
    int m = (int)mst_edges.size();
    multAdj.assign(G.size(), {});
    usedEdge.assign(2*m, false);
    // each MST edge appears twice
    for (int i = 0; i < m; ++i) {
        const auto &e = mst_edges[i];
        multAdj[e.u].push_back({e.v, 2*i});
        multAdj[e.v].push_back({e.u, 2*i});
        multAdj[e.u].push_back({e.v, 2*i+1});
        multAdj[e.v].push_back({e.u, 2*i+1});
    }
}

void TSP::findEulerTour() {
    std::stack<int> st;
    st.push(0);
    while (!st.empty()) {
        int u = st.top();
        // skip used edges
        while (!multAdj[u].empty() && usedEdge[multAdj[u].back().second])
            multAdj[u].pop_back();

        if (multAdj[u].empty()) {
            eulerTour.push_back(u);
            st.pop();
        } else {
            // no structured binding here
            auto edge_info = multAdj[u].back();
            int v  = edge_info.first;
            int id = edge_info.second;
            multAdj[u].pop_back();
            usedEdge[id] = true;
            st.push(v);
        }
    }
    // compute Eulerian walk length
    for (size_t i = 1; i < eulerTour.size(); ++i)
        euler_len += G.distance(eulerTour[i-1], eulerTour[i]);
}

void TSP::buildHamiltonianCycle() {
    std::vector<bool> vis(G.size(), false);
    cycle.reserve(G.size()+1);
    for (int u : eulerTour) {
        if (!vis[u]) {
            cycle.push_back(u);
            vis[u] = true;
        }
    }
    cycle.push_back(cycle.front());
}

double TSP::run() {
    kruskal();
    buildMultigraph();
    findEulerTour();
    buildHamiltonianCycle();
    // final cycle length
    double L = 0;
    for (size_t i = 1; i < cycle.size(); ++i)
        L += G.distance(cycle[i-1], cycle[i]);
    return L;
}
