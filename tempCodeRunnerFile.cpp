//Print nearest neighbor
    auto [nearest, distance] = tree.nearestNeighbor(query);
    std::cout << "Nearest neighbor to " << query.name << ": " << nearest.name << " (";
    for (const auto& dim : nearest.dimensions) {
        std::cout << dim << " ";
    }
    std::cout << ") at distance " << distance << std::endl;