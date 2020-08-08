/*
C. RMQ2
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Входные данные
В первой строке находится число n — размер массива. (1 ≤ n ≤ 105) Во второй строке находится n чисел a i — элементы массива. Далее содержится описание операций, их количество не превышает 2·105. В каждой строке находится одна из следующих операций:

set i j x — установить все a[k], i ≤ k ≤ j в x.
add i j x — увеличить все a[k], i ≤ k ≤ j на x.
min i j — вывести значение минимального элемента в массиве на отрезке с i по j, гарантируется, что (1 ≤ i ≤ j ≤ n).
Все числа во входном файле и результаты выполнения всех операций не превышают по модулю 1018.
Выходные данные
Выведите последовательно результат выполнения всех операций min. Следуйте формату выходного файла из примера.
*/
#include <iostream>
#include <vector>
#include <cmath>

int_fast64_t INF = pow(10, 18) + 1;

class Tree {
public:
    explicit Tree(std::vector<int64_t> *a);

    ~Tree() = default;

    Tree(const Tree &) = delete;

    Tree(Tree &&) = default;

    Tree &operator=(const Tree &) = delete;

    Tree &operator=(Tree &&) = delete;

    void add(int_fast64_t a, int_fast64_t b, int_fast64_t val);

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
    //for (long long i : t) {
    //    std::cout << i << " ";
    //}
    //std::cout << "\n";
    return rmq(0, 0, x - 1, a - 1, b - 1);
}

void Tree::add(int_fast64_t a, int_fast64_t b, int_fast64_t val) {
    add(0, 0, x - 1, a - 1, b - 1, val);
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
        } else{
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

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast64_t n;
    std::cin >> n;

    std::vector<int_fast64_t> a;
    int_fast64_t num;

    for (int_fast64_t i = 0; i < n; i++) {
        std::cin >> num;
        a.push_back(num);
    }
    Tree tree = Tree(&a);

    std::string command;
    int_fast64_t i, j, val;
    getline(std::cin, command);
    while (std::cin >> command) {
        if (command == "min") {
            std::cin >> i >> j;
            std::cout << tree.rmq(i, j) << "\n";
        } else if (command == "set") {
            std::cin >> i >> j >> val;
            tree.set(i, j, val);
        } else if (command == "add") {
            std::cin >> i >> j >> val;
            tree.add(i, j, val);
        }
    }
    return 0;
}

