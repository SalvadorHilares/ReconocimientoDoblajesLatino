#ifndef KDTREE_H
#define KDTREE_H

#include "functions.h"
#include "Point.h"
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

#endif