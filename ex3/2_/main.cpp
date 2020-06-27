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
    auto *num_factor_first = new std::vector<uint32_t>[number + 1];
    auto *num_factor_second = new std::vector<uint32_t>[number + 1];

    uint32_t s_0 = 1;
    uint64_t s_1 = 0;
    uint64_t phi = 1;

    uint32_t s_0_current;
    uint64_t s_1_current;
    uint64_t local_s_1;
    uint64_t phi_current;

    for (uint32_t num = 2; num < number + 1; num++) { // разложние по простым делителям
        /*
        for (int32_t i_ = 0; i_ < number + 1; i_++ ){
            for (int32_t j_ = 0; j_ < num_factor_first[i_].size(); j_++){
                std::cout <<  num_factor_first[i_][j_] << " ";
            }
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
        for (int32_t i = 0; i < number + 1; i++ ){
            std::cout  << num_factor_first[i].size()<< " ";
        } std::cout << "\n";
         */

        // std::cout  << " num " << num << "\n";
        uint32_t s = lp->front();
        lp->pop_front();
        d += s;
        // std::cout << " s " << s << "\n";

        num_factor_first[num] = num_factor_first[num / s];
        num_factor_second[num] = num_factor_second[num / s];

        int k = 0;
        // std::cout << num_factor_first[num].size()  << " " << num <<  "\n";

        for (; k < num_factor_first[num].size(); k++) {
            // std::cout << " s " << s << "\n";
            if (num_factor_first[num][k] == s) {
                // std::cout << "!!!\n";
                num_factor_second[num][k]++;
                k = -1;
                break;
            }
        }
        // std::cout << " k " << k << "\n";











        if (k != -1) {
            num_factor_first[num].push_back(s);
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






        for (int i = 0; i < num_factor_first[num].size(); i++) { // подсчет всех статистик налету
            //std::cout  << " num " << num << " i " << i << "\n";
            //std::cout << num_factor_second[num][i] + 1 << "\n";
            // std::cout << " num_factor_first[num].size() " << num_factor_first[num].size() << "\n";
            s_0_current *= (num_factor_second[num][i] + 1);

            local_s_1 = 1;
            int mult_s_1 = 1;
            for (uint64_t j = 1; j < num_factor_second[num][i] + 1; j++) {
                // std::cout  << " j " << j << "\n";
                mult_s_1 *= num_factor_first[num][i];
                local_s_1 += mult_s_1;
            }
            s_1_current *= local_s_1;
            phi_current *= mult_s_1 - mult_s_1 / num_factor_first[num][i];
        }

        // std::cout << s_0_current << "\n";
        s_0 += s_0_current;
        s_1 += s_1_current;
        phi += phi_current;

        // std::cout << " num " << num << " s_0 " << s_0 << " s_1 " << s_1 << " phi " << phi << "\n";
    }

    std::cout << d << " " << s_0 << " " << s_1 + 1 << " " << phi;

    delete[] num_factor_first;
    delete[] num_factor_second;

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