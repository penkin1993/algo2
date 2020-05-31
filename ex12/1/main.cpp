#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>

struct Pointer {
    int_fast32_t node;
    int_fast32_t edge;
    int_fast32_t pos; // позиция от начала подстроки
};


class Trie {
public:
    explicit Trie();

    void add_symbol(char symbol);

private:
    void go_down(char symbol); // попытка спуститься вниз

    void go_link(char symbol); // переход по суффиксной ссылке

    void split(); // сплит ребра

    std::string input_string;
    Pointer pointer{}; // указатель: ребро + позиция на нем

    std::vector<int_fast32_t> suffix_links; // суффиксные ссылки

    std::vector<std::vector<std::pair<int_fast32_t, int_fast32_t>>> edges; // ребра
    std::vector<int_fast32_t> join_node; // ссылки на смежную вершину ребра
};

Trie::Trie() {
    // -1 = последний символ решетки !!!
    input_string = "";
    edges.emplace_back(); // инит корневой вершины

    suffix_links.push_back(0); // корень ссылается на самого себя

    pointer.node = 0;
    pointer.edge = -1;
    pointer.pos = 0;
}


void Trie::add_symbol(char symbol) {
    input_string += symbol;
    go_down(symbol);
}

void Trie::go_down(char symbol) {

    if (edges[pointer.node].empty()) { // врешина пуста. => указатель стоит на вершине
        edges[pointer.node].emplace_back();
        edges[pointer.node][0].first = input_string.size() - 1;
        edges[pointer.node][0].second = -1;

        go_link(symbol); // перейти по суффиксной ссылке и попытаться пойти вниз по символу

    } else if (pointer.pos == 0) { // без сплита. указаетель стоит на вершине
        edges[pointer.node][pointer.edge];
        // перебрать в цикле ребра. если не найдем нужное, то добавить ребро


    } else { // указатель стоит на середине ребра




    }
}

void Trie::go_link(char symbol) {


}

void Trie::split() {


}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::string input_s;
    std::cin >> input_s;

    Trie trie = Trie();

    for (char input_ : input_s) {
        trie.add_symbol(input_);
    }

    return 0;
}