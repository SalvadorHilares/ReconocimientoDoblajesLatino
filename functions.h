#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Point.h"

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

std::string findMostFrequentName(const std::vector<PointDistancePair>& neighbors) {
    std::unordered_map<std::string, int> nameFrequency;
    for (const auto& neighbor : neighbors) {
        nameFrequency[neighbor.second->point.name]++;
    }

    std::string mostFrequentName;
    int maxFrequency = 0;
    for (const auto& entry : nameFrequency) {
        if (entry.second > maxFrequency) {
            maxFrequency = entry.second;
            mostFrequentName = entry.first;
        }
    }

    return mostFrequentName;
}

#endif
// End of file