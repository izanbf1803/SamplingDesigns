#include "latin_square.hpp"
#include "one_factorization.hpp"
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
using namespace std;


int main() {
    const int N = 4;
    const int K = 1;
    const int SEED = 0xCAFFE;

    srand(SEED);

    cout << "PARAMETERS:" << endl;
    cout << "N = " << N << endl;
    cout << "K = " << K << endl;
    cout << endl;

    LatinSquareAsCube<N,K> ls(LatinSquare<N>::cyclicLatinSquare());

    // unordered_set<LatinSquareAsCube<N,K>> visited;
    unordered_set<size_t> visited;
    if (ls.proper()) visited.insert(LatinSquareAsCubeHash<N,K>()(ls));
    makeJacobsonMatthewsMove(ls);
    const int IT = N * N * 10000;

    cerr.setf(ios::fixed);

    for (int it = 0; it < IT; ++it) {
        if (it % 10000 == 0)
            cerr << setprecision(2) << "$ " << (float)it/IT << endl;
        makeGeneralMove(ls); // makeJacobsonMatthewsMove(ls);
        if (ls.proper()) visited.insert(LatinSquareAsCubeHash<N,K>()(ls));
    }

    cout << visited.size() << endl;









    // const int N = 4;
    // const int K = 4;
    // const int SEED = 0xCAFFE;

    // srand(SEED);

    // cout << "PARAMETERS:" << endl;
    // cout << "N = " << N << endl;
    // cout << "K = " << K << endl;
    // cout << endl;


    // LatinSquare<N> ls;
    // ls.table = {{
    //     {3,0,1,2},
    //     {0,3,2,1},
    //     {1,2,3,0},
    //     {2,1,0,3}
    // }};
    // OneFactorizationAsCube<N,K> of(ls);


    // unordered_set<OneFactorizationAsCube<N,K>,OneFactorizationAsCubeHash<N,K>> visited;

    // for (int it = 0; it < 10000 or (not of.proper()); ++it) {
    //     if (of.proper()) {
    //         visited.insert(of);
    //         cout << endl;
    //         LatinSquare<N> x = of.toLatinSquare();
    //         for (int i = 0; i < N; ++i) {
    //             for (int j = 0; j < N; ++j) {
    //                 cout << int(x.table[i][j]) << ' ';
    //             }
    //             cout << endl;
    //         }
    //     }
    //     makeSymmetricJacobsonMatthewsMove<N,K>(of);
    // }

    // LatinSquare<N> x = of.toLatinSquare();
    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < N; ++j) {
    //         cout << int(x.table[i][j]) << ' ';
    //     }
    //     cout << endl;
    // }

    // of.debugCheck();

    // cout << visited.size() << endl;

    // unordered_set<LatinSquareAsCube<N,K>,LatinSquareAsCubeHash<N,K>> visited2;
    // const int IT = N * N * N * 1000;

    // cerr.setf(ios::fixed);
    // LatinSquareAsCube<N,K> lsc(LatinSquare<N>::cyclicLatinSquare());

    // for (int it = 0; it < IT; ++it) {
    //     if (it % 10000 == 0)
    //         cerr << setprecision(2) << "$ " << (float)it/IT << endl;
    //     makeJacobsonMatthewsMove(lsc);
    //     if (lsc.proper() and OneFactorizationAsCube<N,K>::symmetric(lsc.toLatinSquare()))
    //         visited2.insert(ls);
    // }

    // cout << visited2.size() << endl;








    // const int N = 8;
    // const int K = 4;
    // const int SEED = 0xCAFFE;

    // srand(SEED);

    // cout << "PARAMETERS:" << endl;
    // cout << "N = " << N << endl;
    // cout << "K = " << K << endl;
    // cout << endl;
    // LatinSquare<N> ls = sampleJacobsonMatthews<N, K>(1000);

    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < N; ++j) {
    //         cout << char(ls.table[i][j] + 'a') << ' ';
    //     }
    //     cout << endl;
    // }
}