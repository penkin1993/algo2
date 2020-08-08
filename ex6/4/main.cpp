/*
D. LCA
ограничение по времени на тест5 секунд
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Дано подвешенное дерево с корнем в первой вершине. Вам нужно ответить на m запросов вида "найти LCA двух вершин". LCA вершин u и v в подвешенном дереве — это наиболее удалённая от корня дерева вершина, лежащая на обоих путях от u и v до корня.

Входные данные
В первой строке задано целое число n — число вершин в дереве (1≤n≤2⋅105).

В следующих n−1 строках записано одно целое число x. Число x на строке i означает, что x — предок вершины i(x<i).

Затем дано число m.

Далее заданы m (0≤m≤5⋅105) запросов вида (u,v) — найти LCA двух вершин u и v (1≤u,v≤n; u≠v).

Выходные данные
Для каждого запроса выведите LCA двух вершин на отдельной строке.
*/

#include <iostream>
#include <vector>
#include <cmath>

class Tree {
public:
    void add(int_fast64_t node_id);

    void pre_calc();

    int_fast64_t get_lca(int_fast64_t x, int_fast64_t y);

private:
    std::vector<int_fast64_t> parent = {0};
    std::vector<int_fast64_t> depth = {0};
    std::vector<std::vector<int_fast64_t>> jmp;
};

void Tree::add(int_fast64_t node_id) {
    parent.push_back(node_id - 1); // добавили новую врешину
    depth.push_back(depth[node_id - 1] + 1); // dfs
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
    x -= 1;
    y -= 1;
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
        return x + 1;
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
    return jmp[x][0] + 1;
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

/*
5
1
1
2
2
3
4 5
4 2
3 5

2
2
1


5
1
1
2
3
2
4 5
2 3

1
1
*/
