/*
C. ДНК Роботов
ограничение по времени на тест2 секунды
ограничение по памяти на тест256 мегабайт
вводстандартный ввод
выводстандартный вывод
Последние достижения в технологии синтеза ДНК позволили провести эксперимент по созданию биороботов.

Для облегчения задачи создания ПО для управления роботами было принято решение, что их ДНК будет состоять из M=2n символов для некоторого n≥2. Кроме этого, по техническим причинам это будет не обычная строка, а циклическая, то есть её можно начинать читать с любой позиции.

Одной из целей эксперимента является изучение мутаций биороботов. В результате продолжительных наблюдений было найдено много различных видов роботов. Для понимания процесса мутации учёным необходимо решить следующую задачу. Для ДНК двух роботов требуется определить коэффициент их похожести. Он вычисляется, как максимальное количество совпадающих символов при наилучшем совмещении этих ДНК. Чем больше символов совпадает, тем лучше совмещение.

Требуется написать программу, которая найдёт наилучшее совмещение двух ДНК.

Входные данные
В первой строке входного файла записано одно число M(4≤M≤131072). В следующих двух строках записаны ДНК двух роботов. Обе ДНК — строки, состоящие ровно из M символов из множества {'A', 'C', 'G', 'T'}.

Выходные данные
В выходной файл выведите два числа — максимальное количество совпадающих символов и значение оптимального сдвига — неотрицательное количество символов второй ДНК, которые необходимо перенести из конца строки в её начало для достижения наилучшего совмещения.
*/

#include <bits/stdc++.h>
#include <iostream>

void fft(std::vector<std::complex<double>> &a, bool invert) {
    int_fast32_t n = a.size();
    if (n == 1) {
        return;
    }

    std::vector<std::complex<double>> a0(n / 2), a1(n / 2);
    for (int_fast32_t i = 0; 2 * i < n; i++) {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }
    fft(a0, invert);
    fft(a1, invert);

    double phi;

    if (invert) {
        phi = -2 * acos(-1) / n;
    } else {
        phi = 2 * acos(-1) / n;
    }

    std::complex<double> w(1), wn(cos(phi), sin(phi));
    for (int_fast32_t i = 0; 2 * i < n; i++) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        if (invert) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }
        w *= wn;
    }
}

std::vector<int_fast32_t> prod(std::vector<int_fast32_t> const &a, std::vector<int_fast32_t> const &b) {
    std::vector<std::complex<double>> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int_fast32_t n = 1;

    while (n < a.size() + b.size()) {
        n <<= 1;
    }
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);

    for (int_fast32_t i = 0; i < n; i++)
        fa[i] *= fb[i];

    fft(fa, true);

    std::vector<int_fast32_t> result(n);
    for (int_fast32_t i = 0; i < n; i++)
        result[i] = round(fa[i].real());

    return result;
}


std::vector<int_fast32_t>
get_result_by_symbol(std::string const &input_str1, std::string const &input_str2, char const symbol) {

    std::vector<int_fast32_t> array1;
    std::vector<int_fast32_t> array2;

    for (char i : input_str1) {
        array1.push_back(i == symbol);
    }

    array2.push_back(input_str2[0] == symbol);
    for (int_fast32_t i = input_str2.size() - 1; i > 0; i--) {
        array2.push_back(input_str2[i] == symbol);
    }
    return prod(array1, array2);
}

void get_result(std::string const &input_str1, std::string const &input_str2, int_fast32_t m) {

    std::vector<int_fast32_t> res_a = get_result_by_symbol(input_str1, input_str2, 'A');
    std::vector<int_fast32_t> res_c = get_result_by_symbol(input_str1, input_str2, 'C');
    std::vector<int_fast32_t> res_g = get_result_by_symbol(input_str1, input_str2, 'G');
    std::vector<int_fast32_t> res_t = get_result_by_symbol(input_str1, input_str2, 'T');

    std::vector<int_fast32_t> res(m, 0);

    for (int_fast32_t i = 0; i < res_a.size(); i++) {
        res[i % m] += res_a[i] + res_c[i] + res_g[i] + res_t[i];
    }

    int_fast32_t ind_max = 0;
    int_fast32_t val_max = 0;

    for (int_fast32_t i = 0; i < res.size(); i++) {

        if (val_max < res[i]) {
            val_max = res[i];
            ind_max = i;
        }
    }
    std::cout << val_max << " " << ind_max;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int_fast32_t m;

    std::cin >> m;
    std::string input_str1, input_str2;
    std::cin >> input_str1 >> input_str2;

    get_result(input_str1, input_str2, m);

    return 0;
}
