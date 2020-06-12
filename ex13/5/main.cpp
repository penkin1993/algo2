#include <iostream>
#include <queue>
#include <utility>
#include <cmath>

struct Point {
    long double x, y;

    explicit Point(long double x_ = 0, long double y_ = 0) : x(x_), y(y_) {}

};

struct Line {
    long double a{}, b{}, c{};

    explicit Line(long double a_ = 0, long double b_ = 0, long double c_ = 0) : a(a_), b(b_), c(c_) {}
};

class Circle{
    std::vector<Point> points;
    double rad = 0;

    Point center;
    explicit Circle(int_fast32_t x_, int_fast32_t y_) : center(Point(x_, y_)) {
        points.push_back(center);
    }

    // метод точки в круге

    // круг по двум точкам

    // круг по 3 точкам

    // основной цикл !!!!
};


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    int_fast32_t n;
    int_fast32_t x, y;

    std::cin >> n;

    std::cin >> x >> y;
    Circle circle = Circle(double x, double y);

    for (int_fast32_t i = 1; i < n; i++){
        std::cin >> x >> y;
    }


}