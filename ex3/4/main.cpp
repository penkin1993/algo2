#include <iostream>
#include <vector>

int get_gcd(int a, int b){
    if (a == 0){
        return b;
    } else if (b == 0){
        return a;
    } else{
        return get_gcd(b % a, a);
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int gcd;
    int a, b, c;
    std::cin >> a >> b >> c;

    gcd = get_gcd(a, b);
    if (c % gcd == 0){
        std::cout << gcd;
        
    } else{
        std::cout << -1;
    }


    return 0;
}

/*
2 5 3


*/