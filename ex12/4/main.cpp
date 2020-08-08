/*
D. Ненокку
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Очень известный автор не менее известной книги решил написать продолжение своего произведения. Он писал все свои книги на компьютере, подключенном к интернету. Из-за такой неосторожности мальчику Ненокку удалось получить доступ к еще ненаписанной книге. Каждый вечер мальчик залазил на компьютер писателя и записывал на свой компьютер новые записи. Ненокку, записав на свой компьютер очередную главу, заинтересовался, а использовал ли хоть раз писатель слово "книга". Но он не любит читать книги (он лучше полазает в интернете), и поэтому он просит вас узнать есть ли то или иное слово в тексте произведения. Но естественно его интересует не только одно слово, а достаточно много.

Входные данные
В каждой строчке входного файла записано одна из двух записей.

? <слово> (<слово> - это набор не более 50 латинских символов);
A <текст> (<текст> - это набор не более 105 латинских символов).
1 означает просьбу проверить существование подстроки <слово> в произведение.

2 означает добавление в произведение <текст>.

Писатель только начал работать над произведением, поэтому он не мог написать более 105 символов. А входной файл содержит не более 15 мегабайт информации.

Выходные данные
Выведите на каждую строчку типа 1 "YES", если существует подстрока <слово>, и "NO" в противном случае. Не следует различать регистр букв.
*/
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <algorithm>

int_fast32_t MAX_LEN = 1000001;

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

    void get_ans(std::string *operation, std::string *s);

private:
    std::string input_str = "";

    std::vector<Node> nodes;

    std::pair<int_fast32_t, int_fast32_t> ptr = std::make_pair(0, 0);

    std::pair<int_fast32_t, int_fast32_t> go(std::pair<int_fast32_t, int_fast32_t> st, int_fast32_t l, int_fast32_t r);

    std::pair<int_fast32_t, int_fast32_t>
    go_ones(std::pair<int_fast32_t, int_fast32_t> st, int_fast32_t l, const char *s);

    bool is_contains(std::string *s_);

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
               nodes[v].r));
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

std::pair<int_fast32_t, int_fast32_t>
Trie::go(std::pair<int_fast32_t, int_fast32_t> st, int_fast32_t l, int_fast32_t r) {
    while (l < r)
        if (st.second == nodes[st.first].len()) {
            st = std::make_pair(nodes[st.first].get(input_str.at(l)), 0);
            if (st.first == -1) {
                return st;
            }
        } else {
            if (input_str[nodes[st.first].l + st.second] != input_str.at(l))
                return std::make_pair(-1, -1);
            if (r - l < nodes[st.first].len() - st.second)
                return std::make_pair(st.first, st.second + r - l);
            l += nodes[st.first].len() - st.second;
            st.second = nodes[st.first].len();
        }
    return st;
}


std::pair<int_fast32_t, int_fast32_t>
Trie::go_ones(std::pair<int_fast32_t, int_fast32_t> st, int_fast32_t l, const char *s) {
    int_fast32_t r = l + 1;
    while (true)
        if (st.second == nodes[st.first].len()) {
            st = std::make_pair(nodes[st.first].get(*s), 0);
            if (st.first == -1) {
                return st;
            }
        } else {
            if (input_str[nodes[st.first].l + st.second] != *s) {
                return std::make_pair(-1, -1);
            }
            if (r - l < nodes[st.first].len() - st.second) {
                return std::make_pair(st.first, st.second + r - l);
            }
            st.second = nodes[st.first].len();
            return st;
        }
}

bool Trie::is_contains(std::string *s_) {
    if (!nodes[0].children.count(s_->at(0))) {
        return false;
    }
    if (s_->size() > input_str.size()) {
        return false;
    }

    std::pair<int32_t, int32_t> st = std::make_pair(0, 0);

    for (int_fast32_t i = 0; i < s_->size(); i++) {
        st = go_ones(st, i, &s_->at(i));

        if (st.first == -1) {
            return false;
        }
    }
    return true;
}

void Trie::get_ans(std::string *operation, std::string *s) {
    if (*operation == "?") {
        if (is_contains(s)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }

    if (*operation == "A") {
        for (char i : *s) {
            add_symbol(i);
        }
    }
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::string operation, input_str;

    Trie trie = Trie();

    while (std::cin >> operation >> input_str) {
        std::transform(input_str.begin(), input_str.end(), input_str.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        trie.get_ans(&operation, &input_str);
    }
    return 0;
}
