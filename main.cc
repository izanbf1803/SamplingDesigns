#include "latin_square.hpp"
#include "one_factorization.hpp"
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
using namespace std;


int main() {
    #define N 8
    #define K 2
    const int SEED = 0xCAFFE;

    srand(SEED);

    cout << "PARAMETERS:" << endl;
    cout << "N = " << N << endl;
    cout << "K = " << K << endl;
    cout << endl;


    LatinSquare<N> ls;
    #if N == 4
        ls.table = {{
            {3,0,1,2},
            {0,3,2,1},
            {1,2,3,0},
            {2,1,0,3}
        }};
    #endif
    #if N == 6
        ls.table = {{
            {5, 1, 2, 3, 4, 0},
            {1, 5, 3, 4, 0, 2},
            {2, 3, 5, 0, 1, 4},
            {3, 4, 0, 5, 2, 1},
            {4, 0, 1, 2, 5, 3},
            {0, 2, 4, 1, 3, 5}
        }};
    #endif
    #if N == 8
    ls.table = {{
        {7, 0, 1, 2, 3, 4, 5, 6},
        {0, 7, 2, 1, 4, 3, 6, 5},
        {1, 2, 7, 0, 5, 6, 3, 4},
        {2, 1, 0, 7, 6, 5, 4, 3},
        {3, 4, 5, 6, 7, 0, 1, 2},
        {4, 3, 6, 5, 0, 7, 2, 1},
        {5, 6, 3, 4, 1, 2, 7, 0},
        {6, 5, 4, 3, 2, 1, 0, 7}
    }};
    #endif 
    OneFactorizationAsCube<N,K> of(ls);
    of.debugCheck();

    const int IT = N * N * N * 20000;


    unordered_set<OneFactorizationAsCube<N,K>,OneFactorizationAsCubeHash<N,K>> visited;

    for (int it = 0; it < IT or (not of.proper()); ++it) {
        if (it % 10000 == 0)
            cerr << setprecision(2) << "$ " << (float)it/IT << endl;
        if (of.proper()) {
            visited.insert(of);
            bool ok = OneFactorizationAsCube<N,K>::validOneFactorization(of.toLatinSquare());
            assert(ok);
        }
        makeSymmetricJacobsonMatthewsMove<N,K>(of);
    }

    LatinSquare<N> x = of.toLatinSquare();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << int(x.table[i][j]) << ' ';
        }
        cout << endl;
    }

    of.debugCheck();

    cout << visited.size() << endl;

    unordered_set<LatinSquareAsCube<N,1>,LatinSquareAsCubeHash<N,1>> visited2;

    cerr.setf(ios::fixed);
    LatinSquareAsCube<N,1> lsc(LatinSquare<N>::cyclicLatinSquare());

    for (int it = 0; it < IT; ++it) {
        if (it % 10000 == 0)
            cerr << setprecision(2) << "$ " << (float)it/IT << endl;
        makeJacobsonMatthewsMove(lsc);
        if (lsc.proper() and OneFactorizationAsCube<N,1>::validOneFactorization(lsc.toLatinSquare())) {
            visited2.insert(lsc);
        }
    }

    cout << visited2.size() << endl;
}