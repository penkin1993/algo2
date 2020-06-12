#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>

double EPS = 1e-9;

struct Point {

public:
    double x;
    double y;

    explicit Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}

    double dist(Point p) const {
        return std::sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }
};

struct Circle {
    Point center;
    double radius;

    explicit Circle(Point center_ = Point(0, 0), double radius_ = 0) : center(center_), radius(radius_) {}

    bool is_contains(Point p) const {
        return (center.dist(p) <= radius + EPS);
    }

    Circle(Point p1, Point p2) {
        center = Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
        radius = p1.dist(p2) / 2;
    }

    Circle(Point a, Point b, Point c) {
        double ox = (std::min(std::min(a.x, b.x), c.x) + std::max(std::max(a.x, b.x), c.x)) / 2;
        double oy = (std::min(std::min(a.y, b.y), c.y) + std::max(std::max(a.y, b.y), c.y)) / 2;
        double ax = a.x - ox, ay = a.y - oy;

        double bx = b.x - ox, by = b.y - oy;
        double cx = c.x - ox, cy = c.y - oy;
        double d = (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by)) * 2;

        double x =
                ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) + (cx * cx + cy * cy) * (ay - by)) /
                d;
        double y =
                ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) + (cx * cx + cy * cy) * (bx - ax)) /
                d;
        center = Point(ox + x, oy + y);
        radius = std::max(std::max(center.dist(a), center.dist(b)), center.dist(c));
    }
};

Circle min_disc_with_2_points(std::vector<Point> p, Point q1, Point q2) {
    Circle d = Circle(q1, q2);
    int_fast32_t size = p.size();
    for (int_fast32_t i = 0; i < size; i++)
        if (!d.is_contains(p[i])) {
            d = Circle(p[i], q1, q2);
        }
    return d;
}

Circle min_disc_with_point(std::vector<Point> p, Point q0) {
    Circle d = Circle(q0, p[0]);
    int_fast32_t size = p.size();
    for (int_fast32_t i = 1; i < size; i++)
        if (!d.is_contains(p[i])) {
            std::vector<Point> Q;
            Q.insert(Q.begin(), p.begin(), p.begin() + i);
            d = min_disc_with_2_points(Q, p[i], q0);
        }
    return d;
}

Circle get_ans(std::vector<Point> points) {
    std::random_shuffle(points.begin(), points.end());
    Circle d = Circle(points[0], points[1]);
    int_fast32_t size = points.size();
    for (int_fast32_t i = 2; i < size; i++)
        if (!d.is_contains(points[i])) {
            std::vector<Point> q;
            q.insert(q.begin(), points.begin(), points.begin() + i);
            d = min_disc_with_point(q, points[i]);
        }
    return d;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    std::cout << std::fixed;
    std::cout << std::setprecision(10);

    int_fast32_t n;
    int_fast32_t x, y;

    std::cin >> n;

    std::vector<Point> points;

    for (int_fast32_t i = 0; i < n; i++) {
        std::cin >> x >> y;
        points.emplace_back(x, y);
    }

    Circle circle = get_ans(points);

    std::cout << circle.center.x << " " << circle.center.y << "\n";
    std::cout << circle.radius;

}