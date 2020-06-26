#include <iostream>
#include <vector>
#include <deque>

int_fast32_t INF = 1e7;

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
