#include <iostream>
#include <vector>

class Tree {
public:
    explicit Tree(std::vector<int64_t> *a);

    ~Tree() = default;

    Tree(const Tree &) = delete;

    Tree(Tree &&) = default;

    Tree &operator=(const Tree &) = delete;

    Tree &operator=(Tree &&) = delete;

    void update(int_fast64_t i, int_fast64_t val);

    int_fast64_t rsq(int_fast64_t a, int_fast64_t b);

private:
    int_fast64_t x;
    std::vector<int_fast64_t> t;

    int_fast64_t rsq(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b);
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

    for (int_fast64_t v = x - 2; v >= 0; v--) {
        t[v] = t[2 * v + 1] + t[2 * v + 2];
    }

    t.insert(t.end(), x - a->size(), 0);

    //for (long long i : t) {
    //    std::cout << i << " ";
    //}
    //std::cout << "\n";
}

void Tree::update(int_fast64_t i, int_fast64_t val) {
    int_fast64_t v = x - 1 + i;
    t.at(v) = val;
    while (v != 0) {
        v = (v - 1) / 2;
        t.at(v) = t.at(2 * v + 1) + t.at(2 * v + 2);
    }

    /*for (long long i : t) {
        std::cout << i << " ";
    }
    std::cout << "\n";*/
}


int_fast64_t Tree::rsq(int_fast64_t a, int_fast64_t b) {
    return rsq(0, 0, x - 1, a - 1, b - 1);
}


int_fast64_t Tree::rsq(int_fast64_t v, int_fast64_t l, int_fast64_t r, int_fast64_t a, int_fast64_t b) {

    if ((l > b) or (r < a)) {
        return 0;
    }
    if ((l >= a) and (r <= b)) {
        return t.at(v);
    }
    int_fast64_t m = (l + r) / 2;

    return rsq(2 * v + 1, l, m, a, b) + rsq(2 * v + 2, m + 1, r, a, b);
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
    int_fast64_t i, j;
    getline(std::cin, command);

    for (int row = 0; row < 1e6; ++row) {
        std::cin >> command >> i >> j;
        if (command == "sum") {
            std::cout << tree.rsq(i, j) << std::endl;
        } else if (command == "set") {
            tree.update(i, j);
        }
    }
    return 0;
}



/*
5
1 2 3 4 5
sum 2 5
sum 1 5
sum 1 4
sum 2 4
set 1 10
set 2 3
set 5 2
sum 2 5
sum 1 5
sum 1 4
sum 2 4

*/


/*
14
15
10
9

12
22
20
10
*/



