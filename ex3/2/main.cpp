#include <iostream>
#include <vector>
#include <deque>

void get_min_div(uint32_t number, std::deque<uint32_t> *lp) {
    std::vector<uint32_t> primes;
    for (uint32_t i = 2; i < number; i++) {
        if (lp->at(i) == 0) {
            lp->at(i) = i;
            primes.push_back(i);
        }
        for (uint32_t p : primes) {
            if (p > lp->at(i) || i * p >= number) {
                break;
            }
            lp->at(i * p) = p;
        }
    }
    lp->pop_front();
    lp->pop_front();
}

void get_statistics(uint32_t number, std::deque<uint32_t> *lp) {
    uint64_t d = 0;
    auto *num_factor_first = new uint32_t[(number + 1) * 8];
    auto *num_factor_second = new std::vector<uint8_t>[number + 1];

    auto *first_size = new uint32_t[(number + 1)];
    for (int32_t i = 0; i < (number + 1); i++) {
        first_size[i] = 0;
    }

    uint32_t s_0 = 1;
    uint64_t s_1 = 0;
    uint64_t phi = 1;

    uint32_t s_0_current;
    uint64_t s_1_current;
    uint64_t local_s_1;
    uint64_t phi_current;

    for (uint32_t num = 2; num < number + 1; num++) { // разложние по простым делителям
        /*
        for (int32_t i_ = 0; i_ < (number + 1) * 8; i_++ ){
            std::cout << num_factor_first[i_] << " ";
        } std::cout << "\n";
         */
        /*
        for (int32_t i_ = 0; i_ < number + 1; i_++ ){
            for (int32_t j_ = 0; j_ < num_factor_second[i_].size(); j_++){
                std::cout <<  num_factor_second[i_][j_] << " ";
            }
        } std::cout << "\n";
         */
        /*
        for (int32_t i = 0; i < number + 1; i++){
            std::cout << first_size[i] << " ";
        } std::cout << "\n";
         */

        uint32_t s = lp->front();
        lp->pop_front();
        d += s;

        //std::cout << " s " << s << "\n";

        // num_factor_first[num] = num_factor_first[num / s]; // TODO
        for (int32_t i = 0; i < 8; i++) {
            num_factor_first[i + 8 * num] = num_factor_first[i + 8 * (num / s)];
        }
        first_size[num] = first_size[num / s];


        num_factor_second[num] = num_factor_second[num / s];

        int k = 0;
        // std::cout << first_size[num] << " " << num <<  "\n";
        // TODO first_size[num] ??????

        for (; k < first_size[num]; k++) {
            // if (num_factor_first[num][k] == s) { // TODO
            // std::cout << " s " << s << "\n";


            if (num_factor_first[k + num * 8] == s) {
                // std::cout << "!!!\n";
                num_factor_second[num][k]++;
                k = -1;
                break;
            }
        }
        // std::cout << " k " << k << "\n"; // !!!  k - отличается !!!








        if (k != -1) {
            // num_factor_first[num].push_back(s); // TODO
            num_factor_first[first_size[num] + 8 * num] = s;

            //std::cout << first_size[num] << " ";
            first_size[num] += 1;
            //std::cout << first_size[num] << "\n";






            num_factor_second[num].push_back(1);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////
        s_0_current = 1;
        s_1_current = 1;
        phi_current = 1;

        /*
        for (int32_t i_ = 0; i_ < number + 1; i_++ ){
            for (int32_t j_ = 0; j_ < num_factor_second[i_].size(); j_++){
                std::cout <<  num_factor_second[i_][j_] << " ";
            }
        } std::cout << "\n";
        */



        // for (int i = 0; i < num_factor_first[num].size(); i++) { // подсчет всех статистик налету // TODO


        for (int i = 0; i < first_size[num]; i++) { // подсчет всех статистик налету
            //std::cout  << " num " << num << " i " << i << "\n";
            //std::cout << num_factor_second[num][i] + 1 << "\n";
            // std::cout << " first_size[num] " << first_size[num] << "\n";
            s_0_current *= (num_factor_second[num][i] + 1);

            local_s_1 = 1;
            int mult_s_1 = 1;
            for (uint64_t j = 1; j < num_factor_second[num][i] + 1; j++) {
                // std::cout << " j " << j << "\n";
                // mult_s_1 *= num_factor_first[num][i]; // TODO
                mult_s_1 *= num_factor_first[i + 8 * num];
                local_s_1 += mult_s_1;
            }
            s_1_current *= local_s_1;
            //  phi_current *= mult_s_1 - mult_s_1 / num_factor_first[num][i]; // TODO
            phi_current *= mult_s_1 - mult_s_1 / num_factor_first[i + 8 * num];
        }
        //std::cout << s_0_current << "\n";
        s_0 += s_0_current;
        s_1 += s_1_current;
        phi += phi_current;

        // std::cout << " num " << num << " s_0 " << s_0 << " s_1 " << s_1 << " phi " << phi << "\n";
    }

    std::cout << d << " " << s_0 << " " << s_1 + 1 << " " << phi;

    delete[] num_factor_first;
    delete[] num_factor_second;
    delete[] first_size;

}

/*
10

28 27 87 32


*/
int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int number;
    std::cin >> number;
    std::deque<uint32_t> lp(number + 1, 0);
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