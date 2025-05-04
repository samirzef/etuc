// main.cpp
#include "graph.h"
#include "tsp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

int main() {
    std::vector<int> sizes = {10, 20, 50};
    std::cout << std::fixed << std::setprecision(4);

    for (int n : sizes) {
        std::cout << "\n=== Processing graph with " << n << " nodes ===\n";

        Graph G(n);
        G.generateRandomMetric(static_cast<unsigned>(1234 + n));

        std::string matFile = "distance_matrix_" + std::to_string(n) + ".csv";
        std::cout << "Exporting distance matrix to '" << matFile << "'...\n";
        const auto& mat = G.distanceMatrix();
        std::ofstream outMat(matFile);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                outMat << mat[i][j] << (j + 1 < n ? "," : "");
            }
            outMat << '\n';
        }
        outMat.close();
        std::cout << "Distance matrix saved.\n";

        TSP solver(G);
        double tourLength = solver.run();

        std::cout << "Results:\n";
        std::cout << "- MST weight    : " << solver.mstWeight() << "\n";
        std::cout << "- Euler length  : " << solver.eulerLength() << "\n";
        std::cout << "- TSP tour len  : " << tourLength << "\n";

        std::string resFile = "tsp_result_" + std::to_string(n) + ".csv";
        std::cout << "Exporting TSP cycle to '" << resFile << "'...\n";
        auto cycle = solver.hamiltonianCycle();
        std::ofstream outRes(resFile);
        outRes << "from,to,distance\n";
        double total = 0.0;
        for (size_t k = 1; k < cycle.size(); ++k) {
            int u = cycle[k - 1];
            int v = cycle[k];
            double d = G.distance(u, v);
            total += d;
            outRes << u << ',' << v << ',' << d << '\n';
        }
        outRes << "total,," << total << '\n';
        outRes.close();
        std::cout << "TSP cycle saved.\n";
    }

    std::cout << "\nAll tasks completed. CSV files are in the working directory.\n";
    return 0;
}
