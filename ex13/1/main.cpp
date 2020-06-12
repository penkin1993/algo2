#include <iostream>
#include <cmath>
#include <iomanip>

double EPS = 1e-12;

struct Vector {
    double x;
    double y;

public:
    Vector(double x_, double y_);

    void normalize();

    void mult(double coeff);

    double length() const;
};

Vector::Vector(double x_, double y_) {
    x = x_;
    y = y_;
}

double Vector::length() const {
    return std::sqrt(x * x + y * y);
}

void Vector::normalize() {
    double length = this->length();
    x = x / length;
    y = y / length;
}

void Vector::mult(double coeff) {
    x = x * coeff;
    y = y * coeff;

}

void get_ans(double x1, double y1, double r1,
             double x2, double y2, double r2) {

    if ((std::fabs(x1 - x2) < EPS) && (std::fabs(y1 - y2) < EPS) &&
        (std::fabs(r1 - r2) < EPS)) { // бесконечно много точек
        std::cout << "3\n";
        return;
    }

    double dist_o = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    double sum_rad = r1 + r2;
    double diff_rad = r1 - r2;

    if ((dist_o > sum_rad) || (dist_o < std::fabs(diff_rad))) { // окружности не пересекаются
        std::cout << "0\n";

    } else if ((std::fabs(dist_o - sum_rad) <= EPS) || (std::fabs(dist_o - std::fabs(diff_rad)) <= EPS)) {
        std::cout << "1\n";
        Vector vec = Vector(x2 - x1, y2 - y1);
        vec.normalize();
        if ((diff_rad < 0) && (dist_o < r2)) {
            vec.mult(-1);
        }
        Vector point_h = Vector(x1 + r1 * vec.x, y1 + r1 * vec.y);
        std::cout << point_h.x << " " << point_h.y << "\n";

    } else {
        std::cout << "2\n";

        Vector vec = Vector(x2 - x1, y2 - y1);
        vec.normalize();

        double dist_h = (r1 * r1 / dist_o - r2 * r2 / dist_o + dist_o) / 2;

        Vector vec_h = Vector(dist_h * vec.x, dist_h * vec.y);
        std::cout << x1 + vec_h.x << " " << y1 + vec_h.y << "\n";

        double len_vec_h = vec_h.length();
        std::cout << len_vec_h << " ";

        double len_hp = std::sqrt(r1 * r1 - len_vec_h * len_vec_h);
        std::cout << len_hp << "\n";

        vec = Vector(-2*(x2 - x1), -2*(y2 - y1));

        double c = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + r1 * r1 - r2 * r2;

        double x0 = -vec.x * c / (vec.length() * vec.length());
        double y0 = -vec.y * c / (vec.length() * vec.length());

        double d = r1 * r1 - c * c / (vec.length() * vec.length());
        double coeff = sqrt(d / (vec.length() * vec.length()));

        std::cout << x0 + vec.y * coeff + x1 << " " << y0 - vec.x * coeff + y1 << "\n";
        std::cout << x0 - vec.y * coeff + x1 << " " << y0 + vec.x * coeff + y1 << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::cout << std::fixed;
    std::cout << std::setprecision(10);

    int_fast32_t n;
    std::cin >> n;

    double x1, y1, r1, x2, y2, r2;

    for (int_fast32_t i = 0; i < n; i++) {
        std::cin >> x1 >> y1 >> r1;
        std::cin >> x2 >> y2 >> r2;
        get_ans(x1, y1, r1, x2, y2, r2);
    }

    return 0;
}
