#include <iostream>
#include <vector>
#include <deque>

void get_min_div(int_fast32_t number, std::deque<int_fast32_t> *lp) {
    std::vector<int_fast32_t> primes;
    for (int_fast32_t i = 2; i < number; i++) {
        if (lp->at(i) == 0) {
            lp->at(i) = i;
            primes.push_back(i);
        }
        for (int_fast32_t p : primes) {
            if (p > lp->at(i) || i * p >= number) {
                break;
            }
            lp->at(i * p) = p;
        }
    }
    lp->pop_front();
    lp->pop_front();
}

void get_statistics(int_fast32_t number, std::deque<int_fast32_t> *lp) {
    int_fast64_t d = 0;
    auto *num_factor_first = new std::vector<int_fast32_t>[number + 1];
    auto *num_factor_second = new std::vector<int_fast8_t>[number + 1];

    int_fast32_t s_0 = 1;
    int_fast64_t s_1 = 0;
    int_fast64_t phi = 1;

    int_fast32_t s_0_current;
    int_fast64_t s_1_current;
    int_fast64_t local_s_1;
    int_fast64_t phi_current;

    for (int_fast32_t num = 2; num < number + 1; num++) { // разложние по простым делителям
        int_fast32_t s = lp->front();
        lp->pop_front();
        d += s;

        num_factor_first[num] = num_factor_first[num / s];
        num_factor_second[num] = num_factor_second[num / s];

        int k = 0;
        for (; k < num_factor_first[num].size(); k++) {
            if (num_factor_first[num][k] == s) {
                num_factor_second[num][k]++;
                k = -1;
                break;
            }
        }

        if (k != -1) {
            num_factor_first[num].push_back(s);
            num_factor_second[num].push_back(1);
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////
        s_0_current = 1;
        s_1_current = 1;
        phi_current = 1;

        for (int i = 0; i < num_factor_first[num].size(); i++) { // подсчет всех статистик налету
            s_0_current *= (num_factor_second[num][i] + 1);

            local_s_1 = 1;
            int mult_s_1 = 1;
            for (int_fast64_t j = 1; j < num_factor_second[num][i] + 1; j++) {
                mult_s_1 *= num_factor_first[num][i];
                local_s_1 += mult_s_1;
            }
            s_1_current *= local_s_1;
            phi_current *= mult_s_1 - mult_s_1 / num_factor_first[num][i];
        }

        s_0 += s_0_current;
        s_1 += s_1_current;
        phi += phi_current;
    }

    std::cout << d << " " << s_0 << " " << s_1 + 1 << " " << phi;

    delete[] num_factor_first;
    delete[] num_factor_second;

}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int number;
    std::cin >> number;
    std::deque<int_fast32_t> lp(number + 1, 0);
    get_min_div(number + 1, &lp);
    get_statistics(number, &lp);
    return 0;
}

/*
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <algorithm>


int main() {
    uint32_t n;
    std::cin >> n;

    std::vector<uint32_t> lp(n + 1, 0);
    std::vector<uint32_t> primes;

    uint64_t sd = 0;
    for (uint32_t i = 2; i <= n; i++) {
        if (lp[i] == 0) {
            sd += (lp[i] = i);

            primes.push_back(i);
        }
        for (uint64_t p:primes) {
            if ((p > lp[i]) || (i * p > n)) {
                break;
            }
            sd += (lp[i * p] = p);
        }
    }

    uint64_t ss0 = 1;
    uint64_t ss1 = 1;
    uint64_t sphi = 1;

    uint32_t *divisors = new uint32_t[(n + 1) * 8];
    uint8_t *powers = new uint8_t[(n + 1) * 8]{0};
    uint8_t *sizes = new uint8_t[n + 1]{0};

    for (uint32_t i = 2; i <= n; i++) {
        uint32_t x = i, d = lp[x], prev_d = d;
        if (d == x) {
            divisors[(i << 3)] = d;
            powers[(i << 3)] = 1;
            sizes[i] = 1;
        } else {
            memcpy(&divisors[i << 3], &divisors[i / d << 3], sizeof(uint32_t) * sizes[i / d]);
            memcpy(&powers[i << 3], &powers[i / d << 3], sizeof(uint8_t) * sizes[i / d]);

            uint8_t p = 0;
            for (; p < sizes[i / d]; p++) {
                if (divisors[(i << 3) + p] == d) {
                    break;
                }
            }
            divisors[(i << 3) + p] = d;
            powers[(i << 3) + p]++;
            sizes[i] = std::max(uint8_t(p + 1), sizes[i / d]);
        }

        uint32_t s0 = 1;
        uint32_t s1 = 1;
        uint32_t phi = 1;
        for (int n = 0; n < sizes[i]; n++) {
            s0 *= (powers[(i << 3) + n] + 1);

            uint64_t pp = pow(divisors[(i << 3) + n], powers[(i << 3) + n] - 1);
            s1 *= (pp * divisors[(i << 3) + n] * divisors[(i << 3) + n] - 1) / (divisors[(i << 3) + n] - 1);
            phi *= pp * (divisors[(i << 3) + n] - 1);

        }
        ss0 += s0;
        ss1 += s1;
        sphi += phi;
    }

    std::cout << sd << ' ' << ss0 << ' ' << ss1 << ' ' << sphi;

    delete[]sizes;
    delete[]powers;
    delete[]divisors;

    exit(0);
}
*/