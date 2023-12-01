#include "kdtree.h"

std::string nameOfNearestNeighbor(std::pair<Point, double> nearestNeighbor) {
    return nearestNeighbor.first.name;
}

int main() {
    int k = 5; // número de vecinos más cercanos a buscar

    //std::vector<Point> points = generateRandomPoints(N, D);
    std::string filename = "audio_features.csv"; // Cambia esto por la ruta de tu archivo
    std::vector<Point> points = readCSV(filename);

    KDTree tree(points);

    //std::cout << "KD Tree:" << std::endl;
    //tree.printTree();

    std::string queryFilename = "Processing/query.txt";
    Point query = readQueryPoint(queryFilename);
    query.name = "Query";

    //Print nearest neighbor
    printVector(query.dimensions);
    auto [nearest, distance] = tree.nearestNeighbor(query);
    std::cout << nearest.name << std::endl;
    std::cout << "Nearest neighbor is " << nearest.name << " (";
    for (const auto& dim : nearest.dimensions) {
        std::cout << dim << " ";
    }
    std::cout << ") at distance " << distance << std::endl;

    //Print k nearest neighbors
    std::vector<PointDistancePair> neighbors = tree.kNearestNeighbors(query, k);
    for (const auto& neighbor : neighbors) {
        std::cout << "Vecino: " << neighbor.second->point.name << ", Distancia: " << neighbor.first << std::endl;
    }

    //Print most frequent name
    std::cout << "El nombre más frecuente es: " << findMostFrequentName(neighbors) << std::endl;
    return 0;
}