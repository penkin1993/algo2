/*
F. RMQ наоборот
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводrmq.in
выводrmq.out
Рассмотрим массив a[1..n]. Пусть Q(i, j) — ответ на запрос о нахождении минимума среди чисел a[i], ..., a[j]. Вам даны несколько запросов и ответы на них. Восстановите исходный массив.

Входные данные
Первая строка входного файла содержит число n — размер массива, и m — число запросов (1 ≤ n, m ≤ 100 000). Следующие m строк содержат по три целых числа i, j и q, означающих, что Q(i, j) = q (1 ≤ i ≤ j ≤ n,  - 231 ≤ q ≤ 231 - 1).

Выходные данные
Если искомого массива не существует, выведите строку «inconsistent».

В противном случае в первую строку выходного файла выведите «consistent». Во вторую строку выходного файла выведите элементы массива. Элементами массива должны быть целые числа в интервале от  - 231 до 231 - 1 включительно. Если решений несколько, выведите любое.
*/
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

int_fast64_t INF = pow(2, 31) - 1;

class Tree {
public:
    explicit Tree(std::vector<int64_t> *a);

    ~Tree() = default;

    Tree(const Tree &) = delete;

    Tree(Tree &&) = default;

    Tree &operator=(const Tree &) = delete;

    Tree &operator=(Tree &&) = delete;

    void set(int_fast64_t a, int_fast64_t b, int_fast64_t val);

    int_fast64_t rmq(int_fast64_t a, int_fast64_t b);

private:
    int_fast64_t x;
    std::vector<int_fast64_t> t;
    std::vector<int_fast64_t> upd;
    std::vector<bool> cond;

    int_fast64_t rmq(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b);

    int_fast64_t get(int_fast64_t v);

    void push(int_fast64_t v, int_fast64_t l, int_fast64_t r);

    void add(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b, int_fast64_t val);

    void set(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b, int_fast64_t val);
};

Tree::Tree(std::vector<int64_t> *a) {
    x = 1;
    while (x < a->size()) {
        x *= 2;
    }
    t.insert(t.end(), x - 1, 0);

    for (long long i : *a) {
        t.push_back(i);
    }

    t.insert(t.end(), x - a->size(), INF);
    upd.insert(upd.end(), t.size(), 0);
    cond.insert(cond.end(), t.size(), false);

    for (int_fast64_t v = x - 2; v >= 0; v--) {
        t[v] = std::min(t[2 * v + 1], t[2 * v + 2]);
    }
}

int_fast64_t Tree::rmq(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b) {
    push(v, l, r);

    if ((l > b) or (r < a)) {
        return INF;
    }

    if ((l >= a) and (r <= b)) {
        return get(v);
    }
    int_fast64_t m = (l + r) / 2;

    return std::min(rmq(2 * v + 1, l, m, a, b), rmq(2 * v + 2, m + 1, r, a, b));
}

int_fast64_t Tree::rmq(int_fast64_t a, int_fast64_t b) {
    return rmq(0, 0, x - 1, a - 1, b - 1);
}


void Tree::set(int_fast64_t a, int_fast64_t b, int_fast64_t val) {
    set(0, 0, x - 1, a - 1, b - 1, val);
}

int_fast64_t Tree::get(int_fast64_t v) {
    if (!cond[v]) {
        return t[v] + upd[v];
    } else {
        return upd[v];
    }
}

void Tree::push(int_fast64_t v, int_fast64_t l, int_fast64_t r) {
    if (l == r) {
        if (!cond[v]) {
            t[v] += upd[v];
        } else {
            t[v] = upd[v];
        }

    } else {
        if (cond[v]) {
            cond[2 * v + 1] = cond[v];
            cond[2 * v + 2] = cond[v];
            upd[2 * v + 1] = upd[v];
            upd[2 * v + 2] = upd[v];
        } else {
            upd[2 * v + 1] += upd[v];
            upd[2 * v + 2] += upd[v];
        }
        t[v] = std::min(get(2 * v + 1), get(2 * v + 2));
    }
    cond[v] = false;
    upd[v] = 0;
}

void Tree::add(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b, int_fast64_t val) {
    push(v, l, r);
    if ((l > b) or (r < a)) {
        return;
    }
    if ((l >= a) and (r <= b)) {
        upd[v] += val;
        return;
    }
    int_fast64_t m = (l + r) / 2;
    add(2 * v + 1, l, m, a, b, val);
    add(2 * v + 2, m + 1, r, a, b, val);
    t[v] = std::min(get(2 * v + 1), get(2 * v + 2));
}

void Tree::set(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b, int_fast64_t val) {
    push(v, l, r);
    if ((l > b) or (r < a)) {
        return;
    }
    if ((l >= a) and (r <= b)) {
        upd[v] = val;
        cond[v] = true;
        return;
    }

    int_fast64_t m = (l + r) / 2;
    set(2 * v + 1, l, m, a, b, val);
    set(2 * v + 2, m + 1, r, a, b, val);
    t[v] = std::min(get(2 * v + 1), get(2 * v + 2));
}

struct Requests {
    int_fast64_t i, j, q;

    explicit Requests(int_fast64_t i_, int_fast64_t j_, int_fast64_t q_) : i(i_), j(j_), q(q_) {};

    bool operator<(const Requests &other) const {
        return q < other.q;
    }
};


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::ifstream input_file("rmq.in");
    std::ofstream output_file("rmq.out");

    int_fast64_t n, m;
    input_file >> n >> m;

    std::vector<int_fast64_t> a(n, INF);

    Tree tree = Tree(&a);

    std::vector<Requests> requests;

    int_fast64_t i_, j_, q_;
    for (int_fast64_t i = 0; i < m; i++) {
        input_file >> i_ >> j_ >> q_;
        requests.emplace_back(i_, j_, q_);
    }
    std::sort(requests.begin(), requests.end());

    for (int_fast64_t i = 0; i < m; i++) {
        std::cout << requests[i].i << " " << requests[i].j << "\n";
        tree.set(requests[i].i, requests[i].j, requests[i].q);
    }

    for (int_fast64_t i = 0; i < m; i++) {
        if (tree.rmq(requests[i].i, requests[i].j) != requests[i].q) {
            output_file << "inconsistent\n";
            return 0;
        }
    }

    output_file << "consistent\n";
    for (int_fast64_t i = 1; i < n + 1; i++) {
        output_file << tree.rmq(i, i) << " ";
    }

    return 0;
}
