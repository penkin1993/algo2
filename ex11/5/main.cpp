/*
E. Циклические сдвиги
ограничение по времени на тест2 секунды
ограничение по памяти на тест512 мегабайт
вводстандартный ввод
выводстандартный вывод
k-м циклическим сдвигом строки S называется строка, полученная перестановкой k первых символов строки S в конец строки.

Рассмотрим все различные циклические сдвиги строки S и отсортируем их по возрастанию. Требуется вычислить i-ю строчку этого массива.

Например, для строки abacabac существует четыре различных циклических сдвига: нулевой (abacabac), первый (bacabaca), второй (acabacab) и третий (cabacaba). После сортировки по возрастанию получится такой массив: abacabac, acabacab, bacabaca, cabacaba.

Входные данные
В первой строке входного файла записана строка S, длиной не более 100 000 символов с ASCII-кодами от 32 до 126. Во второй строке содержится единственное целое число k (1 ≤ k ≤ 100 000).

Выходные данные
В выходной файл выведите k-й по возрастанию циклический сдвиг строки S, или слово IMPOSSIBLE, если такого сдвига не существует.
*/

#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

struct Item {
    char symbol;
    int_fast32_t p;
    int_fast32_t c;

    Item(char symbol_, int_fast32_t p_, int_fast32_t c_) {
        symbol = symbol_;
        p = p_;
        c = c_;
    }

    bool operator<(const Item &playerObj) const {
        return symbol < playerObj.symbol;
    }
};


inline int_fast32_t positive_mod(int_fast32_t i, int_fast32_t n) {
    return (i % n + n) % n;
}

bool radix_sort(std::vector<Item> *items, std::vector<Item> *new_items, std::vector<int_fast32_t> *pos,
                int_fast32_t k, int_fast32_t *counter) {

    // получить модуль сдвига
    int_fast32_t mod = -(1 << k);

    std::vector<int_fast32_t> array_counter(*counter + 1, 0);
    std::vector<int_fast32_t> first_c;

    // сформировать первый разряд
    for (int_fast32_t i = 0; i < items->size(); i++) {
        int_fast32_t prev_ind = positive_mod(items->at(i).p + mod, items->size());
        first_c.push_back(items->at(pos->at(prev_ind)).c);
        array_counter[first_c.back()] += 1;

        new_items->at(i).symbol = items->at(pos->at(prev_ind)).symbol;
        new_items->at(i).p = items->at(pos->at(prev_ind)).p;
    }

    // поменять ссылки
    for (int_fast32_t i = 0; i < items->size(); i++) {
        items->at(i).symbol = new_items->at(i).symbol;
        items->at(i).p = new_items->at(i).p;
    }

    // сформировать индексы вставки
    int_fast32_t t1 = array_counter[0];
    int_fast32_t t2;
    array_counter[0] = 0;
    for (int_fast32_t i = 1; i < array_counter.size(); i++) {
        t2 = array_counter[i];
        array_counter[i] = array_counter[i - 1] + t1;
        t1 = t2;
    }

    // сортировка
    std::vector<int_fast32_t> new_first_c(first_c.size(), 0);
    for (int_fast32_t i = 0; i < items->size(); i++) {
        int_fast32_t ind = array_counter[first_c[i]];

        new_items->at(ind).symbol = items->at(i).symbol;
        new_items->at(ind).p = items->at(i).p;
        new_items->at(ind).c = items->at(i).c;

        array_counter[first_c[i]] += 1;
        new_first_c[ind] = first_c[i];
    }

    int_fast32_t new_counter = 0;
    int_fast32_t prev = new_items->at(0).c;
    new_items->at(0).c = 0;

    for (int_fast32_t i = 1; i < new_items->size(); i++) {
        if ((new_first_c[i] != new_first_c[i - 1]) | (new_items->at(i).c != prev)) {
            new_counter++;
        }
        prev = new_items->at(i).c;
        new_items->at(i).c = new_counter;
    }

    // сформировать новый pos
    for (int_fast32_t i = 0; i < new_items->size(); i++) {
        pos->at(new_items->at(i).p) = i;

        items->at(i).symbol = new_items->at(i).symbol;
        items->at(i).p = new_items->at(i).p;
        items->at(i).c = new_items->at(i).c;
    }
    *counter = new_counter;
    return new_counter != items->size() - 1;
}

void get_ans(std::string input_s, int32_t k_pos) {
    std::vector<Item> items;
    std::vector<Item> new_items;
    //input_s += '$'; // определили исходный массив порядков

    std::vector<int_fast32_t> pos(input_s.size(), 0);

    // определили массив p и c
    for (int_fast32_t i = 0; i < input_s.size(); i++) {
        items.emplace_back(Item(input_s[i], i, 0));
        new_items.emplace_back(Item(input_s[i], i, 0));
    }
    std::sort(items.begin(), items.end());

    for (int_fast32_t i = 0; i < items.size(); i++) {
        pos[items[i].p] = i;
    }

    // задание классов классов
    int_fast32_t counter = 0;
    for (int_fast32_t i = 1; i < items.size(); i++) {
        if (items[i].symbol != items[i - 1].symbol) {
            counter++;
        }
        items[i].c = counter;
    }
    // цифровая сортировка
    int32_t k = 0;
    int_fast32_t n = std::ceil(std::log2(input_s.size())); // чтобы не было зацикливания
    for (int_fast32_t i = 0; i < n; i++) {
        radix_sort(&items, &new_items, &pos, k, &counter);
        k++;
    }

    if (k_pos > items.back().c + 1) {
        std::cout << "IMPOSSIBLE";
        return;
    }
    for (auto &item : items) {
        if ((item.c + 1) == k_pos) {
            for (int32_t j = item.p; j < input_s.size(); j++) {
                std::cout << input_s[j];
            }
            for (int32_t j = 0; j < item.p; j++) {
                std::cout << input_s[j];
            }
            return;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::string input_s;
    std::cin >> input_s;

    int32_t k_pos;
    std::cin >> k_pos;
    get_ans(input_s, k_pos);
    return 0;
}
