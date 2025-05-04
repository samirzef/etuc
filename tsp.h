#ifndef TSP_H
#define TSP_H

#include "graph.h"
#include <vector>

// Runs Kruskal, Euler‐tour, and Hamiltonian shortcut
class TSP {
public:
    TSP(const Graph& G);
    // Execute all steps; returns length of final Hamiltonian cycle
    double run();

    // Access intermediate values if desired
    double mstWeight() const { return mst_w; }
    double eulerLength() const { return euler_len; }
    const std::vector<int>& hamiltonianCycle() const { return cycle; }

private:
    const Graph& G;
    double mst_w = 0.0;
    double euler_len = 0.0;
    std::vector<Edge> mst_edges;
    std::vector<std::vector<std::pair<int,int>>> multAdj; // (neighbor, edge_id)
    std::vector<bool> usedEdge;
    std::vector<int> eulerTour, cycle;

    void kruskal();
    void buildMultigraph();
    void findEulerTour();
    void buildHamiltonianCycle();
};

#endif // TSP_H
