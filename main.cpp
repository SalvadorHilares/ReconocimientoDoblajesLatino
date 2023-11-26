#include "kdtree.h"

int main() {
    int N = 10; // Número de puntos
    int D = 20;  // Dimensiones
    
    //std::vector<Point> points = generateRandomPoints(N, D);
    std::string filename = "audio_features.csv"; // Cambia esto por la ruta de tu archivo
    std::vector<Point> points = readCSV(filename);

    KDTree tree(points);

    //std::cout << "KD Tree:" << std::endl;
    //tree.printTree();

    std::string queryFilename = "Processing/query.txt";
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