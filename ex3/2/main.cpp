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

    auto *first_size = new uint8_t[(number + 1)];
    for (int32_t i = 0; i < (number + 1); i++) {
        first_size[i] = 0;
    }

    auto *num_factor_first = new uint32_t[(number + 1) * 7];
    auto *num_factor_second = new uint8_t[(number + 1) * 7];

    uint32_t s_0 = 1;
    uint64_t s_1 = 0;
    uint64_t phi = 1;

    uint32_t s_0_current;
    uint64_t s_1_current;
    uint64_t local_s_1;
    uint64_t phi_current;

    for (uint32_t num = 2; num < number + 1; num++) {
        uint32_t s = lp->front();
        lp->pop_front();
        d += s;
        for (int32_t i = 0; i < 7; i++) {
            num_factor_first[i + 7 * num] = num_factor_first[i + 7 * (num / s)];
        }
        for (int32_t i = 0; i < 7; i++) {
            num_factor_second[i + 7 * num] = num_factor_second[i + 7 * (num / s)];
        }

        first_size[num] = first_size[num / s];
        int k = 0;

        for (; k < first_size[num]; k++) {
            if (num_factor_first[k + num * 7] == s) {
                num_factor_second[k + num * 7]++;
                k = -1;
                break;
            }
        }
        if (k != -1) {
            num_factor_first[first_size[num] + 7 * num] = s;
            num_factor_second[first_size[num] + 7 * num] = 1;
            first_size[num] += 1;
        }

        s_0_current = 1;
        s_1_current = 1;
        phi_current = 1;


        for (int i = 0; i < first_size[num]; i++) {
            s_0_current *= (num_factor_second[i + 7 * num] + 1);

            local_s_1 = 1;
            int mult_s_1 = 1;
            for (uint64_t j = 1; j < num_factor_second[i + 7 * num] + 1; j++) {
                mult_s_1 *= num_factor_first[i + 7 * num];
                local_s_1 += mult_s_1;
            }
            s_1_current *= local_s_1;
            phi_current *= mult_s_1 - mult_s_1 / num_factor_first[i + 7 * num];
        }
        s_0 += s_0_current;
        s_1 += s_1_current;
        phi += phi_current;
    }

    std::cout << d << " " << s_0 << " " << s_1 + 1 << " " << phi;

    delete[] num_factor_first;
    delete[] num_factor_second;
    delete[] first_size;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int number;
    std::cin >> number;
    std::deque<uint32_t> lp(number + 1, 0);
    get_min_div(number + 1, &lp);
    get_statistics(number, &lp);
    return 0;
}