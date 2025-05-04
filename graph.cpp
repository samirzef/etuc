#include "graph.h"

Graph::Graph(int n_) : n(n_), dist(n, std::vector<double>(n, 0.0)) {}

void Graph::generateRandomMetric(unsigned seed) {
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> uni(0.0, 1.0);

    // assign random points in [0,1]^2
    std::vector<std::pair<double,double>> pts(n);
    for (int i = 0; i < n; i++)
        pts[i] = { uni(rng), uni(rng) };

    // compute Euclidean distances (satisfies triangle inequality)
    edge_list.clear();
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            double dx = pts[i].first  - pts[j].first;
            double dy = pts[i].second - pts[j].second;
            double d = std::sqrt(dx*dx + dy*dy);
            dist[i][j] = dist[j][i] = d;
            edge_list.push_back({i, j, d});
        }
    }
}
