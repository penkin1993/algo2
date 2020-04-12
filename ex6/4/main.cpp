#include <iostream>
#include <vector>
#include <cmath>

struct TreeNode {
    explicit TreeNode(int_fast64_t _parent) : parent(_parent) {}

    int_fast64_t parent;
    int_fast64_t depth = - 1;
    std::vector<int_fast64_t> children;
    std::vector<int_fast64_t> jmp;
};

class Tree {
public:
    void add(int_fast64_t node_id);

    void pre_calc();

    int_fast64_t get_lca(int_fast64_t x, int_fast64_t y);

private:
    std::vector<TreeNode> nodes;
};

void Tree::add(int_fast64_t node_id) {
    nodes.emplace_back(node_id - 1); // добавили новую врешину
    nodes.back().parent = nodes[node_id -1].depth + 1; // dfs
    nodes[node_id - 1].children.push_back(nodes.size() - 1); // сообщили родителю о новом ребенке
}


void Tree::pre_calc() {
    for (auto & node : nodes) {
        node.jmp.push_back(node.parent);
    }
    for (int_fast64_t k = 1; k < std::ceil(log(nodes.size())) + 1; k++) {
        for (auto & node : nodes) {
            node.jmp.push_back(nodes[node.jmp[k-1]].jmp[k-1]);
        }
    }
}

int_fast64_t get_lca(int_fast64_t x, int_fast64_t y){




}




int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast64_t n, m;
    int_fast64_t x, y;
    Tree tree = Tree();

    std::cin >> n;
    for (int_fast64_t i = 0; i < n - 1; i++) {
        std::cin >> x;
        tree.add(x);
    }

    tree.pre_calc();
    std::cin >> m;
    for (int_fast64_t i = 0; i < m; i++) {
        std::cin >> x >> y;
        std::cout << tree.get_lca(x, y) << "\n";
    }
    return 0;
}