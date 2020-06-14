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

void get_result(std::string num1, std::string num2) {

    if (num1 == "0" || (num2 == "0")){
        std::cout << 0;
        return;
    }

    if ((num1[0] == '-') && (num2[0] == '-')){
        return get_result(num1.substr(1, num1.size() - 1), num2.substr(1, num2.size() - 1));
    } else if  ((num1[0] == '-') && (num2[0] != '-')) {
        std::cout << "-";
        return get_result(num1.substr(1, num1.size() - 1), num2);
    } else if  ((num1[0] != '-') && (num2[0] == '-')) {
        std::cout << "-";
        return get_result(num1, num2.substr(1, num2.size() - 1));
    }

    int_fast32_t n = 2;

    int_fast32_t fill1 = (n - num1.length() % n) % n;
    int_fast32_t fill2 = (n - num2.length() % n) % n; // добавить в начало fill2 нулей

    num1 = std::string(fill1, '0') + num1;
    num2 = std::string(fill2, '0') + num2;


    std::vector<int_fast32_t> num1_array;
    std::vector<int_fast32_t> num2_array;

    for (int_fast32_t i = num1.size() / n - 1; i >= 0; i--) {
        num1_array.push_back(std::stoi(num1.substr(n * i, n)));
    }

    for (int_fast32_t i = num2.size() / n - 1; i >= 0; i--) {
        num2_array.push_back(std::stoi(num2.substr(n * i, n)));
    }

    std::vector<int_fast32_t> res_array = multiply(num1_array, num2_array);

    int_fast32_t dev = std::pow(10, n);
    std::vector<int_fast32_t> ans(res_array.size() + 1, 0);

    int_fast32_t next_rest = 0;

    for (int_fast32_t i = 0; i < res_array.size(); i++) {
        int_fast32_t rest = res_array[i] % dev;
        rest += next_rest;

        next_rest = res_array[i] / dev;

        if (rest < dev){
            ans[i] += rest;
        } else{
            next_rest += rest / dev;
            ans[i] += rest % dev;
        }
    }

    if (next_rest != 0){
        ans[ans.size() - 1] = next_rest;
    }

    std::string res_string;

    for (int_fast32_t i = ans.size() - 1; i >= 0; i--){
        std::string ans_p = std::to_string(ans[i]);

        res_string += std::string(n - ans_p.size(), '0') + ans_p;
    }

    for (int_fast32_t i = 0; i < res_string.size(); i++){
        if (res_string[i] != '0'){
            std::cout << res_string.substr(i, res_string.size() - i);
            return;
        }
    }

}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    std::string num1;
    std::string num2;

    std::cin >> num1;
    std::cin >> num2;

    get_result(num1, num2);

    return 0;
}