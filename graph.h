// graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <random>
#include <cmath>

template<typename T>
struct Edge {
    int u, v;
    T w;
};

class Graph {
public:
    Graph(int n);
    void generateRandomMetric(unsigned seed = std::random_device{}());
    
    int size() const { return n; }
    const std::vector<Edge<double>>& edges() const { return edge_list; }
    double distance(int u, int v) const { return dist[u][v]; }
    const std::vector<std::vector<double>>& distanceMatrix() const { return dist; }

private:
    int n;
    std::vector<std::vector<double>> dist;
    std::vector<Edge<double>> edge_list;
};

#endif // GRAPH_H
