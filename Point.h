#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <queue>

struct Point {
    std::vector<double> dimensions;
    std::string name;

    double& operator[](size_t i) { return dimensions[i]; }
    const double& operator[](size_t i) const { return dimensions[i]; }
    size_t size() const { return dimensions.size(); }
};

struct KDNode {
    Point point;
    KDNode *left, *right;
};

using PointDistancePair = std::pair<double, KDNode*>;
struct Compare {
    bool operator()(const PointDistancePair& a, const PointDistancePair& b) {
        return a.first > b.first; // Compara basado en la distancia, el más cercano al principio
    }
};

#endif