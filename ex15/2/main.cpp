/*
B. Персистентный массив
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Дан массив (вернее, первая, начальная его версия).

Нужно уметь отвечать на два запроса:

∘ ai[j]=x — создать из i-й версии новую, в которой j-й элемент равен x, а остальные элементы такие же, как в i-й версии.

∘ get ai[j] — сказать, чему равен j-й элемент в i-й версии.

Входные данные
Количество чисел в массиве N (1≤N≤105) и N элементов массива. Далее количество запросов M (1≤M≤105) и M запросов. Формат описания запросов можно посмотреть в примере. Если уже существует K версий, новая версия получает номер K+1. И исходные, и новые элементы массива — целые числа от 0 до 109. Элементы в массиве нумеруются числами от 1 до N.

Выходные данные
На каждый запрос типа get вывести соответствующий элемент нужного массива.
*/

#include <iostream>
#include <vector>
#include <cmath>

struct Node {
    struct Node *left = nullptr, *right = nullptr;
    int_fast32_t val;

    Node() {
        left = nullptr;
        right = nullptr;
        val = NULL;
    }

    ~Node() {
        delete left;
        delete right;
    }
};

class Tree_array {

public:
    explicit Tree_array(std::vector<int_fast32_t> *input_array);

    ~Tree_array() {
        delete nodes_ver.back();
    }

    void set(int_fast32_t ver, int_fast32_t ind, int_fast32_t val);

    int_fast32_t get(int_fast32_t ver, int_fast32_t ind);

private:
    int_fast32_t cur_ver = 1;
    int_fast32_t array_size;
    std::vector<Node *> nodes_ver;

    void add_nodes(Node &node, int_fast32_t left, int_fast32_t right, std::vector<int_fast32_t> *input_array);

    void set_down(Node &node, int_fast32_t left, int_fast32_t right, int_fast32_t pos, int_fast32_t value);

    int_fast32_t get_down(Node &node, int_fast32_t left, int_fast32_t right, int_fast32_t pos);
};

Tree_array::Tree_array(std::vector<int_fast32_t> *input_array) {
    array_size = input_array->size();
    nodes_ver.push_back(new Node());

    add_nodes(*nodes_ver[0], 0, array_size - 1, input_array);
}

void Tree_array::add_nodes(Node &node, int_fast32_t left_ind, int_fast32_t right_ind,
                           std::vector<int_fast32_t> *input_array) {
    if (left_ind == right_ind) {
        node.val = input_array->at(left_ind);
        return;
    }

    int_fast32_t middle = std::floor((left_ind + right_ind) / 2);

    node.left = new Node();
    add_nodes(*node.left, left_ind, middle, input_array);

    node.right = new Node();
    add_nodes(*node.right, middle + 1, right_ind, input_array);
}

int_fast32_t Tree_array::get(int_fast32_t ver, int_fast32_t ind) {
    return get_down(*nodes_ver[ver - 1], 0, array_size - 1, ind - 1);
}

int_fast32_t Tree_array::get_down(Node &node, int_fast32_t left_ind, int_fast32_t right_ind, int_fast32_t pos) {
    if (left_ind == right_ind) {
        return node.val;
    }

    int_fast32_t middle = std::floor((left_ind + right_ind) / 2);
    if (pos <= middle) {
        return get_down(*node.left, left_ind, middle, pos);
    } else {
        return get_down(*node.right, middle + 1, right_ind, pos);
    }
}

void Tree_array::set(int_fast32_t ver, int_fast32_t ind, int_fast32_t val) {
    nodes_ver.push_back(new Node());
    Node *node = nodes_ver.back();
    *node = *nodes_ver[ver - 1];
    nodes_ver[cur_ver] = node;

    set_down(*nodes_ver[cur_ver], 0, array_size - 1, ind - 1, val);
    cur_ver++;
}

void Tree_array::set_down(Node &node, int_fast32_t left_ind, int_fast32_t right_ind,
                          int_fast32_t pos, int_fast32_t value) {
    if (left_ind == right_ind) {
        node.val = value;
        return;
    }

    int_fast32_t middle = std::floor((left_ind + right_ind) / 2);

    if (pos <= middle) {
        Node *left = new Node();
        *left = *node.left;
        node.left = left;
        set_down(*left, left_ind, middle, pos, value);

    } else {
        Node *right = new Node();
        *right = *node.right;
        node.right = right;
        set_down(*right, middle + 1, right_ind, pos, value);
    }
}

int main() {
    int_fast32_t n;
    std::cin >> n;

    std::vector<int_fast32_t> input_array;
    int_fast32_t val;

    for (int_fast32_t i = 0; i < n; i++) {
        std::cin >> val;
        input_array.push_back(val);
    }

    Tree_array tree_array = Tree_array(&input_array);

    int_fast32_t m;
    std::cin >> m;

    int_fast32_t ver, pos, value;
    std::string command;

    for (int_fast32_t i = 0; i < m; i++) {
        std::cin >> command;
        if (command == "create") {
            std::cin >> ver >> pos >> value;
            tree_array.set(ver, pos, value);

        } else if (command == "get") {
            std::cin >> ver >> pos;
            std::cout << tree_array.get(ver, pos) << "\n";
        }
    }
}
