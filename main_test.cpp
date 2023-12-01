#include "kdtree.h"
#include "utils.h"

std::string nameOfNearestNeighbor() {
    std::string filename = "audio_features.csv"; // Cambia esto por la ruta de tu archivo
    //std::cout << "Reading file: " << filename << std::endl;
    std::vector<Point> points = readCSV(filename);
    
    KDTree tree(points);

    //std::cout << "KD Tree:" << std::endl;
    //tree.printTree();

    std::string queryFilename = "Processing/query_test.txt";
    Point query = readQueryPoint(queryFilename);
    query.name = "Query";

    //Print nearest neighbor
    //printVector(query.dimensions);
    auto [nearest, distance] = tree.nearestNeighbor(query);
    return nearest.name;
}


int main() {
    std::cout << nameOfNearestNeighbor() << std::endl;
    return 0;
}