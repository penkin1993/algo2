#include <algorithm>
#include <iostream>
#include <vector>

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

void is_in_s(std::string input_s, std::vector<Item> *items, std::vector<int_fast32_t> *pos,
             std::vector<std::string> *all_str) {

    input_s = input_s.substr(0, input_s.size() - 1);
    int_fast32_t input_s_size = input_s.size();
    bool is_yes;

    for (const auto &check_str : *all_str) {
        is_yes = false;
        int_fast32_t left = 1;
        int_fast32_t right = items->size() - 1;
        int_fast32_t mid;

        while (left <= right) {
            mid = (left + right) / 2;

            std::string current_str = input_s.substr(items->at(mid).p,
                                                     std::min<int_fast32_t>(check_str.size(),
                                                                            input_s_size - items->at(mid).p));

            if (check_str == current_str) {
                is_yes = true;
                break;
            }
            if (check_str < current_str)
                right = mid - 1;
            else
                left = mid + 1;
        }
        if (is_yes) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
}

void get_ans(std::string input_s, std::vector<std::string> *all_str) {
    std::vector<Item> items;
    std::vector<Item> new_items;
    input_s += '$'; // определили исходный массив порядков

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
    while (radix_sort(&items, &new_items, &pos, k, &counter)) {
        k++;
    }
    is_in_s(input_s, &items, &pos, all_str);

}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast32_t n;
    std::cin >> n;

    std::vector<std::string> all_str;
    std::string s;

    for (int_fast32_t i = 0; i < n; i++) {
        std::cin >> s;
        all_str.push_back(s);
    }

    std::string input_s;
    std::cin >> input_s;
    get_ans(input_s, &all_str);
    return 0;
}

