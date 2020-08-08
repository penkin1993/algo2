/*
A. Первообразный корень по простому модулю
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Дано простое нечётное число p. Требуется найти минимальный первообразный корень по модулю p.

Входные данные
Первая строка входного файла содержит простое число p (3≤p≤109).

Выходные данные
Выведите ответ на поставленную задачу.
*/

#include <iostream>
#include <vector>
#include <deque>

int_fast64_t pow_module(int_fast64_t a, int_fast64_t b, int_fast64_t n) {
    int_fast64_t x = 1, y = a;
    while (b > 0) {
        if (b % 2 == 1) {
            x = (x * y) % n;
        }
        y = (y * y) % n;
        b /= 2;
    }
    return x % n;
}

void get_prime_div(int_fast64_t number, std::vector<int_fast64_t> &prime_div) {
    int_fast64_t n = number;
    for (int i = 2; i < n + 1; i++) {
        if (number % i == 0) {
            prime_div.push_back(i);
        } else {
            continue;
        }
        while (number % i == 0) {
            number = number / i;
        }
        if (number == 1) {
            break;
        }
    }
}

int_fast64_t get_g(int_fast64_t number, std::vector<int_fast64_t> &prime_div){
    bool ind;

    for (int_fast64_t i = 2; i  < number; i++) {
        ind = true;
        for (int_fast64_t j : prime_div) {
            if (pow_module(i, (number - 1) / j, number) == 1) {
                ind = false;
                break;
            }
        }
        if (ind) {
            return i;
        }
    }
    return -1;
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    int_fast64_t number;
    std::cin >> number;

    std::vector<int_fast64_t> prime_div;
    get_prime_div(number - 1, prime_div);
    std::cout << get_g(number, prime_div);
    return 0;
}
