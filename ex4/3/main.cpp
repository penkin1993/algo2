#include <iostream>
#include <vector>
#include <deque>
#include <cmath>
#include <unordered_map>

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

std::pair<int64_t, int64_t> get_gcd_x_y(int64_t a, int64_t b, int64_t d) {
    int64_t sign_a = (a >= 0) ? 1 : -1;
    int64_t sign_b = (b >= 0) ? 1 : -1;
    a = abs(a);
    b = abs(b);
    int64_t c = -1;
    if (b > a) {
        c = b;
        b = a;
        a = c;
    };

    std::pair<int64_t, int64_t> x_y;
    if (b == 0) {
        if (c == -1) {
            return {sign_a, 0};
        } else {
            return {0, sign_b};
        }
    } else {
        x_y = get_gcd_x_y(b, a % b, d);
        int64_t x = x_y.second;
        int64_t y = x_y.first - x_y.second * (a / b);
        if (c == -1) {
            return {sign_a * x, sign_b * y};
        } else {
            return {sign_a * y, sign_b * x};
        }
    }
}


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

int_fast64_t get_log(int_fast64_t a, int_fast64_t b, int_fast64_t n) {
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

//////////////////////////////////////////////////////////////////////////////////////////////

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

int_fast64_t get_ans(int_fast64_t a, int_fast64_t b, int_fast64_t m) {
    std::vector<int_fast64_t> prime_div;
    get_prime_div(a - 1, prime_div);
    int_fast64_t g = get_g(a, prime_div);
    if (g == -1){
        return -1;
    } else{
        int_fast64_t s = get_log(g, a, m);
        if (s == -1){
            return -1;
        } else{
            int64_t gcd;
            std::pair<int64_t, int64_t> x_y;
            int64_t x;
            gcd = get_gcd(b, 1 - m);
            if (s % gcd == 0) {
                x_y = get_gcd_x_y(b, 1 - m, gcd);
                x = x_y.first * (-s / gcd);
                return x;
            } else {
                return -1;
            }

        }
    }
}
int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    int_fast64_t a, b, m;
    std::cin >> a >> b >> m;

    int_fast64_t ans = get_ans(a, b, m);
    std::cout << ans;

    return 0;
}

//2
//4 3 5 // 4
//1 3 7 // 1
