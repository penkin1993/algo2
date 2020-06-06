#include <iostream>
#include <iomanip>






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
