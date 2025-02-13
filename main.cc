#include "latin_square.hpp"
#include <iostream>
using namespace std;

int main() {
    const int N = 10;
    const int SEED = 0xCAFFE;

    srand(SEED);

    LatinSquare<N> ls = sampleJacobsonMatthews<N>(1000);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << char(ls.table[i][j] + 'a') << ' ';
        }
        cout << endl;
    }
}