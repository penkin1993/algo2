#include <iostream>

// Узел бинарного дерева
struct TreeNode {
    explicit TreeNode(int _value) : value(_value) {}
    int value = 0;
    TreeNode *parent = nullptr;
};

// TODO: Уже рашал ???

// 1. TODO: DFS
// 2. TODO: precalc
// 3. TODO: rest



void precalc(p[]) {
    dfs(root, d) //подсчитываем глубины
    for (int_fast64_t v = 0; v < n; v++) {
        jmp[v][0] = p[v];
    }
    for (k = 1; k < log(n); k++) {
        jmp[v][k] = jmp[jmp[v][k - 1]][k - 1]
    }
}



lca(u, v)
if d[u] < d[v]
swap(u, v)
delta = d[u] – d[v]
for k = log(𝑛) to 0
if delta >= 2
𝑘
        u = jmp[u][k]
delta -= 2
𝑘
if u = v
return u


        lca(u, v)
…
for k = log(𝑛) to 0
u’ = jmp[u][k]
v’ = jmp[v][k]
if u’ ≠ v’
u = u’, v = v’
return jmp[u, 0] //





int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}