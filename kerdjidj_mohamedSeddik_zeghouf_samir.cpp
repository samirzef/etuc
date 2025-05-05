#include "graph.h"
#include "tsp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

int main() {
    std::vector<int> sizes = {10, 20, 50};
    std::cout << std::fixed << std::setprecision(4);
    std::ofstream out("kerdjidj_mohamedSeddik_zeghouf_samir.csv"); // Single file for everything
    if (!out.is_open()) {
        std::cerr << "Failed to open output file!\n";
        return 1;
    }

    for (int n : sizes) {
        std::cout << "\n=== Processing graph with " << n << " nodes ===\n";

        Graph G(n);
        G.generateRandomMetric(static_cast<unsigned>(1234 + n));

        TSP solver(G);
        double tourLength = solver.run();

        std::cout << "Results:\n";
        std::cout << "- Poids de l'arbre couvrant minimal    : " << solver.mstWeight() << "\n";
        std::cout << "- Longueur du parcours eulerien  : " << solver.eulerLength() << "\n";
        std::cout << "- Longueur du cycle hamiltonien (TSP)  : " << tourLength << "\n";

        std::cout << n << "\t" << solver.mstWeight() << "\t"
                  << solver.eulerLength() << "\t" << tourLength << "\n";

        out << "=== Graph with " << n << " nodes ===\n";

        // 1. Distance Matrix
        out << "Distance Matrix:\n,";
        for (int i = 1; i <= n; ++i) out << i << (i == n ? "\n" : ",");
        const auto& mat = G.distanceMatrix();
        for (int i = 0; i < n; ++i) {
            out << (i + 1) << ",";
            for (int j = 0; j < n; ++j) {
                out << std::fixed << std::setprecision(5) << mat[i][j];
                out << (j + 1 < n ? "," : "");
            }
            out << "\n";
        }
        out << "\n";

        // 2. l'arbre couvrant minimal
        out << "l'arbre couvrant minimal:\nfrom,to,weight\n";
        for (const auto& e : solver.getMSTEdges()) {
            out << (e.u + 1) << "," << (e.v + 1) << "," << std::fixed << std::setprecision(5) << e.w << "\n";
        }
        out << "total,," << std::fixed << std::setprecision(5) << solver.mstWeight() << "\n\n";

        // 3. parcours eulerien
        out << "parcours eulerien:\nfrom,to,distance\n";
        const auto& tour = solver.getEulerTour();
        double totalEuler = 0.0;
        for (size_t i = 1; i < tour.size(); ++i) {
            int u = tour[i - 1];
            int v = tour[i];
            double d = G.distance(u, v);
            totalEuler += d;
            out << (u + 1) << "," << (v + 1) << "," << std::fixed << std::setprecision(5) << d << "\n";
        }
        out << "total,," << std::fixed << std::setprecision(5) << totalEuler << "\n\n";

        // 4. cycle hamiltonien
        out << "cycle hamiltonien:\nfrom,to,distance\n";
        const auto& cycle = solver.hamiltonianCycle();
        double totalTSP = 0.0;
        for (size_t i = 1; i < cycle.size(); ++i) {
            int u = cycle[i - 1];
            int v = cycle[i];
            double d = G.distance(u, v);
            totalTSP += d;
            out << (u + 1) << "," << (v + 1) << "," << std::fixed << std::setprecision(5) << d << "\n";
        }
        out << "total,," << std::fixed << std::setprecision(5) << totalTSP << "\n\n";

        out << "\n\n"; 
    }

    out.close();
    std::cout << "\nAll tasks completed. Results saved in 'full_results.csv'.\n";
    return 0;
}
