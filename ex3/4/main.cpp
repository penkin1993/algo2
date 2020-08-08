/*
D. Прямая
ограничение по времени на тест1 second
ограничение по памяти на тест256 megabytes
вводстандартный ввод
выводстандартный вывод
Своим уравнением Ax + By + C = 0 задана прямая на плоскости. Требуется найти любую принадлежащую этой прямой точку, координаты которой — целые числа от  - 5·1018 до 5·1018 включительно, или выяснить что таких точек нет.

Входные данные
В первой строке содержатся три целых числа A, B и C ( - 2·109 ≤ A, B, C ≤ 2·109) — соответствующие коэффициенты уравнения прямой. Гарантируется, что A 2 + B 2 > 0.

Выходные данные
Если искомая точка существует, выведите ее координаты, иначе выведите единственное число -1.
*/

#include <iostream>
#include <vector>

int64_t get_gcd(int64_t a, int64_t b) {
    a = abs(a);
    b = abs(b);
    if (b < a) {
        int64_t c;
        c = b;
        b = a;
        a = c;
    };
    if (a == 0) {
        return b;
    } else if (b == 0) {
        return a;
    } else {
        return get_gcd(b % a, a);
    }
}

std::pair<int64_t, int64_t> get_gcd_x_y(int64_t a, int64_t b, int64_t d) {
    int64_t sign_a = (a >= 0) ? 1 : -1;
    int64_t sign_b = (b >= 0) ? 1 : -1;
    a = abs(a);
    b = abs(b);
    int64_t c = -1;
    if (b > a) {
        c = b;
        b = a;
        a = c;
    };

    std::pair<int64_t, int64_t> x_y;
    if (b == 0) {
        if (c == -1) {
            return {sign_a, 0};
        } else {
            return {0, sign_b};
        }
    } else {
        x_y = get_gcd_x_y(b, a % b, d);
        int64_t x = x_y.second;
        int64_t y = x_y.first - x_y.second * (a / b);
        if (c == -1) {
            return {sign_a * x, sign_b * y};
        } else {
            return {sign_a * y, sign_b * x};
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int64_t gcd;
    std::pair<int64_t, int64_t> x_y;
    int64_t a, b, d, x, y;
    std::cin >> a >> b >> d;
    d *= -1;

    gcd = get_gcd(a, b);
    if (d % gcd == 0) {
        x_y = get_gcd_x_y(a, b, gcd);
        x = x_y.first * (d / gcd);
        y = x_y.second * (d / gcd);
        std::cout << x << " " << y;

    } else {
        std::cout << -1;
    }
    return 0;
}
