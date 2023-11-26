#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <string>
#include <random>
#include <fstream>
#include <sstream>

struct Point {
    std::vector<double> dimensions;
    std::string name;

    double& operator[](size_t i) { return dimensions[i]; }
    const double& operator[](size_t i) const { return dimensions[i]; }
    size_t size() const { return dimensions.size(); }
};

void printVector(const std::vector<double>& v) {
    std::cout << "(";
    for (const auto& dim : v) {
        std::cout << dim << " ";
    }
    std::cout << ")" << std::endl;
}

Point generateRandomPoint(int D, const std::string& name) {
    std::default_random_engine generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 10.0);
    Point p;
    p.dimensions.resize(D);
    for (int d = 0; d < D; ++d) {
        p.dimensions[d] = distribution(generator);
    }
    p.name = name;
    return p;
}

std::vector<Point> generateRandomPoints(int N, int D) {
    std::vector<Point> points;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, 10.0);

    for (int i = 0; i < N; ++i) {
        Point p;
        p.dimensions.resize(D);
        for (int d = 0; d < D; ++d) {
            p.dimensions[d] = distribution(generator);
        }
        p.name = "Punto " + std::to_string(i + 1);
        points.push_back(p);
    }

    return points;
}

std::vector<Point> readCSV(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Point point;

        // Leer las dimensiones
        while (std::getline(ss, token, ',')) {
            if (ss.peek() == ' ') ss.ignore();
            bool last = false;
            if (token[token.size() - 1] == '"'){
                last = true;
            }
            if (token[0] == '"' || token[1] == '[' || token[token.size() - 2] == ']' || token[token.size() - 1] == '"') {
                // Eliminar corchetes si existen
                token.erase(std::remove(token.begin(), token.end(), '['), token.end());
                token.erase(std::remove(token.begin(), token.end(), '"'), token.end());
                token.erase(std::remove(token.begin(), token.end(), ']'), token.end());
            }

            if (std::stringstream(token) >> token) {
                // Convertir a double y agregar a las dimensiones
                point.dimensions.push_back(std::stod(token));
                if (last) break;
            }
            //printVector(point.dimensions);
        }

        // Leer el nombre
        if (std::getline(ss, token, ',')) {
            point.name = token;
        }

        points.push_back(point);
    }

    return points;
}
struct KDNode {
    Point point;
    KDNode *left, *right;
};

class KDTree {
private:
    KDNode* root;

    KDNode* buildTree(std::vector<Point>& points, int depth, int start, int end) {
        if (start > end) return nullptr;

        int axis = depth % points[0].size();
        int mid = start + (end - start) / 2;
        std::nth_element(points.begin() + start, points.begin() + mid, points.begin() + end + 1, 
                        [axis](const Point& a, const Point& b) {
                            return a[axis] < b[axis];
                        });

        KDNode* node = new KDNode{points[mid], nullptr, nullptr};
        node->left = buildTree(points, depth + 1, start, mid - 1);
        node->right = buildTree(points, depth + 1, mid + 1, end);
        return node;
    }

    double distance(const Point& a, const Point& b) {
        double dist = 0;
        for (size_t i = 0; i < a.size(); ++i) {
            dist += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return std::sqrt(dist);
    }

    void nearestNeighbor(KDNode* node, const Point& target, KDNode*& best, double& bestDist, int depth) {
        if (node == nullptr) return;

        double d = distance(target, node->point);
        if (d < bestDist) {
            bestDist = d;
            best = node;
        }

        int axis = depth % target.size();
        double delta = target[axis] - node->point[axis];
        double delta2 = delta * delta;

        KDNode *first = (delta < 0) ? node->left : node->right;
        KDNode *second = (delta < 0) ? node->right : node->left;

        nearestNeighbor(first, target, best, bestDist, depth + 1);
        if (delta2 < bestDist) {
            nearestNeighbor(second, target, best, bestDist, depth + 1);
        }
    }

    void printTree(KDNode* node, int depth) {
        if (node == nullptr) return;

        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << node->point.name << ": (";
        for (const auto& dim : node->point.dimensions) {
            std::cout << dim << " ";
        }
        std::cout << ")" << std::endl;

        printTree(node->left, depth + 1);
        printTree(node->right, depth + 1);
    }

public:
    KDTree(std::vector<Point>& points) {
        root = buildTree(points, 0, 0, points.size() - 1);
    }

    std::pair<Point, double> nearestNeighbor(const Point& target) {
        KDNode* best = nullptr;
        double bestDist = std::numeric_limits<double>::max();
        nearestNeighbor(root, target, best, bestDist, 0);
        return {best ? best->point : Point{{}, "No Match"}, bestDist}; 
    }

    void printTree() {
        printTree(root, 0);
    }
};

Point readQueryPoint(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    Point queryPoint;

    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string number;
        while (std::getline(iss, number, ',')) {
            queryPoint.dimensions.push_back(std::stod(number));
        }
    }
    return queryPoint;
}

int main() {
    int N = 10; // Número de puntos
    int D = 20;  // Dimensiones
    
    //std::vector<Point> points = generateRandomPoints(N, D);
   std::string filename = "audio_features.csv"; // Cambia esto por la ruta de tu archivo
    std::vector<Point> points = readCSV(filename);

    KDTree tree(points);

    //std::cout << "KD Tree:" << std::endl;
    //tree.printTree();

    std::string queryFilename = "query.txt";
    Point query = readQueryPoint(queryFilename);
    query.name = "Query";
    // //Print nearest neighbor result only
    // auto [nearest, distance] = tree.nearestNeighbor(query);
    // std::cout << "Nearest neighbor to " << query.name << " is " << nearest.name;
    // std::cout << " at distance " << distance << std::endl;

    //Print nearest neighbor
    printVector(query.dimensions);
    auto [nearest, distance] = tree.nearestNeighbor(query);
    std::cout << nearest.name << std::endl;
    std::cout << "Nearest neighbor is " << nearest.name << " (";
    for (const auto& dim : nearest.dimensions) {
        std::cout << dim << " ";
    }
    std::cout << ") at distance " << distance << std::endl;

    return 0;
}
