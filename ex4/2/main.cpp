/*
B. Дискретное логарифмирование
ограничение по времени на тест3 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Даны натуральные числа a, b, и простое число n. Требуется найти дискретный логарифм b по основанию a по модулю n, то есть такое число x (0⩽x<n), что ax≡b(modn).

Входные данные
В первой строке заданы через пробел три целых числа a, b и n (0⩽a,b<n⩽1012), Число n простое.

Выходные данные
В первой строке выведите −1, если дискретного логарифма не существует. Иначе следует вывести его значение.

Если ответ неоднозначен, разрешается выводить любой.
*/
#include <iostream>
#include <deque>
#include <cmath>
#include <unordered_map>

__int128_t pow_module(int_fast64_t a, int_fast64_t b, int_fast64_t n) {
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

int_fast64_t get_ans(int_fast64_t a, int_fast64_t b, int_fast64_t n) {
    if (a == 0) {
        if (b == 0) {
            return -1;
        } else if (b == 1) {
            return 0;
        } else {
            return -1;
        }

    }
    int_fast64_t k = std::ceil(pow(n, 0.5));

    int_fast64_t r_ans = -1;
    int_fast64_t s_ans = -1;

    std::unordered_map<int_fast64_t, int_fast64_t> s1;

    int_fast64_t res;
    for (int_fast64_t r = 1; r < k + 1; r++) {
        res = pow_module(a, r * k, n);

        if (s1.find(res) == s1.end()) {
            s1.insert({res, r});
        }
    }

    for (int_fast64_t s = 1; s < k + 1; s++) {
        res = (b * pow_module(a, s, n)) % n;

        if (s1.find(res) != s1.end()) {
            r_ans = s1.at(res);
            s_ans = s;
            break;
        }
    }
    if (r_ans == -1) {
        return -1;
    } else {
        return r_ans * k - s_ans;
    }
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    int_fast64_t a, b, n;
    std::cin >> a >> b >> n;

    int_fast64_t ans = get_ans(a, b, n);
    std::cout << ans;

    return 0;
}

