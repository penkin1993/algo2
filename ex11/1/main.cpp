#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>

inline int_fast32_t positive_mod(int_fast32_t i, int_fast32_t n) {
    return (i % n + n) % n;
}

void radix_sort(std::vector<std::tuple<char, int_fast32_t, int_fast32_t>> * char_array,
                std::vector<int_fast32_t> * pos, int_fast32_t k){

    // получить модуль сдвига
    int_fast32_t mod =  -(1 << k);
    //std::cout << mod;

    std::vector<int_fast32_t> first_c;
    // сформировать первый разряд
    for (int_fast32_t i = 0; i < char_array->size(); i++){
        int_fast32_t prev_ind = positive_mod(std::get<1>(char_array->at(i)) + mod, char_array->size());
        first_c.push_back(std::get<2>(char_array->at(pos->at(prev_ind))));
    }

    // for (int_fast32_t i = 0; i < first_c.size(); i++){
    //   std::cout << first_c[i] << " " << std::get<2>(char_array->at(i)) << "\n";
    //}

    //отсортировать его


    // сформировать новый c

    // + сформировать новый pos
    

    // выход из него если все классы различыне
}

void get_sort_suffix(std::string input_s) {
    std::vector<std::tuple<char, int_fast32_t, int_fast32_t>> char_array;
    input_s += '$'; // определили исходный массив порядков

    std::vector<int_fast32_t> pos(input_s.size(), 0);
    // + определить ссылки на элементы char_array. Чтобы по символу в строке input_s находить его tuple

    // определили массив p и c
    for (int_fast32_t i = 0; i < input_s.size(); i++) {
        char_array.emplace_back(input_s[i], i, 0);
    }
    std::sort(char_array.begin(), char_array.end());

    for (int_fast32_t i = 0; i < char_array.size(); i++) {
        pos[std::get<1>(char_array[i])] = i;
    }

    // задание классов классов
    int_fast32_t counter = 0;

    for (int_fast32_t i = 1; i < char_array.size(); i++) {
        if (std::get<0>(char_array[i]) != std::get<0>(char_array[i - 1])) {
            counter++;
        }
        std::get<2>(char_array[i]) = counter;
    }

    // цифровая сортировка
    radix_sort(& char_array, & pos, 0);


    for (int_fast32_t i = 0; i < char_array.size(); i++) {
        std::cout << std::get<0>(char_array[i]) << " " << std::get<1>(char_array[i]) << " "
                  << std::get<2>(char_array[i]) << " "<< pos[i] << "\n";
    }

}


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    //std::string input_s;
    //std::cin >> input_s;
    //get_sort_suffix(input_s);
    get_sort_suffix("abacaba");

    // std::cout << input_s << "\n";

    return 0;
}
