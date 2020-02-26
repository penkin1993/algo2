#include <iostream>
#include <fstream>
#include <vector>






int main() {
    std::ifstream input_file("input.txt");
    //std::ofstream output_file("output.txt");
    int n, k;
    input_file >> n >> k;

    int r;
    std::vector<int> reward;
    for (int i = 0; i < n - 2; i++) {
        input_file >> r;
        reward.push_back(r);
        //std::cout << r;
    }



    return 0;
}



//std::cout << n << " " << k;