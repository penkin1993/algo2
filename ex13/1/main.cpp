#include <iostream>
#include <cmath>
#include <iomanip>

struct Vector {
    long double x;
    long double y;

public:
    Vector(long double x_, long double y_);

    void normalize();

    long double length();
};

Vector::Vector(long double x_, long double y_) {
    x = x_;
    y = y_;
}

long double Vector::length() {
    return std::sqrt(x * x + y * y);
}

void Vector::normalize() {
    long double length = this->length();
    x = x / length;
    y = y / length;
}

void get_ans(long double x1, long double y1, long double r1,
             long double x2, long double y2, long double r2) {

    if ((x1 == x2) && (y1 == y2) && (r1 == r2)) { // бесконечно много точек
        std::cout << "3\n";
        return;
    }

    long double dist_o = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    long double sum_rad = r1 + r2;

    if ((dist_o > sum_rad) || (dist_o < r1 - r2)) { // окружности не пересекаются
        std::cout << "0\n";

    } else if ((dist_o == sum_rad) || (dist_o == r1 - r2)) {
        std::cout << "1\n";
        Vector vec = Vector(x2 - x1, y2 - y1);
        vec.normalize();
        Vector point_h = Vector(x1 + r1 * vec.x, y1 + r1 * vec.y);
        std::cout << point_h.x << " " << point_h.y << "\n";

    } else if ((dist_o < sum_rad) || (dist_o > r1 - r2)) {
        std::cout << "2\n";
        Vector vec = Vector(x2 - x1, y2 - y1);
        vec.normalize();

        long double dist_h = (r1 * r1 - r2 * r2 + dist_o * dist_o) / 2 / dist_o;

        Vector vec_h = Vector(dist_h * vec.x, dist_h * vec.y);
        std::cout << x1 + vec_h.x << " " << y1 + vec_h.y << "\n";

        long double len_vec_h = vec_h.length();
        std::cout << len_vec_h << " ";

        long double len_hp = std::sqrt(r1 * r1 - len_vec_h * len_vec_h);
        std::cout << len_hp << "\n";

        Vector vec_p1 = Vector(-vec_h.y, vec_h.x); // перпендикуляры
        Vector vec_p2 = Vector(vec_h.y, - vec_h.x);

        vec_p1.normalize(); // нормализовали в единичные векторы
        vec_p2.normalize();

        vec_p1.x = len_hp * vec_p1.x;
        vec_p1.y = len_hp * vec_p1.y;

        vec_p2.x = len_hp * vec_p2.x;
        vec_p2.y = len_hp * vec_p2.y;

        std::cout << x1 + vec_h.x + vec_p1.x << " " << y1 + vec_h.y +  vec_p1.y << "\n";
        std::cout << x1 + vec_h.x + vec_p2.x << " " << y1 + +vec_h.y + vec_p2.y << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::cout << std::fixed;
    std::cout << std::setprecision(10);


    int_fast32_t n;
    std::cin >> n;

    long double x1, y1, r1, x2, y2, r2;
    for (int_fast32_t i = 0; i < n; i++) {
        std::cin >> x1 >> y1 >> r1;
        std::cin >> x2 >> y2 >> r2;

        get_ans(x1, y1, r1, x2, y2, r2);
    }

    return 0;
}

/*
4
3 4 5
11 4 2
3 4 5
11 4 3
3 4 5
11 4 4
3 4 5
3 4 5


0
1
8.0000000000 4.0000000000
2
7.5625000000 4.0000000000
4.5625000000 2.0453835215
7.5625000000 6.0453835215
7.5625000000 1.9546164785
3

*/