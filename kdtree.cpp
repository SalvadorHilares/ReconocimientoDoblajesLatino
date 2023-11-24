#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <string>

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

    Point nearestNeighbor(const Point& target) {
        KDNode* best = nullptr;
        double bestDist = std::numeric_limits<double>::max();
        nearestNeighbor(root, target, best, bestDist, 0);
        return best ? best->point : Point{{}, "No Match"};
    }

    void printTree() {
        printTree(root, 0);
    }
};

int main() {
    std::vector<Point> points = {
        {{2.1, 3.1}, "Punto A"}, 
        {{5.4, 2.3}, "Punto B"}, 
        {{9.7, 2.1}, "Punto C"}, 
        {{4.3, 3.3}, "Punto D"}, 
        {{2.9, 4.7}, "Punto E"}, 
        {{7.1, 7.2}, "Punto F"}
    };
    KDTree tree(points);

    std::cout << "KD Tree:" << std::endl;
    tree.printTree();

    Point query = {{5, 5}, "Consulta"};
    Point nearest = tree.nearestNeighbor(query);
    std::cout << "Nearest neighbor to " << query.name << ": " << nearest.name << " (";
    for (const auto& dim : nearest.dimensions) {
        std::cout << dim << " ";
    }
    std::cout << ")" << std::endl;

    return 0;
}
