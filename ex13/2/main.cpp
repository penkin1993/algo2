#include <iostream>
#include <iomanip>
#include <vector>

double F_INF = 10000000;
double EPS = 1e-9;

struct Point {
    int_fast32_t x, y;

    explicit Point(double x_, double y_) : x(x_), y(y_) {}
};

struct Segment {

public:

    Point point0, point1;

    double a, b, c;


    explicit Segment(double x0_, double y0_,
                     double x1_, double y1_) : point0(x0_, y0_), point1(x1_, y1_) {
        // точки не совпадают
        a = y0_ - y1_;
        b = x1_ - x0_;
        c = x0_ * y1_ - x1_ * y0_;
    }

    Point intersection(Segment segment) const {
        // функция персечения двух отрезков
        double sign0 = a * segment.point0.x + a * segment.point0.y + c;
        double sign1 = a * segment.point1.x + a * segment.point1.y + c;

        // пересекаются ли они. Если да, то возвращать точку, если нет, то возвращать F_INF + 1
        if (sign0 * sign1 < 0) {
            return Point(F_INF + 1, 0);
        }

        // решаем уравнение
        double d = a * segment.b - b * segment.a;

        if (std::abs(d) < EPS) { // случай совпадения прямых
            return Point(-F_INF - 1, 0);
        }

        double dx = b * segment.c - segment.b * c;

        double dy = c * segment.a - segment.c * a;

        return Point(dx / d, dy / d);
    }

};

class Figure {

public:
    explicit Figure();

    void split(double a, double b, double c); // алгоритм сечения фигуры прямой

    double calc_square(); // вычисление площади получивашейся фигуры

private:
    std::vector<Segment> *segments{}; // каждый раз создавать новый ???

};

Figure::Figure() {
    segments->push_back(Segment(-F_INF, -F_INF, -F_INF, F_INF));
    segments->push_back(Segment(-F_INF, F_INF, F_INF, F_INF));
    segments->push_back(Segment(F_INF, F_INF, F_INF, -F_INF));
    segments->push_back(Segment(F_INF, -F_INF, -F_INF, F_INF));
}

void Figure::split(double a, double b, double c) {
    // TODO: основной алгоритм пересечения новой прямой

}



int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::cout << std::fixed;
    std::cout << std::setprecision(10);

    int_fast32_t n;
    std::cin >> n;

    Figure figure = Figure();

    double a, b, c;
    for (int_fast32_t i = 0; i < n; i++) {
        std::cin >> a >> b >> c;
        figure.split(a, b, c);
    }

    figure.calc_square();

    return 0;
}
