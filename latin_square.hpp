#include <vector>
#include <cassert>
#include <random>
#include <map>


#define DEBUG_LATIN_SQUARE true


int randint(int lo, int hi) {
    return rand() % (hi - lo + 1) + lo;
}


struct Point3 {
    int x, y, z;

    Point3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

    Point3() : Point3(0, 0, 0) {}    
};


template<int N> // N: dimension of the latin square
class LatinSquare {
public:
    int table[N][N];

    LatinSquare() {}

    static LatinSquare<N> cyclicLatinSquare() {
        LatinSquare<N> ls;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                ls.table[i][j] = (i + j) % N;
            }
        }
        return ls;
    }

};


template<int N>
class LatinSquareAsCube {
public:
    int cube[N][N][N];
    bool proper = true;
    Point3 improper;

    LatinSquareAsCube() {
        fromLatinSquare(LatinSquare<N>::cyclicLatinSquare());
    }

    LatinSquareAsCube(const LatinSquare<N>& ls) {
        fromLatinSquare(ls);
    }

    std::vector<int> onesInLine(int x, int y, int z) {
        assert(x == -1 or y == -1 or z == -1);
        std::vector<int> indices;
        if (x == -1) {
            for (int i = 0; i < N; ++i) {
                if (cube[i][y][z] == 1) {
                    indices.push_back(i);
                }
            }
        }
        else if (y == -1) {
            for (int i = 0; i < N; ++i) {
                if (cube[x][i][z] == 1) {
                    indices.push_back(i);
                }
            }
        }
        else if (z == -1) {
            for (int i = 0; i < N; ++i) {
                if (cube[x][y][i] == 1) {
                    indices.push_back(i);
                }
            }
        }
        return indices;
    }

    void fromLatinSquare(const LatinSquare<N>& ls) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    if (ls.table[i][j] == k) {
                        cube[i][j][k] = 1;
                    }
                    else {
                        cube[i][j][k] = 0;
                    }
                }
            }
        }
    }

    LatinSquare<N> toLatinSquare() {
        assert(proper);

        LatinSquare<N> ls;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    if (cube[i][j][k] == 1) {
                        ls.table[i][j] = k;
                    }
                }
            }
        }
        
        return ls;
    }

    void debugCheck() {
        std::map<int,int> cnt;
        
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    ++cnt[cube[i][j][k]];
                }
            }
        }

        if (proper) {
            assert(cnt[-1] == 0);
        }
        else {
            assert(cnt[-1] == 1);
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int sum = 0;
                for (int k = 0; k < N; ++k) {
                    sum += cube[i][j][k];
                }
                assert(sum == 1);
            }
        }

        for (int i = 0; i < N; ++i) {
            for (int k = 0; k < N; ++k) {
                int sum = 0;
                for (int j = 0; j < N; ++j) {
                    sum += cube[i][j][k];
                }
                assert(sum == 1);
            }
        }

        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                int sum = 0;
                for (int i = 0; i < N; ++i) {
                    sum += cube[i][j][k];
                }
                assert(sum == 1);
            }
        }
    }
};


template<int N>
std::vector<LatinSquare<N>> generateAllLatinSquares() {
    
}


template<int N>
void applyJacobsonMatthewsMove(LatinSquareAsCube<N>& lsc, Point3 pos1, Point3 pos2) {
    ++lsc.cube[pos1.x][pos1.y][pos1.z];
    --lsc.cube[pos1.x][pos1.y][pos2.z];
    --lsc.cube[pos1.x][pos2.y][pos1.z];
    --lsc.cube[pos2.x][pos1.y][pos1.z];
    ++lsc.cube[pos1.x][pos2.y][pos2.z];
    ++lsc.cube[pos2.x][pos1.y][pos2.z];
    ++lsc.cube[pos2.x][pos2.y][pos1.z];
    --lsc.cube[pos2.x][pos2.y][pos2.z];
    if (lsc.cube[pos2.x][pos2.y][pos2.z] == -1) {
        lsc.proper = false;
        lsc.improper = pos2;
    }
    else {
        lsc.proper = true;
        lsc.improper = Point3();
    }
}


template<int N>
void makeJacobsonMatthewsMove(LatinSquareAsCube<N>& lsc) {
    if (lsc.proper) {
        std::vector<Point3> zeros;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    if (lsc.cube[i][j][k] == 0) {
                        zeros.push_back(Point3(i, j, k));
                    }
                }
            }
        }
        int n = zeros.size();
        Point3 pos1 = zeros[randint(0, n-1)];
        std::vector<int> oneX = lsc.onesInLine(-1, pos1.y, pos1.z);
        std::vector<int> oneY = lsc.onesInLine(pos1.x, -1, pos1.z);
        std::vector<int> oneZ = lsc.onesInLine(pos1.x, pos1.y, -1);
        Point3 pos2(oneX[0], oneY[0], oneZ[0]);
        applyJacobsonMatthewsMove(lsc, pos1, pos2);
    }
    else {
        Point3 pos1 = lsc.improper;
        std::vector<int> onesX = lsc.onesInLine(-1, pos1.y, pos1.z);
        std::vector<int> onesY = lsc.onesInLine(pos1.x, -1, pos1.z);
        std::vector<int> onesZ = lsc.onesInLine(pos1.x, pos1.y, -1);
        Point3 pos2(onesX[randint(0, 1)], onesY[randint(0, 1)], onesZ[randint(0, 1)]);
        applyJacobsonMatthewsMove(lsc, pos1, pos2);
    }

#ifdef DEBUG_LATIN_SQUARE
    lsc.debugCheck();
#endif 
}


template<int N>
LatinSquare<N> sampleJacobsonMatthews(int moves) {
    assert(moves >= 0);

    LatinSquareAsCube<N> lsc;
    while (moves > 0 or not lsc.proper) {
        makeJacobsonMatthewsMove(lsc);
        --moves;
    }

    return lsc.toLatinSquare();
}
