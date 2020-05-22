#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

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


    // + цифровая сортировка !!!!












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
