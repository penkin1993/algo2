#include <iostream>
#include <vector>

void get_min_div(int_fast64_t number, std::vector<int_fast64_t> *primes, std::vector<int_fast64_t> *lp) {
    for (int_fast64_t i = 2; i < number; i++) {
        if (lp->at(i) == 0) {
            lp->at(i) = i;
            primes->push_back(i);
        }
        for (int_fast64_t p : *primes) {
            if (p > lp->at(i) || i * p >= number) {
                break;
            }
            lp->at(i * p) = p;
        }
    }
}


void get_statistics(int_fast64_t number) {
}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int number;
    std::cin >> number;
    std::vector<int_fast64_t> primes;
    std::vector<int_fast64_t> lp(number + 1, 0);
    get_min_div(number + 1, &primes, &lp);

    //for (int i: primes){
    //    std::cout << i << " ";
    //}
    //for (int i: lp){
    //    std::cout << i << " ";
    //}

    //print_statistics(number, &min_del);

    return 0;
}
