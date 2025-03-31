#include "latin_square.hpp"
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
using namespace std;

int main() {
    const int N = 5;
    const int K = 1;
    const int SEED = 0xCAFFE;

    srand(SEED);

    cout << "PARAMETERS:" << endl;
    cout << "N = " << N << endl;
    cout << "K = " << K << endl;
    cout << endl;

    LatinSquareAsCube<N,K> ls;

    // unordered_set<LatinSquareAsCube<N,K>> visited;
    unordered_set<size_t> visited;
    if (ls.proper()) visited.insert(std::hash<LatinSquareAsCube<N,K>>()(ls));
    makeJacobsonMatthewsMove(ls);
    const int IT = 1e7;

    cerr.setf(ios::fixed);

    for (int it = 0; it < IT; ++it) {
        if (it % 10000 == 0)
            cerr << setprecision(2) << "$ " << (float)it/IT << endl;
        makeJacobsonMatthewsMove(ls);
        if (ls.proper()) visited.insert(std::hash<LatinSquareAsCube<N,K>>()(ls));
    }

    cout << visited.size() << endl;

    // LatinSquare<N> ls = sampleJacobsonMatthews<N, K>(1000);

    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < N; ++j) {
    //         cout << char(ls.table[i][j] + 'a') << ' ';
    //     }
    //     cout << endl;
    // }
}