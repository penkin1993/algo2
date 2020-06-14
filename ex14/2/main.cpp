#include <bits/stdc++.h>
#include <iostream>
#include <queue>
#include <cmath>

using cd = std::complex<double>;
const double PI = acos(-1);

void fft(std::vector<cd> &a, bool invert) {
    int_fast32_t n = a.size();
    if (n == 1)
        return;

    std::vector<cd> a0(n / 2), a1(n / 2);
    for (int_fast32_t i = 0; 2 * i < n; i++) {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }
    fft(a0, invert);
    fft(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    cd w(1), wn(cos(ang), sin(ang));
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

std::vector<int_fast32_t> multiply(std::vector<int_fast32_t> const &a, std::vector<int_fast32_t> const &b) {
    std::vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
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

void get_result(std::string & input_str) {

    std::vector<int_fast32_t> place_array (input_str.size(), 1);
    for (int_fast32_t i = 0; i < place_array.size(); i++){
        place_array[i] = input_str[i] == '1';
    }
    std::vector<int_fast32_t> result = multiply(place_array, place_array);

    int_fast64_t pos_counter = 0;

    for (int_fast32_t i = 0; i < place_array.size(); i++){
        if (place_array[i]){
            pos_counter += (result[2*i] - 1) / 2;
        }
    }

    std::cout << pos_counter << "\n";
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    std::string input_str;

    std::cin >> input_str;

    get_result(input_str);

    return 0;
}