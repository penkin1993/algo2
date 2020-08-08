/*
E. Факторизация
ограничение по времени на тест3 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Дано натуральное число. Факторизуйте его, то есть представьте в виде произведения набора простых чисел. Число p называется простым, если имеет ровно два различных натуральных делителя: 1 и p.

Входные данные
В единственной строке записано единственное натуральное число N. 2 ≤ N ≤ 9·1018.

Выходные данные
Выведите в неубывающем порядке одно или несколько простых чисел, произведение которых равно N.
*/

#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>

int64_t get_gcd(int64_t a, int64_t b) {
    if (b < a) {
        int64_t c;
        c = b;
        b = a;
        a = c;
    };
    if (a == 0) {
        return b;
    } else if (b == 0) {
        return a;
    } else {
        return get_gcd(b % a, a);
    }
}

__int128_t num_generator(__int128_t number, int c, int_fast64_t mod) {
    __int128_t new_number = number * number;
    new_number = (new_number - c) % mod;
    return new_number;
}


void get_multiplications(int_fast64_t number,
                         std::vector<int_fast64_t> *c,
                         std::priority_queue<int_fast64_t, std::vector<int_fast64_t>, std::greater<> > *multiplications) {
    int_fast64_t a;
    int_fast64_t b;
    int_fast64_t gcd;
    int_fast64_t diff;

    if (number < 10) {
        for (int i = 2; i < 4; i++) {
            while (number % i == 0) {
                multiplications->push(i);
                number = number / i;
            }
        }
        if (number != 1) {
            multiplications->push(number);
        }
        return;
    }

    for (int_fast64_t j:*c) {
        a = 2;
        b = 2;
        for (int i = 0; i < 2000000; i++) { // проверка с одним модулем
            a = num_generator(a, j, number);
            b = num_generator(num_generator(b, j, number), j, number);
            if (b > a) {
                diff = b - a;
            } else {
                diff = a - b;
            }

            gcd = get_gcd(diff, number);

            if ((gcd != number) & (gcd != 1)) {
                get_multiplications(gcd, c, multiplications);
                get_multiplications(number / gcd, c, multiplications);
                return;
            }
        }
    }
    multiplications->push(number);
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast64_t number;
    std::cin >> number;

    std::priority_queue<int_fast64_t, std::vector<int_fast64_t>, std::greater<> > multiplications;
    std::vector<int_fast64_t> c({1, 5});
    get_multiplications(number, &c, &multiplications);

    while (!multiplications.empty()) {
        std::cout << multiplications.top() << " ";
        multiplications.pop();
    }
    return 0;
}
