#include "graph.h"
#include "tsp.h"
#include <iostream>
#include <vector>
#include <iomanip>

int main() {
    std::vector<int> ns = {10, 20, 50};
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "n\tMST\tEuler\tTSP Tour\n";
    for (int n : ns) {
        Graph G(n);
        G.generateRandomMetric(1234 + n);
        TSP solver(G);
        double tour = solver.run();
        std::cout
            << n << '\t'
            << solver.mstWeight()    << '\t'
            << solver.eulerLength()  << '\t'
            << tour                  << '\n';
    }
    return 0;
}
