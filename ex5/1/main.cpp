/*
A. Сумма простая
ограничение по времени на тест1 секунда
ограничение по памяти на тест512 мегабайт
вводстандартный ввод
выводстандартный вывод
Вам нужно научиться отвечать на запрос «сумма чисел на отрезке».

Массив не меняется. Запросов много. Отвечать на каждый запрос следует за .

Входные данные
Размер массива — n и числа x, y, a 0, порождающие массив a: 

Далее следует количество запросов m и числа z, t, b 0, порождающие массив b: .

Массив c строится следующим образом: .

Запросы: i-й из них — найти сумму на отрезке от min(c 2i, c 2i + 1) до max(c 2i, c 2i + 1) в массиве a.

Ограничения: 1 ≤ n ≤ 107, 0 ≤ m ≤ 107. Все числа целые от 0 до 216. t может быть равно  - 1.

Выходные данные
Выведите сумму всех сумм.
*/
#include <iostream>
#include <vector>


void sum_init(int_fast64_t n, int_fast64_t x, int_fast64_t y, int_fast64_t a0, std::vector<int_fast64_t> *sum) {
    sum->push_back(a0);
    int_fast64_t mod = 1 << 16;
    int_fast64_t new_element = a0;

    for (int_fast64_t i = 0; i < n - 1; i++) {
        new_element = (x * new_element + y) % mod;
        sum->push_back(sum->back() + new_element);
    }
}

int_fast64_t get_sum(int_fast64_t c_min, int_fast64_t c_max, const std::vector<int_fast64_t> *sum) {
    if (c_min == 0) {
        return sum->at(c_max);
    } else {
        return sum->at(c_max) - sum->at(c_min - 1);
    }
}

int_fast64_t next_number(int_fast64_t b, int_fast64_t z, int_fast64_t t, int_fast64_t mod) {
    int_fast64_t next_num = (z * b + t) % mod;
    if (next_num < 0) {
        next_num = mod + next_num;
    }
    return next_num;
}

void get_all_sum(int_fast64_t n, int_fast64_t m, int_fast64_t z, int_fast64_t t, int_fast64_t b0,
                 const std::vector<int_fast64_t> *sum) {
    int_fast64_t mod = 1 << 30;
    int_fast64_t b_even = b0;
    int_fast64_t b_odd = next_number(b_even, z, t, mod);

    int_fast64_t c_even, c_odd;
    int_fast64_t s = 0;

    for (int_fast64_t i = 0; i < m; i++) {
        c_even = b_even % n;
        c_odd = b_odd % n;
        s += get_sum(std::min(c_even, c_odd), std::max(c_even, c_odd), sum);

        b_even = next_number(b_odd, z, t, mod);
        b_odd = next_number(b_even, z, t, mod);
    }
    std::cout << s;
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    int_fast64_t n, x, y, a0;
    int_fast64_t m, z, t, b0;

    std::cin >> n >> x >> y >> a0;
    std::cin >> m >> z >> t >> b0;

    std::vector<int_fast64_t> sum;
    sum_init(n, x, y, a0, &sum);

    get_all_sum(n, m, z, t, b0, &sum);


    return 0;
}
