#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

int_fast64_t INF = 100000000;

class Tree {
public:
    void add(int_fast64_t node_id, int_fast64_t weight);

    void pre_calc();

    int_fast64_t get_lca(int_fast64_t x, int_fast64_t y);

    int_fast64_t get_min_weight(int_fast64_t x, int_fast64_t y);

private:
    std::vector<int_fast64_t> parent = {0};
    std::vector<int_fast64_t> depth = {0};
    std::vector<std::vector<int_fast64_t>> jmp;
    std::vector<int_fast64_t> weights = {0};
};

void Tree::add(int_fast64_t node_id, int_fast64_t weight) {
    depth.push_back(depth[node_id - 1] + 1); // dfs
    parent.push_back(node_id - 1); // добавили новую врешину
    weights.push_back(weight);
}


void Tree::pre_calc() {
    for (int_fast64_t k = 0; k < parent.size(); k++) {
        jmp.emplace_back();
        jmp[k].push_back(parent[k]);
    }

    for (int_fast64_t k = 1; k < std::ceil(log2(parent.size())) + 1; k++) {
        for (int_fast64_t v = 0; v < parent.size(); v++) {
            jmp[v].push_back(jmp[jmp[v][k - 1]][k - 1]);
        }
    }
}

int_fast64_t Tree::get_lca(int_fast64_t x, int_fast64_t y) {
    if (depth[x] < depth[y]) {
        std::swap(x, y);
    }
    int_fast64_t delta = depth[x] - depth[y];

    for (int_fast64_t k = std::ceil(log2(parent.size())); k >= 0; k--) { // поднялись на одинаковую глубину
        if (delta >= 1 << k) {
            x = jmp[x][k];
            delta -= 1 << k;
        }
    }

    if (x == y) {
        return x;
    }
    int_fast64_t _x, _y;
    for (int_fast64_t k = std::ceil(log2(parent.size())); k >= 0; k--) {
        _x = jmp[x][k];
        _y = jmp[y][k];
        if (_x != _y) {
            x = _x;
            y = _y;
        }
    }
    return jmp[x][0];
}

int_fast64_t Tree::get_min_weight(int_fast64_t x, int_fast64_t y){
    x -= 1;
    y -= 1;
    int_fast64_t lca_node = get_lca(x, y);
    int_fast64_t min_weight = INF;
    while (x != lca_node) {
        if (weights[x] < min_weight){
            min_weight = weights[x];
        }
        x = parent[x];
    }

    while (y != lca_node) {
        if (weights[y] < min_weight){
            min_weight = weights[y];
        }
        y = parent[y];
    }

    return min_weight;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::ifstream input_file("minonpath.in");
    std::ofstream output_file("minonpath.out");

    int_fast64_t n, m;
    int_fast64_t x, y;
    Tree tree = Tree();

    input_file >> n;
    for (int_fast64_t i = 0; i < n - 1; i++) {
        input_file >> x >> y;
        tree.add(x, y);
    }

    tree.pre_calc();
    input_file >> m;

    for (int_fast64_t i = 0; i < m; i++) {
        input_file >> x >> y;
        output_file << tree.get_min_weight(x, y) << "\n";
    }
    output_file.close();

    return 0;
}