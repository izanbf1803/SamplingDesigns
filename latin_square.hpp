#include <vector>
#include <array>
#include <cassert>
#include <random>
#include <map>


#define DEBUG_LATIN_SQUARE true


int randint(int lo, int hi) {
    return rand() % (hi - lo + 1) + lo;
}


template<typename T>
std::vector<T> mergeVectors(const std::vector<T>& A, const std::vector<T>& B) {
    std::vector<T> C;
    C.reserve(A.size() + B.size());
    C.insert(C.end(), A.begin(), A.end());
    C.insert(C.end(), B.begin(), B.end());
    return C;
}


template<typename T>
T sampleUniformly(const std::vector<T>& v) {
    int n = v.size();
    return v[randint(0, n-1)];
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


template<int N, int K> // N denotes the size, K the maximum number of improper positions
class LatinSquareAsCube {
public:
    std::array<std::array<std::array<int, N>, N>, N> cubeArray;
    int negatives;

    LatinSquareAsCube() {
        fromLatinSquare(LatinSquare<N>::cyclicLatinSquare());
        negatives = 0;
    }

    LatinSquareAsCube(const LatinSquare<N>& ls) {
        fromLatinSquare(ls);
        negatives = 0;
    }

    bool operator==(const LatinSquareAsCube<N,K>& rhs) const {
        return rhs.cubeArray == cubeArray;
    }

    inline int atCube(int i, int j, int k) const {
        return cubeArray[i][j][k];
    }

    void setCube(int i, int j, int k, int val) {
        int& entry = cubeArray[i][j][k];
        if (entry < 0) --negatives;
        entry = val;
        if (val < 0) ++negatives;
    }

    void incCube(int i, int j, int k) {
        setCube(i, j, k, atCube(i, j, k) + 1);
    }
    
    void decCube(int i, int j, int k) {
        setCube(i, j, k, atCube(i, j, k) - 1);
    }

    std::vector<Point3> positionsWithValue(int value) const {
        std::vector<Point3> points;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    if (atCube(i, j, k) == value) {
                        points.push_back(Point3(i, j, k));
                    }
                }
            }
        }
        return points;
    }

    std::vector<Point3> improper() const {
        return positionsWithValue(-1);
    }

    bool proper() const {
        return negatives == 0;
    }

    std::vector<int> valueInLine(int value, int x, int y, int z) const {
        assert(x == -1 or y == -1 or z == -1);
        std::vector<int> indices;
        if (x == -1) {
            for (int i = 0; i < N; ++i) {
                if (atCube(i, y, z) == value) {
                    indices.push_back(i);
                }
            }
        }
        else if (y == -1) {
            for (int i = 0; i < N; ++i) {
                if (atCube(x, i, z) == value) {
                    indices.push_back(i);
                }
            }
        }
        else if (z == -1) {
            for (int i = 0; i < N; ++i) {
                if (atCube(x, y, i) == value) {
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
                        setCube(i, j, k, 1);
                    }
                    else {
                        setCube(i, j, k, 0);
                    }
                }
            }
        }
    }

    LatinSquare<N> toLatinSquare() const {
        assert(proper());

        LatinSquare<N> ls;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    if (atCube(i, j, k) == 1) {
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
                    ++cnt[atCube(i, j, k)];
                }
            }
        }

        if (proper()) {
            assert(cnt[-1] == 0);
        }
        else {
            assert(cnt[-1] == 1);
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int sum = 0;
                for (int k = 0; k < N; ++k) {
                    sum += atCube(i, j, k);
                }
                assert(sum == 1);
            }
        }

        for (int i = 0; i < N; ++i) {
            for (int k = 0; k < N; ++k) {
                int sum = 0;
                for (int j = 0; j < N; ++j) {
                    sum += atCube(i, j, k);
                }
                assert(sum == 1);
            }
        }

        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                int sum = 0;
                for (int i = 0; i < N; ++i) {
                    sum += atCube(i, j, k);
                }
                assert(sum == 1);
            }
        }
    }
};


template<int N, int K>
struct std::hash<LatinSquareAsCube<N,K>> {
    std::size_t operator()(const LatinSquareAsCube<N,K>& ls) const {
        const long long M = 9'949'370'777'987'917ll;
        const long long P = 13;
        static_assert(2.0*M*P < 1e18);

        long long H = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    H += ls.cubeArray[i][j][k];
                    H *= P;
                    H %= M;
                }
            }
        }
        return std::size_t((H + M) % M);
    }
};


template<int N, int K>
void applyJacobsonMatthewsMove(LatinSquareAsCube<N, K>& lsc, Point3 pos1, Point3 pos2) {
    lsc.incCube(pos1.x, pos1.y, pos1.z); // ++
    lsc.decCube(pos1.x, pos1.y, pos2.z); // --
    lsc.decCube(pos1.x, pos2.y, pos1.z); // --
    lsc.decCube(pos2.x, pos1.y, pos1.z); // --
    lsc.incCube(pos1.x, pos2.y, pos2.z); // ++
    lsc.incCube(pos2.x, pos1.y, pos2.z); // ++
    lsc.incCube(pos2.x, pos2.y, pos1.z); // ++
    lsc.decCube(pos2.x, pos2.y, pos2.z); // --
}


template<int N, int K>
void makeJacobsonMatthewsMove(LatinSquareAsCube<N, K>& lsc) {
    // TODO use K!!!
    if (lsc.proper()) {
        Point3 pos1 = sampleUniformly(lsc.positionsWithValue(0));
        std::vector<int> oneX = lsc.valueInLine(1, -1, pos1.y, pos1.z);
        std::vector<int> oneY = lsc.valueInLine(1, pos1.x, -1, pos1.z);
        std::vector<int> oneZ = lsc.valueInLine(1, pos1.x, pos1.y, -1);
        Point3 pos2(oneX[0], oneY[0], oneZ[0]);
        applyJacobsonMatthewsMove(lsc, pos1, pos2);
    }
    else {
        Point3 pos1 = sampleUniformly(lsc.positionsWithValue(-1));
        int x = sampleUniformly(lsc.valueInLine(1, -1, pos1.y, pos1.z));
        int y = sampleUniformly(lsc.valueInLine(1, pos1.x, -1, pos1.z));
        int z = sampleUniformly(lsc.valueInLine(1, pos1.x, pos1.y, -1));
        Point3 pos2(x, y, z);
        applyJacobsonMatthewsMove(lsc, pos1, pos2);
    }

#ifdef DEBUG_LATIN_SQUARE
    lsc.debugCheck();
#endif 
}


template<int N, int K>
LatinSquare<N> sampleJacobsonMatthews(int moves) {
    assert(moves >= 0);

    LatinSquareAsCube<N, K> lsc;
    while (moves > 0 or (not lsc.proper())) {
        makeJacobsonMatthewsMove(lsc);
        --moves;
    }

    return lsc.toLatinSquare();
}
