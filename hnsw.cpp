#include <iostream>
#include <vector>
#include "hnswlib/hnswlib/hnswlib.h"

//print vector
template<typename T>
void print_vector(std::vector<T> vec) {
    std::cout << "[";
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i != vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

struct Punto {
    std::string name;
    std::vector<float> dimensions;

    Punto(std::string name, std::vector<float> dimensions)
        : name(name), dimensions(dimensions) {}
};

int main() {
    // Configuración inicial
    int dim = 2;  // Número de dimensiones
    int num_elements = 3;  // Número total de elementos
    hnswlib::SpaceInterface<float>* space = new hnswlib::L2Space(dim);

    // Crear un objeto HNSW
    hnswlib::HierarchicalNSW<float>* hnsw = new hnswlib::HierarchicalNSW<float>(space, num_elements);

    // Agregar puntos
    for (int i = 0; i < num_elements; ++i) {
        std::vector<float> vec(dim);
        for (int j = 0; j < dim; ++j) {
            vec[j] = static_cast<float>(rand()) / RAND_MAX;
        }
        print_vector(vec);
        hnsw->addPoint(vec.data(), i);
    }

    // Realizar una consulta kNN
    int k = 1;  // Número de vecinos más cercanos
    std::vector<float> query_point(dim);
    for (int j = 0; j < dim; ++j) {
        query_point[j] = static_cast<float>(rand()) / RAND_MAX + 1;
    }
    
    std::priority_queue<std::pair<float, hnswlib::labeltype >> result = hnsw->searchKnn(query_point.data(), k);
    
    std::cout << "Los " << k << " vecinos más cercanos al punto P(" << query_point[0] << ", " << query_point[1] << "):" << std::endl;
    while (!result.empty()) {
        std::cout << "ID: " << result.top().second << " - Distancia: " << result.top().first << std::endl;
        result.pop();
    }

    // Liberar recursos
    delete hnsw;
    delete space;

    return 0;
}
