/*
A. Персистентный стек
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Реализуйте персистентный стек.

Входные данные
Первая строка содержит количество действий n (1≤n≤200000). В строке номер i+1 содержится описание действия i:.

t m — добавить в конец стека номер t (0≤t<i) число m (0<m≤1000);
t 0 — удалить последний элемент стека номер t (0≤t<i). Гарантируется, что стек t не пустой.
В результате действия i, описанного в строке i+1, создается стек номер i. Изначально имеется пустой стек с номером ноль.
Все числа во входном файле целые.

Выходные данные
Для каждой операции удаления выведите удаленный элемент на отдельной строке.
*/

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
