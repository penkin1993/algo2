#include <iostream>
#include <vector>
#include <algorithm>

int_fast64_t ZERO = 0;
int_fast64_t MAX_S = 400002;
int_fast64_t SHIFT = 200001;

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

    t.insert(t.end(), x - a->size(), ZERO);
    upd.insert(upd.end(), t.size(), ZERO);
    cond.insert(cond.end(), t.size(), false);

    for (int_fast64_t v = x - 2; v >= 0; v--) {
        t[v] = std::max(t[2 * v + 1], t[2 * v + 2]);
    }
}

int_fast64_t Tree::rmq(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b) {
    push(v, l, r);

    if ((l > b) or (r < a)) {
        return ZERO;
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
        intervals.emplace_back(x1 + SHIFT, true, y1 + SHIFT, y2 + SHIFT);
        intervals.emplace_back(x2 + SHIFT + 1, false, y1 + SHIFT, y2 + SHIFT); // потому что включительно
    }

    sort(intervals.begin(), intervals.end());

    int_fast64_t max_val = 0;
    int_fast64_t loc_max = 0;
    int_fast64_t x_max = 0;
    int_fast64_t y_max = 0;

    std::tuple<int_fast64_t, bool, int_fast64_t, int_fast64_t> interval;
    for (auto & i : intervals) {
        interval = i;
        //std::cout << std::get<0>(interval) << ", " << std::get<1>(interval) << ", " << std::get<2>(interval) << ", " << std::get<3>(interval) << '\n';

        if (std::get<1>(interval)) {
            tree.add(std::get<2>(interval), std::get<3>(interval), 1);
            loc_max = tree.rmq(0, 2*MAX_S);
            if (loc_max > max_val) {// проверка максимума (максимум x1, y1 последнего добавленного)
                max_val = loc_max;
                x_max = std::get<0>(interval);
                if (tree.rmq(std::get<3>(interval), std::get<3>(interval)) == max_val) { ;
                    y_max = std::get<3>(interval);
                } else{
                    y_max = std::get<2>(interval);
                }
            }
        } else {
            tree.add(std::get<2>(interval), std::get<3>(interval), -1);
        }
    }
    std::cout << max_val << "\n";
    std::cout << x_max - SHIFT << " " << y_max - SHIFT;

    return 0;
}

/*
2
0 0 3 3
1 1 4 4

//std::cout << "add " << std::get<2>(interval)  << " " <<  std::get<3>(interval) << "\n";
//std::cout << loc_max << "\n";


//std::cout << std::get<0>(interval) << ", " << std::get<1>(interval) << ", " << std::get<2>(interval) << ", " << std::get<3>(interval) << '\n';
//std::cout << "del " << std::get<2>(interval)  << " " <<  std::get<3>(interval) << "\n";
*/



