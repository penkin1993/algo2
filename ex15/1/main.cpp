#include <iostream>
#include <vector>

struct Element {
    int_fast32_t v, val, p; // версия значение, указатель
    explicit Element(int_fast32_t v_, int_fast32_t val_, int_fast32_t p_) : v(v_), val(val_), p(p_) {};
};


class Stack {

public:
    explicit Stack() {
        elements.emplace_back(0, -1, -1);
    }

    int_fast32_t pop(int_fast32_t t);

    void push(int_fast32_t t, int_fast32_t m);

private:
    std::vector<Element> elements;
};

int_fast32_t Stack::pop(int_fast32_t t) {
    Element element = elements.at(t);
    while (true) {
        if (element.val != -1) {
            Element new_element = Element(elements.size(), -1, element.p);
            elements.push_back(new_element);
            return element.val;
        } else {
            element = elements.at(element.p);
        }
    }
}

void Stack::push(int_fast32_t t, int_fast32_t m) {
    Element new_element = Element(elements.size(), m, t);
    elements.push_back(new_element);
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    int_fast32_t n;
    std::cin >> n;

    Stack stack = Stack();

    int_fast32_t t, m;

    for (int_fast32_t i = 0; i < n; i++) {
        std::cin >> t >> m;
        if (m == 0) {
            std::cout << stack.pop(t) << "\n";
        } else {
            stack.push(t, m);
        }
    }
    return 0;
}
