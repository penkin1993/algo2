#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>

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

int64_t get_gcd(int64_t a, int64_t b) {
    a = abs(a);
    b = abs(b);
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
    new_number = (new_number + c) % mod;
    return new_number;
}


void get_multiplications(int_fast64_t number,
                         std::unordered_set<int_fast64_t> *prime_num,
                         std::vector<int_fast64_t> *c,
                         std::vector<int_fast64_t> *multiplications) {
    if (prime_num->count(number)) { // совсем маленькие простые числа добавляем сразу
        multiplications->push_back(number);
        return;
    } // TODO: Этого не достаточно !!!!!





    int_fast64_t num_sqrt = floor(pow(number, 0.5));

    int_fast64_t a;
    int_fast64_t b;
    int_fast64_t gcd;

    for (int_fast64_t j:*c) {
        a = 3;
        b = 3;
        for (int i = 0; i < 100; i++) { // проверка с одним модулем
            a = num_generator(a, j, num_sqrt);
            b = num_generator(num_generator(b, j, num_sqrt), j, num_sqrt);

            if (b > a) {
                gcd = get_gcd(b - a, number);
                //std::cout << num_sqrt << " b - a " << b - a << " gcd " << gcd << "\n";
            } else {
                gcd = get_gcd(a - b, number);
            }

            if ((gcd != number) & (gcd != 1)) {
                std::cout << " gcd " << gcd << "\n";
                get_multiplications(gcd, prime_num, c, multiplications);
                get_multiplications(number / gcd, prime_num, c, multiplications);
                return;
            }
        }
    }
    multiplications->push_back(number);
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast64_t number;
    std::cin >> number;

    std::vector<int_fast64_t> multiplications;
    std::unordered_set<int_fast64_t> prime_num({2, 3, 5, 7, 11, 13, 17, 19});
    std::vector<int_fast64_t> c({1, 3, 7});
    get_multiplications(number, &prime_num, &c, &multiplications);

    for (int_fast64_t num:multiplications) {
        std::cout << num << " ";
    }
    return 0;
}