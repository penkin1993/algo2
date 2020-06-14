#include <iostream>
#include <queue>
#include <utility>
#include <cmath>
#include <algorithm>

double F_INF = 1e4 + 1;
double F_INF_LIM = 1e4;
double EPS = 1e-8;
double SQUARE_LIM = 1e-8;


struct Line {
    double a{}, b{}, c{};

    explicit Line(double a_ = 0, double b_ = 0, double c_ = 0) : a(a_), b(b_), c(c_) {}
};

struct Point {
    double x, y;

    explicit Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}

    bool is_on_the_line(Line line_) const {
        return std::fabs(line_.a * x + line_.b * y + line_.c) < EPS;
    }

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
        line.a = point0.y - point1.y;
        line.b = point1.x - point0.x;
        line.c = point0.x * point1.y - point1.x * point0.y;
    }

    bool is_intersect(Line line_) const {
        double sign0 = line_.a * point0.x + line_.b * point0.y + line_.c;
        double sign1 = line_.a * point1.x + line_.b * point1.y + line_.c;

        return ((sign0 >= EPS) && (sign1 <= -EPS)) || ((sign0 <= -EPS) && (sign1 >= EPS));
    }

    Point intersection(Line line_) const {
        if (!is_intersect(line_)) {
            throw std::invalid_argument("Received a wrong value");
        }
        double d = line.a * line_.b - line.b * line_.a;

        double dx = line.b * line_.c - line_.b * line.c;

        double dy = line.c * line_.a - line_.c * line.a;

        return Point(dx / d, dy / d);
    }
};

class Figure {

public:
    explicit Figure();

    explicit Figure(std::queue<Point> points_);

    bool split(std::queue<Figure> *figure_queue, double a, double b, double c);

    double calc_square();

    std::queue<Point> points;

private:
    static bool check_lim(Point point);
};


Figure::Figure() {
    points.emplace(Point(-F_INF, -F_INF));
    points.emplace(Point(F_INF, -F_INF));
    points.emplace(Point(F_INF, F_INF));
    points.emplace(Point(-F_INF, F_INF));
}

Figure::Figure(std::queue<Point> points_) {
    points = std::move(points_);
}

bool Figure::check_lim(Point point) {
    return (std::abs(point.x) >= F_INF_LIM) || (std::abs(point.y) >= F_INF_LIM);
}

bool Figure::split(std::queue<Figure> *figure_queue, double a, double b, double c) {
    Line new_line = Line(a, b, c);

    std::queue<Point> new_points;

    int_fast32_t n = points.size();

    Point next_point0 = points.front();
    points.pop();

    points.push(next_point0);

    for (int_fast32_t i = 0; i < n; i++) {

        Point next_point1 = points.front();
        points.pop();

        if (next_point1.is_on_the_line(new_line)) {
            points.push(next_point1);
            new_points.push(next_point1);

        } else if ((next_point0.intersect_sign(new_line) || next_point0.is_on_the_line(new_line)) &&
                   next_point1.intersect_sign(new_line)) {
            points.push(next_point1);

        } else if ((!next_point0.intersect_sign(new_line) || next_point0.is_on_the_line(new_line)) &&
                   !next_point1.intersect_sign(new_line)) {

            new_points.push(next_point1);

        } else {
            Point new_point = Segment(next_point0, next_point1).intersection(new_line);
            points.push(new_point);
            new_points.push(new_point);

            if (next_point1.intersect_sign(new_line)) {
                points.push(next_point1);
            } else {
                new_points.push(next_point1);
            }
        }
        next_point0 = next_point1;
    }

    if (new_points.size() > 2) {
        figure_queue->push(Figure(new_points));
    }
    return points.size() > 2;
}

double Figure::calc_square() {
    double square = 0;

    Point first_point = points.front();
    if (check_lim(first_point)) {
        return 0;
    }

    Point next_point0 = points.front();
    if (check_lim(next_point0)) {
        return 0;
    }
    points.pop();

    while (!points.empty()) {
        Point next_point1 = points.front();
        if (check_lim(next_point1)) {
            return 0;
        }
        points.pop();

        square += 0.5 * next_point0.vector_prod(next_point1);

        next_point0 = next_point1;
    }
    square += 0.5 * next_point0.vector_prod(first_point);

    return square;
}


Line get_line(Point point0_, Point point1_) {
    double a = point0_.y - point1_.y;
    double b = point1_.x - point0_.x;
    double c = point0_.x * point1_.y - point1_.x * point0_.y;

    return Line(a, b, c);
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    int_fast32_t n;
    std::cin >> n;

    std::queue<Figure> figure_queue;

    figure_queue.push(Figure());

    double x0, y0, x1, y1;

    for (int_fast32_t i = 0; i < n; i++) {
        std::cin >> x0 >> y0 >> x1 >> y1;

        Line line = get_line(Point(x0, y0), Point(x1, y1));
        int_fast32_t array_size = figure_queue.size();
        for (int_fast32_t j = 0; j < array_size; j++) {

            Figure figure = figure_queue.front();
            figure_queue.pop();

            if (figure.split(&figure_queue, line.a, line.b, line.c)) {
                figure_queue.push(figure);
            }
        }
    }
    std::vector<double> square_array;

    while (!figure_queue.empty()) {
        Figure figure = figure_queue.front();
        figure_queue.pop();

        double next_square = figure.calc_square();
        if (next_square > SQUARE_LIM) {
            square_array.push_back(next_square);
        }
    }
    std::cout << square_array.size() << "\n";
    std::sort(square_array.begin(), square_array.end());

    for (double i : square_array) {
        std::cout << i << "\n";
    }
    return 0;
}
/*

5
0 0 1 0
1 0 1 1
1 1 0 1
0 1 0 0
0 0 1 1

*/