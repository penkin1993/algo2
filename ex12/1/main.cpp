#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <unordered_map>

int_fast32_t MAX_LEN = 100000;

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

    std::pair<int_fast32_t, int_fast32_t> go(std::pair<int_fast32_t, int_fast32_t> st, int_fast32_t l,
                                             int_fast32_t r, std::string *s);

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
            go(std::make_pair(to, nodes[to].len()), nodes[v].l + (nodes[v].parent == 0),
               nodes[v].r, &input_str));
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

void Trie::add_symbol(char symbol) {
    int_fast32_t pos = input_str.size();
    input_str += symbol;

    while (true) {
        std::pair<int_fast32_t, int_fast32_t> nptr = go(ptr, pos, pos + 1, &input_str);
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

std::pair<int_fast32_t, int_fast32_t>
Trie::go(std::pair<int_fast32_t, int_fast32_t> st, int_fast32_t l, int_fast32_t r, std::string *s) {
    while (l < r)
        if (st.second == nodes[st.first].len()) {
            st = std::make_pair(nodes[st.first].get(s->at(l)), 0);
            if (st.first == -1) {
                return st;
            }
        } else {
            if (input_str[nodes[st.first].l + st.second] != s->at(l))
                return std::make_pair(-1, -1);
            if (r - l < nodes[st.first].len() - st.second)
                return std::make_pair(st.first, st.second + r - l);
            l += nodes[st.first].len() - st.second;
            st.second = nodes[st.first].len();
        }
    return st;
}



void Trie::get_ans() {
    std::cout << nodes.size() << " " << nodes.size() - 1 << "\n";

    for (int_fast32_t i = 1; i < nodes.size(); i++) {
        std::cout << nodes[i].parent + 1 << " " << i + 1 << " " << nodes[i].l + 1 << " ";
        if (nodes[i].r == MAX_LEN) {
            std::cout << input_str.size() << "\n";
        } else {
            std::cout << nodes[i].r << "\n";
        }
    }


    /*
    std::cout << input_str << "\n";
    for (int_fast32_t i = 0; i < nodes.size(); i++) {
        std::cout << i << " " << input_str.substr(nodes[i].l, nodes[i].r)  << "\n";
    }
    */






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

/*
ababb

7 6
1 4 1 2
1 6 2 2
4 2 3 5
4 5 5 5
6 3 3 5
6 7 5 5


ababb$
*/