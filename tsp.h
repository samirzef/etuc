// tsp.h
#ifndef TSP_H
#define TSP_H

#include "graph.h"
#include <vector>

class TSP {
public:
    explicit TSP(const Graph& G);
    double run();

    double mstWeight() const { return mst_w; }
    double eulerLength() const { return euler_len; }
    const std::vector<int>& hamiltonianCycle() const { return cycle; }
    const std::vector<Edge<double>>& getMSTEdges() const { return mst_edges; }
    const std::vector<int>& getEulerTour() const { return eulerTour; }



private:
    const Graph& G;
    double mst_w = 0.0;
    double euler_len = 0.0;
    std::vector<Edge<double>> mst_edges;
    std::vector<std::vector<std::pair<int, int>>> multAdj;
    std::vector<bool> usedEdge;
    std::vector<int> eulerTour, cycle;

    void kruskal();
    void buildMultigraph();
    void findEulerTour();
    void buildHamiltonianCycle();
};

#endif // TSP_H
