#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <stack>
#include <algorithm>

int_fast32_t MAX_LEN = 400001;

struct Node {
    int_fast32_t l, r, parent, link = -1;
    std::unordered_map<char, int_fast32_t> children;

    explicit Node(int_fast32_t l = 0, int_fast32_t r = 0,
                  int_fast32_t parent = -1)
            : l(l), r(r), parent(parent) {}

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

    void dfs(std::vector<int_fast32_t> *l, std::vector<int_fast32_t> *lcp);
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

    std::vector<int_fast32_t> l;
    std::vector<int_fast32_t> lcp;
    dfs(&l, &lcp);

    for (int_fast32_t i = 1; i < lcp.size(); i++) {
        std::cout << input_str.size() - l[i] + 1 << " ";
    }

    std::cout << "\n";

    for (int_fast32_t i = 2; i < lcp.size(); i++) {
        std::cout << lcp[i] << " ";
    }

}

void Trie::dfs(std::vector<int_fast32_t> *l, std::vector<int_fast32_t> *lcp) {

    std::stack<int_fast32_t> nodes_stack;
    nodes_stack.push(0);

    std::stack<int_fast32_t> depths_stack;
    depths_stack.push(0);

    int_fast32_t min_depth = 0;

    while (!nodes_stack.empty()) {
        int_fast32_t next_node = nodes_stack.top();
        nodes_stack.pop();

        int_fast32_t next_depth = depths_stack.top();
        depths_stack.pop();

        if (next_depth < min_depth) {
            min_depth = next_depth;
        }

        if (!nodes[next_node].children.empty()) { // если не лист

            std::vector<std::pair<char, int_fast32_t>> children_ord; // для итерации в лексикографическом порядке
            for (auto const &x : nodes[next_node].children) {
                children_ord.emplace_back(x.first, x.second);
            }
            std::sort(children_ord.begin(), children_ord.end());

            for (int_fast32_t i = children_ord.size() - 1; i >= 0; i--) {
                nodes_stack.push(children_ord[i].second);
                depths_stack.push(next_depth + nodes[next_node].r - nodes[next_node].l);
            }

        } else { // лист
            l->push_back(
                    next_depth + std::min<int_fast32_t>(nodes[next_node].r, input_str.size()) - nodes[next_node].l);
            lcp->push_back(min_depth);
            min_depth = next_depth;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::string input_str;
    std::cin >> input_str;
    input_str += "$";

    Trie trie = Trie();

    for (char i : input_str) {
        trie.add_symbol(i);
    }

    trie.get_ans();
    return 0;
}
