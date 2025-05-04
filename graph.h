#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <random>
#include <cmath>

// Represents a weighted undirected edge
struct Edge {
    int u, v;
    double w;
};

class Graph {
public:
    Graph(int n);
    // Generate a random Δ‐metric on n points in the plane [0,1]²
    void generateRandomMetric(unsigned seed = std::random_device{}());
    int size() const { return n; }
    const std::vector<Edge>& edges() const { return edge_list; }
    double distance(int u, int v) const { return dist[u][v]; }

private:
    int n;
    std::vector<std::vector<double>> dist;
    std::vector<Edge> edge_list;
};

#endif // GRAPH_H
