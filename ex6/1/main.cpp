/*
A. Звезды
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Вася любит наблюдать за звездами. Но следить за всем небом сразу ему тяжело. Поэтому он наблюдает только за частью пространства, ограниченной кубом размером n × n × n. Этот куб поделен на маленькие кубики размером 1 × 1 × 1. Во время его наблюдений могут происходить следующие события:

В каком-то кубике появляются или исчезают несколько звезд.
К нему может заглянуть его друг Петя и поинтересоваться, сколько видно звезд в части пространства, состоящей из нескольких кубиков.
Входные данные
Первая строка входного файла содержит натуральное число 1 ≤ n ≤ 128. Координаты кубиков — целые числа от 0 до n - 1. Далее следуют записи о происходивших событиях по одной в строке. В начале строки записано число m. Если m равно:

1, то за ним следуют 4 числа — x, y, z (0 ≤ x, y, z < N) и k ( - 20000 ≤ k ≤ 20000) — координаты кубика и величина, на которую в нем изменилось количество видимых звезд;
2, то за ним следуют 6 чисел — x 1, y 1, z 1, x 2, y 2, z 2 (0 ≤ x 1 ≤ x 2 < N, 0 ≤ y 1 ≤ y 2 < N, 0 ≤ z 1 ≤ z 2 < N), которые означают, что Петя попросил подсчитать количество звезд в кубиках (x, y, z) из области: x 1 ≤ x ≤ x 2, y 1 ≤ y ≤ y 2, z 1 ≤ z ≤ z 2;
3, то это означает, что Васе надоело наблюдать за звездами и отвечать на вопросы Пети. Эта запись встречается во входном файле только один раз и будет последней.
Количество записей во входном файле не больше 100 002.
Выходные данные
Для каждого Петиного вопроса выведите искомое количество звезд.
*/
#include <iostream>
#include <vector>
#include <cmath>


class Tree {

public:
    explicit Tree(int_fast64_t n);

    ~Tree() = default;

    Tree(const Tree &) = delete;

    Tree(Tree &&) = default;

    Tree &operator=(const Tree &) = delete;

    int_fast64_t get(int_fast64_t i, int_fast64_t j, int_fast64_t k);

    void add(int_fast64_t i, int_fast64_t j, int_fast64_t k, int_fast64_t val);

    void set(int_fast64_t i, int_fast64_t j, int_fast64_t k, int_fast64_t val);

    int_fast64_t rsq(int_fast64_t i_l, int_fast64_t j_l, int_fast64_t k_l,
                     int_fast64_t i_r, int_fast64_t j_r, int_fast64_t k_r);

private:
    int_fast64_t size;

    std::vector<int_fast64_t> a;

    std::vector<int_fast64_t> t;

    int_fast64_t get_check(int_fast64_t i, int_fast64_t j, int_fast64_t k);
};

Tree::Tree(int_fast64_t n) {
    size = n;
    a.insert(a.begin(), pow(size, 3), 0);
    t.insert(t.begin(), pow(size, 3), 0);
}

void Tree::set(int_fast64_t i, int_fast64_t j, int_fast64_t k, int_fast64_t val) {
    a[i + j * size + k * size * size] += val;
    add(i, j, k, val);
}

int_fast64_t Tree::get(int_fast64_t i, int_fast64_t j, int_fast64_t k) {
    int_fast64_t res = 0;
    int_fast64_t _j, _k;
    while (i >= 0) {
        _j = j;
        while (_j >= 0) {
            _k = k;
            while (_k >= 0) {
                res += t[i + _j * size + _k * size * size];
                _k = (_k & (_k + 1)) - 1;
            }
            _j = (_j & (_j + 1)) - 1;
        }
        i = (i & (i + 1)) - 1;
    }
    return res;
}

void Tree::add(int_fast64_t i, int_fast64_t j, int_fast64_t k, int_fast64_t val) {
    int_fast64_t _j;
    int_fast64_t _k;
    while (i < size) {
        _j = j;
        while (_j < size) {
            _k = k;
            while (_k < size) {
                t[i + _j * size + _k * size * size] += val;
                _k = _k | (_k + 1);
            }
            _j = _j | (_j + 1);
        }
        i = i | (i + 1);
    }
}

int_fast64_t Tree::get_check(int_fast64_t i, int_fast64_t j, int_fast64_t k) {
    if ((i >= 0) && (j >= 0) && (k >= 0)) {
        return get(i, j, k);
    } else {
        return 0;
    }
}

int_fast64_t Tree::rsq(int_fast64_t i_l, int_fast64_t j_l, int_fast64_t k_l,
                       int_fast64_t i_r, int_fast64_t j_r, int_fast64_t k_r) {

    int_fast64_t res = get(i_r, j_r, k_r);
    res -= (get_check(i_l - 1, j_r, k_r) + get_check(i_r, j_l - 1, k_r) + get_check(i_r, j_r, k_l - 1));
    res += (get_check(i_l - 1, j_l - 1, k_r) +
            get_check(i_r, j_l - 1, k_l - 1) +
            get_check(i_l - 1, j_r, k_l - 1));
    res -= get_check(i_l - 1, j_l - 1, k_l - 1);
    return res;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast64_t n;
    std::cin >> n;

    Tree tree = Tree(n);

    std::string command;
    int_fast64_t x1, y1, z1, x2, y2, z2, val;
    std::cin >> command;
    while (command != "3") {
        if (command == "1") {
            std::cin >> x1 >> y1 >> z1 >> val;
            tree.set(x1, y1, z1, val);
        } else if (command == "2") {
            std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            std::cout << tree.rsq(x1, y1, z1, x2, y2, z2) << "\n";
        }
        std::cin >> command;
    }
}
