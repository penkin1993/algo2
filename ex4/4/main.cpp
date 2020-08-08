/*
D. Проверка на простоту
ограничение по времени на тест1 секунда
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Дано n натуральных чисел a i. Определите для каждого числа, является ли оно простым.

Входные данные
Программа получает на вход число n, 1 ≤ n ≤ 1000 и далее n чисел a i, 1 ≤ a i ≤ 1018.

Выходные данные
Если число a i простое, программа должна вывести YES, для составного числа программа должна вывести NO.

*/
#include <iostream>
#include <vector>
#include <random>

__int128_t pow_module(__int128_t a, __int128_t b, __int128_t n) {
    __int128_t x = 1, y = a;
    while (b > 0) {
        if (b % 2 == 1) {
            x = (x * y) % n;
        }
        y = (y * y) % n;
        b /= 2;
    }
    return x % n;
}

bool is_prime(__int128_t a, __int128_t s, __int128_t pow_2, __int128_t m) {
    __int128_t rest;
    while (pow_2 > 0) {
        rest = pow_module(a, s * (1 << pow_2), m);
        if (rest == 1){
            pow_2--;
            continue;
        } else return rest == m - 1;
    }
    return true;
}


void is_prime(__int128_t m) {
    if ((m == 2) || (m == 3)) {
        std::cout << "\nYES";
        return;
    }
    if ((m == 1) || (m % 2 == 0)) {
        std::cout << "\nNO";
        return;
    }
    std::random_device r;
    std::default_random_engine generator(r());
    std::uniform_int_distribution<int_fast64_t> distribution(2, m - 2);

    __int128_t s = m - 1;
    __int128_t pow_2 = 0;

    while (s % 2 == 0) {
        pow_2++;
        s = s / 2;
    }

    __int128_t a;
    for (int i = 0; i < 20; i++) {
        a = distribution(generator);
        if (!is_prime(a, s, pow_2, m)) {
            std::cout << "\nNO";
            return;
        }
    }
    std::cout << "\nYES";
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast64_t n, m;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        std::cin >> m;
        is_prime(m);
    }
    return 0;
}
