/*
A. Массовое разложение на множители
ограничение по времени на тест1.0 с
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Дано много чисел. Требуется разложить их все на простые множители.

Входные данные
В первой строке задано число n (2 ≤ n ≤ 300000). В следующих n строках заданы числа a i (2 ≤ a i ≤ 106), которые нужно разложить на множители.

Выходные данные
Для каждого числа выведите в отдельной строке разложение на простые множители в порядке возрастания множителей.
*/

#include <iostream>
#include <vector>

void print_multiplications(int number, std::vector<int> *min_del) {
    while (number != 1) {
        std::cout << min_del->at(number - 2) << " ";
        number = number / min_del->at(number - 2);
    }
    std::cout << "\n";
}

void get_min_del(std::vector<int> *min_del) {
    for (int i = 2; i < min_del->size(); i++) {
        if (min_del->at(i - 2) == 0) {
            for (int j = i; j < min_del->size(); j += i) {
                if (min_del->at(j - 2) == 0) {
                    min_del->at(j - 2) = i;
                }
            }
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int n, number;
    std::cin >> n;
    std::vector<int> min_del(1000001, 0);
    std::vector<int> prime_numbers;
    get_min_del(&min_del);

    for (int i = 0; i < n; ++i) {
        std::cin >> number;
        print_multiplications(number, &min_del);
    }
    return 0;
}
