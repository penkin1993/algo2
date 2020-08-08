/*
C. Окна
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
На экране расположены прямоугольные окна, каким-то образом перекрывающиеся (со сторонами, параллельными осям координат). Вам необходимо найти точку, которая покрыта наибольшим числом из них.

Входные данные
В первой строке входного файла записано число окон n (1 ≤ n ≤ 50000). Следующие n строк содержат координаты окон x (1, i) y (1, i) x (2, i) y (2, i), где (x (1, i), y (1, i)) — координаты левого верхнего угла i-го окна, а (x (2, i), y (2, i)) — правого нижнего (на экране компьютера y растет сверху вниз, а x — слева направо). Все координаты — целые числа, по модулю не превосходящие 2·105.

Выходные данные
В первой строке выходного файла выведите максимальное число окон, покрывающих какую-либо из точек в данной конфигурации. Во второй строке выведите два целых числа, разделенные пробелом — координаты точки, покрытой максимальным числом окон. Окна считаются замкнутыми, т.е. покрывающими свои граничные точки.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

int_fast64_t INF = 0;
int_fast64_t MAX_S = 2 * 1e5;

class Tree {
public:
    explicit Tree(std::vector<int64_t> *a);

    ~Tree() = default;

    Tree(const Tree &) = delete;

    Tree(Tree &&) = default;

    Tree &operator=(const Tree &) = delete;

    Tree &operator=(Tree &&) = delete;

    void add(int_fast64_t a, int_fast64_t b, int_fast64_t val);

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
        t[v] = std::max(t[2 * v + 1], t[2 * v + 2]);
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

    return std::max(rmq(2 * v + 1, l, m, a, b), rmq(2 * v + 2, m + 1, r, a, b));
}

int_fast64_t Tree::rmq(int_fast64_t a, int_fast64_t b) {
    return rmq(0, 0, x - 1, a, b);
}

void Tree::add(int_fast64_t a, int_fast64_t b, int_fast64_t val) {
    add(0, 0, x - 1, a, b, val);
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
        t[v] = std::max(get(2 * v + 1), get(2 * v + 2));
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
    t[v] = std::max(get(2 * v + 1), get(2 * v + 2));
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
    std::vector<int_fast64_t> a(2*MAX_S, 0);
    Tree tree = Tree(&a);

    int_fast64_t n;
    std::cin >> n;
    int_fast64_t x1, y1, x2, y2;
    std::vector<std::tuple<int_fast64_t, bool, int_fast64_t, int_fast64_t>> intervals;
    for (int_fast64_t i = 0; i < n; i++) {
        std::cin >> x1 >> y1 >> x2 >> y2;
        intervals.emplace_back(MAX_S + x1, true, MAX_S + y1, MAX_S + y2);
        intervals.emplace_back(MAX_S + x2 + 1, false, MAX_S + y1, MAX_S + y2); // потому что включительно
    }

    sort(intervals.begin(), intervals.end());

    int_fast64_t max_val = 0;
    int_fast64_t loc_max;
    int_fast64_t x_max = 0;
    int_fast64_t y_max = 0;

    std::tuple<int_fast64_t, bool, int_fast64_t, int_fast64_t> interval;
    for (int_fast64_t i = 0; i < 2 * n; i++) {
        interval = intervals[i];

        if (std::get<1>(interval)) {
            tree.add(std::get<2>(interval), std::get<3>(interval), 1);
            loc_max = tree.rmq(0, 2*MAX_S);

            if (loc_max > max_val) {
                max_val = loc_max;
                x_max = std::get<0>(interval);

                int_fast64_t i_ = 0;
                int_fast64_t j_ = 2*MAX_S;

                while (i_ != j_) {
                    int_fast64_t  m = (i_ + j_) / 2;
                    if (tree.rmq(i_, m) == max_val) {
                        j_ = m;
                    } else {
                        i_ = m + 1;
                    }
                }
                y_max = i_;
            }

        } else {
            tree.add(std::get<2>(interval), std::get<3>(interval), -1);
        }

    }
    std::cout << max_val << "\n";
    std::cout << x_max - MAX_S << " " << y_max - MAX_S;

    return 0;
}

