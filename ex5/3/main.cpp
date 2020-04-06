#include <iostream>
#include <vector>

int_fast64_t INF = 1 << 30;

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
    std::vector<int_fast64_t> add_;
    std::vector<int_fast64_t> set_;

    int_fast64_t rmq(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b);

    int_fast64_t get(int_fast64_t v);

    void push(int_fast64_t v, int_fast64_t l, int_fast64_t r);

    void add(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b, int_fast64_t val);

    void set(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b, int_fast64_t val);
};

int_fast64_t Tree::get(int_fast64_t v) {
    if (set_[v] == INF) {
        return t[v] + add_[v];
    } else {
        return set_[v];
    }
}

void Tree::push(int_fast64_t v, int_fast64_t l, int_fast64_t r) {
    if (l == r) {
        if (set_[v] == INF) {
            t[v] += add_[v];
        } else {
            t[v] = set_[v];
        }

    } else {
        add_[2 * v + 1] += add_[v];
        add_[2 * v + 2] += add_[v];

        set_[2 * v + 1] += set_[v];
        set_[2 * v + 2] += set_[v];

        t[v] = std::min(get(2 * v + 1), get(2 * v + 2));

        add_[v] = 0;
        set_[v] = INF;
    }
}

void Tree::add(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b, int_fast64_t val) {
    push(v, l, r);
    if (l > b or r < a) {
        return;
    }
    if ((l >= a) and (r <= b)) {
        add_[v] += val;
        return;
    }
    int_fast64_t m = (l + r) / 2;
    add(2 * v + 1, l, m, a, b, val);
    add(2 * v + 2, m + 1, r, a, b, val);
}


void Tree::add(int_fast64_t a, int_fast64_t b, int_fast64_t val) {
    add(0, 0, x - 1, a - 1, b - 1, val);
}

void Tree::set(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b, int_fast64_t val) {
    push(v, l, r);
    if (l > b or r < a) {
        return;
    }
    if ((l >= a) and (r <= b)) {
        set_[v] = val;
        return;
    }
    int_fast64_t m = (l + r) / 2;
    add(2 * v + 1, l, m, a, b, val);
    add(2 * v + 2, m + 1, r, a, b, val);
}


void Tree::set(int_fast64_t a, int_fast64_t b, int_fast64_t val) {
    set(0, 0, x - 1, a - 1, b - 1, val);
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

Tree::Tree(std::vector<int64_t> *a) {
    x = 1;
    while (x < a->size()) {
        x *= 2;
    }
    t.insert(t.end(), x - 1, 0);

    for (long long i : *a) {
        t.push_back(i);
    }

    for (int_fast64_t v = x - 2; v >= 0; v--) {
        t[v] = std::min(t[2 * v + 1], t[2 * v + 2]);
    }

    t.insert(t.end(), x - a->size(), INF);
    add_.insert(add_.end(), t.size(), 0);
    set_.insert(set_.end(), t.size(), INF);
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
}

/*

5
1 2 3 4 5
min 2 5
min 1 5
min 1 4
min 2 4
set 1 3 10
add 2 4 4
min 2 5
min 1 5
min 1 4
min 2 4


2
1
1
2
5
5
8
8


*/
