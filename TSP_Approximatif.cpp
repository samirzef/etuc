#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stack>
#include <iomanip>
using namespace std;
#include <fstream> // pour gérer les fichiers


void exportMatrixToExcel(const vector<vector<double>> &matrix, const string &filename);
void exportResultsToExcel(double mstLen, double eulerLen, double hamiltonLen, double approximationFactor, const string &filename);


struct Point {
    double x, y;
};

struct Edge {
    int u, v;
    double weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// ---------- Génération du graphe complet ----------
vector<Point> generatePoints(int n) {
    vector<Point> points(n);
    for (auto &p : points) {
        p.x = rand() % 1000;
        p.y = rand() % 1000;
    }
    return points;
}

double euclidean(const Point &a, const Point &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

vector<vector<double>> generateGraph(const vector<Point> &points) {
    int n = points.size();
    vector<vector<double>> graph(n, vector<double>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            graph[i][j] = euclidean(points[i], points[j]);
    return graph;
}

// ---------- Kruskal ----------
vector<int> parent, rankSet;

int find(int u) {
    if (u != parent[u]) parent[u] = find(parent[u]);
    return parent[u];
}

bool unite(int u, int v) {
    u = find(u);
    v = find(v);
    if (u == v) return false;
    if (rankSet[u] < rankSet[v]) swap(u, v);
    parent[v] = u;
    if (rankSet[u] == rankSet[v]) rankSet[u]++;
    return true;
}

vector<Edge> kruskal(vector<vector<double>> &graph) {
    int n = graph.size();
    vector<Edge> edges;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            edges.push_back({i, j, graph[i][j]});

    sort(edges.begin(), edges.end());
    parent.resize(n);
    rankSet.resize(n, 0);
    for (int i = 0; i < n; ++i) parent[i] = i;

    vector<Edge> mst;
    for (auto &e : edges)
        if (unite(e.u, e.v)) mst.push_back(e);
    return mst;
}

// ---------- Graphe eulérien ----------
vector<vector<int>> buildEulerianGraph(const vector<Edge> &mst) {
    vector<vector<int>> adj;
    int n = 0;
    for (auto &e : mst) n = max(n, max(e.u, e.v));
    adj.resize(n + 1);
    for (auto &e : mst) {
        adj[e.u].push_back(e.v);
        adj[e.v].push_back(e.u);
        adj[e.u].push_back(e.v);
        adj[e.v].push_back(e.u);
    }
    return adj;
}

// ---------- Parcours eulérien ----------
void dfs(int u, vector<vector<int>> &adj, vector<int> &euler) {
    while (!adj[u].empty()) {
        int v = adj[u].back();
        adj[u].pop_back();
        auto it = find(adj[v].begin(), adj[v].end(), u);
        if (it != adj[v].end()) adj[v].erase(it);
        dfs(v, adj, euler);
    }
    euler.push_back(u);
}

// ---------- Hamiltonien ----------
vector<int> eulerToHamilton(const vector<int> &euler) {
    vector<int> hamilton;
    vector<bool> visited(euler.size(), false);
    for (int v : euler) {
        if (!visited[v]) {
            visited[v] = true;
            hamilton.push_back(v);
        }
    }
    return hamilton;
}

// ---------- Calcul de longueur ----------
double calculateLength(const vector<int> &path, const vector<vector<double>> &graph) {
    double len = 0;
    for (int i = 0; i < path.size() - 1; ++i)
        len += graph[path[i]][path[i + 1]];
    len += graph[path.back()][path[0]]; // retour au point de départ
    return len;
}


 

// ---------- MAIN ----------
int main() {
    srand(time(0));
    int n;
    cout << "Entrez le nombre de points : ";
    cin >> n;

    auto points = generatePoints(n);
    auto graph = generateGraph(points);
    auto mst = kruskal(graph);
    auto adj = buildEulerianGraph(mst);

    vector<int> euler;
    dfs(0, adj, euler);
    reverse(euler.begin(), euler.end());

    auto hamilton = eulerToHamilton(euler);

    double mstLen = 0;
    for (auto &e : mst) mstLen += e.weight;
    double eulerLen = calculateLength(euler, graph);
    double hamiltonLen = calculateLength(hamilton, graph);

    cout << fixed << setprecision(2);
    cout << "\nRésultats pour n = " << n << endl;
    cout << "Longueur de l'arbre couvrant minimum : " << mstLen << endl;
    cout << "Longueur du parcours eulérien        : " << eulerLen << endl;
    cout << "Longueur du cycle hamiltonien        : " << hamiltonLen << endl;
    cout << "Facteur d'approximation               : " << (hamiltonLen / mstLen) << endl;

    // Exporter la matrice des distances
    exportMatrixToExcel(graph, "distance_matrix.csv");

    // Exporter les résultats
    exportResultsToExcel(mstLen, eulerLen, hamiltonLen, hamiltonLen / mstLen, "results.csv");

    return 0;
}



// Fonction pour exporter la matrice des distances dans un fichier CSV
void exportMatrixToExcel(const vector<vector<double>> &matrix, const string &filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto &row : matrix) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i < row.size() - 1) file << ",";
            }
            file << "\n";
        }
        file.close();
    } else {
        cerr << "Impossible d'ouvrir le fichier : " << filename << endl;
    }
}

// Fonction pour exporter les résultats dans un fichier CSV
void exportResultsToExcel(double mstLen, double eulerLen, double hamiltonLen, double approximationFactor, const string &filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "Métrique=Valeur\n";
        file << "Longueur de l'arbre couvrant minimum=" << mstLen << "\n";
        file << "Longueur du parcours eulérien=" << eulerLen << "\n";
        file << "Longueur du cycle hamiltonien=" << hamiltonLen << "\n";
        file << "Facteur d'approximation=" << approximationFactor << "\n";
        file.close();
    } else {
        cerr << "Impossible d'ouvrir le fichier : " << filename << endl;
    }
}