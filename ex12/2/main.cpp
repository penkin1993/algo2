/*
B. Количество подстрок
ограничение по времени на тест2 секунды
ограничение по памяти на тест512 мегабайт
вводстандартный ввод
выводстандартный вывод
Вычислите количество различных подстрок строки s.

Входные данные
Единственная строка входного файла содержит строку s (1 ≤ |s| ≤ 400 000). Строка состоит из строчных латинских букв.

Выходные данные
Выведите одно число — ответ на задачу.
*/
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <unordered_map>

int_fast32_t MAX_LEN = 400000;

struct Node {
    explicit Node(int_fast32_t l = 0, int_fast32_t r = 0,
                  int_fast32_t parent = -1)
            : l(l), r(r), parent(parent) {}

    int_fast32_t l, r, parent, link = -1;
    std::unordered_map<char, int_fast32_t> children;

    int_fast32_t len() const { return r - l; }

    int_fast32_t &get(char c) {
        if (!children.count(c)) children[c] = -1;
        return children[c];
    }
};


class Trie {

public:
    explicit Trie() {
        nodes.emplace_back();
    }

    void add_symbol(char symbol);

    void get_ans();

private:
    std::string input_str = "";

    std::vector<Node> nodes;

    std::pair<int_fast32_t, int_fast32_t> ptr = std::make_pair(0, 0);

    std::pair<int_fast32_t, int_fast32_t> go(std::pair<int_fast32_t, int_fast32_t> st, int_fast32_t l, int_fast32_t r);

    int_fast32_t split(std::pair<int32_t, int32_t> st);

    int_fast32_t get_link(int_fast32_t v);
};


int_fast32_t Trie::get_link(int_fast32_t v) {
    if (nodes[v].link != -1) {
        return nodes[v].link;
    }
    if (nodes[v].parent == -1) {
        return 0;
    }
    int_fast32_t to = get_link(nodes[v].parent);

    return nodes[v].link = split(
            go(std::make_pair(to, nodes[to].len()), nodes[v].l + (nodes[v].parent == 0), nodes[v].r));
}

int_fast32_t Trie::split(std::pair<int32_t, int32_t> st) {
    if (st.second == nodes[st.first].len())
        return st.first;

    if (st.second == 0)
        return nodes[st.first].parent;

    Node v = nodes[st.first];

    int_fast32_t id = nodes.size();
    nodes.emplace_back();
    nodes[id] = Node(v.l, v.l + st.second, v.parent);
    nodes[v.parent].get(input_str[v.l]) = id;
    nodes[id].get(input_str[v.l + st.second]) = st.first;
    nodes[st.first].parent = id;
    nodes[st.first].l += st.second;
    return id;
}

std::pair<int_fast32_t, int_fast32_t>
Trie::go(std::pair<int_fast32_t, int_fast32_t> st, int_fast32_t l, int_fast32_t r) {
    while (l < r)
        if (st.second == nodes[st.first].len()) {
            st = std::make_pair(nodes[st.first].get(input_str[l]), 0);
            if (st.first == -1) {
                return st;
            }
        } else {
            if (input_str[nodes[st.first].l + st.second] != input_str[l])
                return std::make_pair(-1, -1);
            if (r - l < nodes[st.first].len() - st.second)
                return std::make_pair(st.first, st.second + r - l);
            l += nodes[st.first].len() - st.second;
            st.second = nodes[st.first].len();
        }
    return st;
}

void Trie::add_symbol(char symbol) {
    int_fast32_t pos = input_str.size();
    input_str += symbol;

    while (true) {
        std::pair<int_fast32_t, int_fast32_t> nptr = go(ptr, pos, pos + 1);
        if (nptr.first != -1) {
            ptr = nptr;
            return;
        }
        int_fast32_t mid = split(ptr);

        int_fast32_t leaf = nodes.size();
        nodes.emplace_back();

        nodes[leaf] = Node(pos, MAX_LEN, mid);
        nodes[mid].get(input_str[pos]) = leaf;

        ptr.first = get_link(mid);
        ptr.second = nodes[ptr.first].len();
        if (!mid) break;
    }
}

void Trie::get_ans() {
    int_fast64_t count = 0;
    for (auto &node : nodes) {
        if (node.r == MAX_LEN) {
            count += input_str.size() - node.l;
        } else {
            count += node.r - node.l;
        }
    }
    std::cout << count;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::string input_str;
    std::cin >> input_str;

    Trie trie = Trie();

    for (char i : input_str) {
        trie.add_symbol(i);
    }

    trie.get_ans();
    return 0;
}

