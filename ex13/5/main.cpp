#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

double EPS = 1e-8;

struct Point {

public:
    double x;
    double y;

    explicit Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}


    Point sub(const Point &p) const  {
        return Point{x - p.x, y - p.y};
    }

    double dist(Point p) const{
        return std::hypot(x - p.x, y - p.y);
    }

    double cross( Point p) const  {
        return x * p.y - y * p.x;
    }

};

struct Circle {

public:
    Point c;
    double r{};

    explicit Circle(Point c_ = Point(), double r_ = 0) : c(c_), r(r_) {}

    bool is_in_circle(Point p) const {
        return c.dist(p) <= r + EPS;
    }
};

Circle make_circle2(Point a, Point b) {
    Point c = Point((a.x + b.x) / 2, (a.y + b.y) / 2);
    return Circle{c, std::max(c.dist(a), c.dist(b))};
}

Circle make_circle3(Point a, Point b, Point c) {
    double ox = (std::min(std::min(a.x, b.x), c.x) + std::max(std::max(a.x, b.x), c.x)) / 2;
    double oy = (std::min(std::min(a.y, b.y), c.y) + std::max(std::max(a.y, b.y), c.y)) / 2;
    double ax = a.x - ox, ay = a.y - oy;


    double bx = b.x - ox, by = b.y - oy;
    double cx = c.x - ox, cy = c.y - oy;
    double d = (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by)) * 2;

    double x =
            ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) + (cx * cx + cy * cy) * (ay - by)) / d;
    double y =
            ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) + (cx * cx + cy * cy) * (bx - ax)) / d;
    Point p{ox + x, oy + y};


    double r = std::max(std::max(p.dist(a), p.dist(b)), p.dist(c));
    return Circle(p, r);
}

static Circle min_disc_with_2points(std::vector<Point> *points, size_t end, Point p, Point q) {
    Circle circle = make_circle2(p, q);
    Circle left;
    Circle right;

    Point pq = q.sub(p);
    for (size_t i = 0; i < end; i++) {
        const Point &r = points->at(i);
        if (circle.is_in_circle(r)) {
            continue;
        }

        double cross = pq.cross(r.sub(p));
        Circle c = make_circle3(p, q, r);
        if (c.r < 0) {
            continue;
        }
        else if (cross > 0 && (left.r < 0 || pq.cross(c.c.sub(p)) > pq.cross(left.c.sub(p)))) {
            left = c;
        }
        else if (cross < 0 && (right.r < 0 || pq.cross(c.c.sub(p)) < pq.cross(right.c.sub(p)))) {
            right = c;
        }
    }

    if (left.r < 0 && right.r < 0) {
        return circle;
    }
    else if (left.r < 0) {
        return right;
    }
    else if (right.r < 0) {
        return left;
    }
    else {
        return left.r <= right.r ? left : right;
    }
}

static Circle min_disc_with_point(std::vector<Point> *points, size_t end, Point p) {
    Circle circle = Circle(p, 0);
    for (size_t i = 0; i < end; i++) {
        const Point &q = points->at(i);
        if (!circle.is_in_circle(q)) {
            if (circle.r == 0) {
                circle = make_circle2(p, q);
            }
            else {
                circle = min_disc_with_2points(points, i + 1, p, q);
            }
        }
    }
    return circle;
}

Circle make_disk_with_points(std::vector<Point> * points) {
    Circle c;
    for (size_t i = 0; i < points->size(); i++) {
        const Point &p = points->at(i);
        if (!c.is_in_circle(p)) {
            c = min_disc_with_point(points, i + 1, p);
        }
    }
    return c;
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

    Circle circle = make_disk_with_points(&points);

    std::cout << circle.c.x << " " << circle.c.y << "\n";
    std::cout << circle.r;

}

/*
3
0 2
0 0
2 0

1.00000000000000000000 1.00000000000000000000
1.41421356237309514547
 */