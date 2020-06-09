#include <iostream>
#include <iomanip>
#include <vector>

double F_INF = 10000000;
double EPS = 1e-9;

struct Line {
    double a, b, c;

    explicit Line(double a_ = 0, double b_ = 0, double c_ = 0) : a(a_), b(b_), c(c_) {}
};

struct Point {
    double x, y;

    explicit Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}

    bool intersect_sign(Line line_) const {
        return line_.a * x + line_.b * y + line_.c > EPS; // TODO: Включить случай
    }

    double vector_prod(Point point_) const {
        return x * point_.y - y * point_.x;
    }
};


struct Segment {

public:

    Point point0, point1;

    Line line;

    explicit Segment(double x0_, double y0_,
                     double x1_, double y1_) : point0(x0_, y0_), point1(x1_, y1_) {
        // точки не совпадают
        line.a = y0_ - y1_;
        line.b = x1_ - x0_;
        line.c = x0_ * y1_ - x1_ * y0_;
    }

    bool is_intersect(Line line_) const {
        // функция персечения двух отрезков
        double sign0 = line_.a * point0.x + line_.b * point0.y + line_.c;
        double sign1 = line_.a * point1.x + line_.b * point1.y + line_.c;

        return sign0 * sign1 < 0;
    }

    Point intersection(Line line_) const {
        // пересекаются ли они. Если да, то возвращать точку, если нет, то возвращать F_INF + 1
        if (!is_intersect(line_)) {
            throw std::invalid_argument("Received wrong value");
        }

        // решаем уравнение
        double d = line.a * line_.b - line.b * line_.a;

        double dx = line.b * line_.c - line_.b * line.c;

        double dy = line.c * line_.a - line_.c * line.a;

        return Point(dx / d, dy / d);
    }

};


class Figure {

public:
    explicit Figure();

    void split(double a, double b, double c); // алгоритм сечения фигуры прямой

    double calc_square(); // вычисление площади получивашейся фигуры

private:
    std::vector<Segment> segments; // каждый раз создавать новый ???

};

Figure::Figure() {
    segments.emplace_back(-F_INF, -F_INF, -F_INF, F_INF);
    segments.emplace_back(-F_INF, F_INF, F_INF, F_INF);
    segments.emplace_back(F_INF, F_INF, F_INF, -F_INF);
    segments.emplace_back(F_INF, -F_INF, -F_INF, F_INF);
}

void Figure::split(double a, double b, double c) {
    Line new_line = Line(a, b, c);
    std::vector<Segment> new_segments;
    Point new_point;

    for (auto &segment : segments) {
        if (segment.point0.intersect_sign(new_line) & segment.point1.intersect_sign(new_line)) {
            new_segments.push_back(segment);

        } else if (!segment.point0.intersect_sign(new_line) & !segment.point1.intersect_sign(new_line)) {
            continue;

        } else {
            new_point = segment.intersection(new_line);
            if (segment.point0.intersect_sign(new_line)) {
                new_segments.emplace_back(segment.point0.x, segment.point0.y, new_point.x, new_point.y);
            } else {
                new_segments.emplace_back(segment.point1.x, segment.point1.y, new_point.x, new_point.y);
            }
        }
    }
    segments = new_segments;
}

double Figure::calc_square() {
    double square = 0;
    for (auto & segment : segments) {
     square += 0.5*segment.point0.vector_prod(segment.point1);
    }
    return square;
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
