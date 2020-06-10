#include <iostream>
#include <iomanip>
#include <queue>

double F_INF = 1e9;
double EPS = 1e-9;

struct Line {
    double a, b, c;

    explicit Line(double a_ = 0, double b_ = 0, double c_ = 0) : a(a_), b(b_), c(c_) {}
};

struct Point {
    double x, y;

    explicit Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}

    bool intersect_sign(Line line_) const {
        return line_.a * x + line_.b * y + line_.c > EPS;
    }

    double vector_prod(Point point_) const {
        return x * point_.y - y * point_.x;
    }
};

struct Segment {

public:

    Point point0, point1;

    Line line;

    explicit Segment(Point point0_, Point point1_) : point0(point0_), point1(point1_) {
        // точки не совпадают
        line.a = point0.y - point1.y;
        line.b = point1.x - point0.x;
        line.c = point0.x * point1.y - point1.x * point0.y;
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
    std::queue<Point> points;

};

Figure::Figure() {
    points.emplace(Point(-F_INF, -F_INF));
    points.emplace(Point(F_INF, -F_INF));
    points.emplace(Point(F_INF, F_INF));
    points.emplace(Point(-F_INF, F_INF));
}

double Figure::calc_square() {
    double square = 0;

    Point first_point = points.front();

    Point next_point0 = points.front();
    points.pop();

    while (!points.empty()) {
        Point next_point1 = points.front();
        points.pop();

        square += 0.5 * next_point0.vector_prod(next_point1);

        next_point0 = next_point1;
    }
    square += 0.5 * next_point0.vector_prod(first_point);

    return square;
}

void Figure::split(double a, double b, double c) {
    Line new_line = Line(a, b, c);

    int_fast32_t n = points.size();
    Point first_point = points.front();

    Point next_point0 = points.front();
    points.pop();

    for (int_fast32_t i = 0; i < n - 1; i++) {

        Point next_point1 = points.front();
        points.pop();

        if (next_point0.intersect_sign(new_line) & next_point1.intersect_sign(new_line)) {
            points.push(next_point1);

        } else if (!next_point0.intersect_sign(new_line) & !next_point1.intersect_sign(new_line)) {
            next_point0 = next_point1;
            continue;

        } else {
            Point new_point = Segment(next_point0, next_point1).intersection(new_line);
            points.push(new_point);

            if (next_point1.intersect_sign(new_line)) {
                points.push(next_point1);
            }
        }
        next_point0 = next_point1;
    }

    if (next_point0.intersect_sign(new_line) & first_point.intersect_sign(new_line)) {
        points.push(first_point);

    } else if (next_point0.intersect_sign(new_line) & !first_point.intersect_sign(new_line)) {
        Point new_point = Segment(next_point0, first_point).intersection(new_line);
        points.push(new_point);

    } else if (!next_point0.intersect_sign(new_line) & first_point.intersect_sign(new_line)) {
        Point new_point = Segment(next_point0, first_point).intersection(new_line);
        points.push(new_point);
        points.push(first_point);
    }
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

    std::cout << figure.calc_square();

    return 0;
}
