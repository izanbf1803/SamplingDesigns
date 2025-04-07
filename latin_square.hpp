#ifndef __LATIN_SQUARE__
#define __LATIN_SQUARE__

#include <vector>
#include <array>
#include <cassert>
#include <random>
#include <map>
#include <iostream>
#include <iomanip>


#define DEBUG_LATIN_SQUARE true


int randint(int lo, int hi) {
    return rand() % (hi - lo + 1) + lo;
}

template<typename T>
bool inRange(T low, T value, T high) {
    return low <= value and value <= high;
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
    assert(n >= 1);
    return v[randint(0, n-1)];
}


struct Point3 {
    int x, y, z;

    Point3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

    Point3() : Point3(0, 0, 0) {}    
};

std::ostream& operator<<(std::ostream& os, const Point3& p) {
    return os << "(" << p.x << "," << p.y << "," << p.z << ")";
}


template<int N> // N: dimension of the latin square
class LatinSquare {
public:
    std::array<std::array<int, N>, N> table;

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

    // static LatinSquare<N> symmetricLatinSquare() {
    //     LatinSquare<N> ls;
    //     // for (int i = 0; i < N; ++i) {
    //     //     ls.table[i][i] = N - 1;
    //     //     for (int j = 0; j < i; ++j) {
    //     //         ls.table[j][i] = ls.table[i][j] = (i + j) % (N - 1);
    //     //     }
    //     // }
    //     for (int i = 0; i < N; ++i) {
    //         ls.table[i][i] = N - 1;
    //         for (int j = i+1; j < N; ++j) {
    //             int val = (i + j) % (N - 1);
    //             ls.table[j][i] = ls.table[i][j] = val;
    //         }
    //     }
    //     return ls;
    // }
};


template<int N, int K> // N denotes the size, K the maximum number of improper positions
class LatinSquareAsCube {
public:
    std::array<std::array<std::array<int, N>, N>, N> cubeArray;
    int negativeCnt;

    // LatinSquareAsCube() {
    //     fromLatinSquare(LatinSquare<N>::cyclicLatinSquare());
    //     this->negativeCnt = 0;
    //     std::cout << "WHAAT" << std::endl;
    // }

    LatinSquareAsCube(const LatinSquare<N>& ls) {
        fromLatinSquare(ls);
        this->negativeCnt = 0;
    }

    bool operator==(const LatinSquareAsCube<N,K>& rhs) const {
        return rhs.cubeArray == this->cubeArray;
    }

    inline int atCube(int i, int j, int k) const {
        return this->cubeArray[i][j][k];
    }

    void setCube(int i, int j, int k, int val) {
        int& entry = this->cubeArray[i][j][k];
        if (entry < 0) --this->negativeCnt;
        entry = val;
        if (val < 0) ++this->negativeCnt;
    }

    void cubeAdd(int i, int j, int k, int value) {
        this->setCube(i, j, k, this->atCube(i, j, k) + value);
    }

    std::vector<Point3> positionsWithValue(int value) const {
        std::vector<Point3> points;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    if (this->atCube(i, j, k) == value) {
                        points.push_back(Point3(i, j, k));
                    }
                }
            }
        }
        return points;
    }

    // std::vector<Point3> strongProperPositions() const {
    //     std::vector<Point3> points;
    //     for (int i = 0; i < N; ++i) {
    //         for (int j = 0; j < N; ++j) {
    //             for (int k = 0; k < N; ++k) {
    //                 bool stronglyProper = 
    //                         this->atCube(i, j, k) == 0;
    //                     // and this->valueInLine(-1, -1, j, k).empty()
    //                     // and this->valueInLine(-1, i, -1, k).empty()
    //                     // and this->valueInLine(-1, i, j, -1).empty();
    //                 if (stronglyProper) {
    //                     points.push_back(Point3(i, j, k));
    //                 }
    //             }
    //         }
    //     }
    //     return points;
    // }

    std::vector<Point3> improper() const {
        return this->positionsWithValue(-1);
    }

    bool proper() const {
        return this->negativeCnt == 0;
    }

    std::vector<int> valueInLine(int value, int x, int y, int z) const {
        assert(x == -1 or y == -1 or z == -1);
        std::vector<int> indices;
        if (x == -1) {
            for (int i = 0; i < N; ++i) {
                if (this->atCube(i, y, z) == value) {
                    indices.push_back(i);
                }
            }
        }
        else if (y == -1) {
            for (int i = 0; i < N; ++i) {
                if (this->atCube(x, i, z) == value) {
                    indices.push_back(i);
                }
            }
        }
        else if (z == -1) {
            for (int i = 0; i < N; ++i) {
                if (this->atCube(x, y, i) == value) {
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
                        this->setCube(i, j, k, 1);
                    }
                    else {
                        this->setCube(i, j, k, 0);
                    }
                }
            }
        }
    }

    LatinSquare<N> toLatinSquare() const {
        assert(this->proper());

        LatinSquare<N> ls;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    if (this->atCube(i, j, k) == 1) {
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
                    ++cnt[this->atCube(i, j, k)];
                }
            }
        }

        if (proper()) {
            assert(cnt[-1] == 0);
        }
        else {
            assert(cnt[-1] <= K);
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int sum = 0;
                for (int k = 0; k < N; ++k) {
                    sum += this->atCube(i, j, k);
                }
                assert(sum == 1);
            }
        }

        for (int i = 0; i < N; ++i) {
            for (int k = 0; k < N; ++k) {
                int sum = 0;
                for (int j = 0; j < N; ++j) {
                    sum += this->atCube(i, j, k);
                }
                assert(sum == 1);
            }
        }

        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                int sum = 0;
                for (int i = 0; i < N; ++i) {
                    sum += this->atCube(i, j, k);
                }
                assert(sum == 1);
            }
        }
    }

    void debugPrint() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::string s;
                for (int k = 0; k < N; ++k) {
                    if (this->cubeArray[i][j][k] == 1) {
                        if (not s.empty()) s += "+";
                        s.push_back(char('a' + k));
                    }
                    else if (this->cubeArray[i][j][k] == -1) {
                        s += "-";
                        s.push_back(char('a' + k));
                    }
                }
                std::cout << std::setw(2*K) << s << " ";
            }
            std::cout << std::endl;
        }
    }   
};


template<int N>
struct CubeHash {
    std::size_t operator()(const std::array<std::array<std::array<int,N>,N>,N>& arr) const {
        const long long M = 9'949'370'777'987'917ll;
        const long long P = 13;
        static_assert(2.0*M*P < 1e18);

        long long H = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    H += arr[i][j][k];
                    H *= P;
                    H %= M;
                }
            }
        }
        return std::size_t((H + M) % M);
    }
};


template<int N, int K>
struct LatinSquareAsCubeHash {
    std::size_t operator()(const LatinSquareAsCube<N,K>& ls) const {
        return CubeHash<N>()(ls.cubeArray);
    }
};

template<int N, int K>
bool isValidPlusMinusMove(LatinSquareAsCube<N, K>& lsc, Point3 pos1, Point3 pos2) {
    applyPlusMinusMove(lsc, pos1, pos2, +1);

    bool ok = lsc.negativeCnt <= K
            and inRange(-1, lsc.atCube(pos1.x, pos1.y, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos1.x, pos1.y, pos2.z), 1)
            and inRange(-1, lsc.atCube(pos1.x, pos2.y, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos2.x, pos1.y, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos1.x, pos2.y, pos2.z), 1)
            and inRange(-1, lsc.atCube(pos2.x, pos1.y, pos2.z), 1)
            and inRange(-1, lsc.atCube(pos2.x, pos2.y, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos2.x, pos2.y, pos2.z), 1);

    // undo
    applyPlusMinusMove(lsc, pos1, pos2, -1);

    return ok;
}


template<int N, int K>
void applyPlusMinusMove(LatinSquareAsCube<N, K>& lsc, Point3 pos1, Point3 pos2, int value) {
    lsc.cubeAdd(pos1.x, pos1.y, pos1.z, +value); // ++
    lsc.cubeAdd(pos1.x, pos1.y, pos2.z, -value); // --
    lsc.cubeAdd(pos1.x, pos2.y, pos1.z, -value); // --
    lsc.cubeAdd(pos2.x, pos1.y, pos1.z, -value); // --
    lsc.cubeAdd(pos1.x, pos2.y, pos2.z, +value); // ++
    lsc.cubeAdd(pos2.x, pos1.y, pos2.z, +value); // ++
    lsc.cubeAdd(pos2.x, pos2.y, pos1.z, +value); // ++
    lsc.cubeAdd(pos2.x, pos2.y, pos2.z, -value); // --
}


template<int N, int K>
void applyJacobsonMatthewsMove(LatinSquareAsCube<N, K>& lsc, Point3 pos1, Point3 pos2) {
    applyPlusMinusMove(lsc, pos1, pos2, +1);
}


template<int N, int K>
void makeJacobsonMatthewsMove(LatinSquareAsCube<N, K>& lsc) {
    // TODO use K!!!
    std::vector<Point3> initial_points = lsc.positionsWithValue(-1);
    if (lsc.negativeCnt < K) {
        initial_points = mergeVectors(initial_points, lsc.positionsWithValue(0));
    }
    std::vector<std::pair<Point3,Point3>> candidates;
    for (const Point3& p : initial_points) {
        for (int x : lsc.valueInLine(1, -1, p.y, p.z)) {
            for (int y : lsc.valueInLine(1, p.x, -1, p.z)) {
                for (int z : lsc.valueInLine(1, p.x, p.y, -1)) {
                    Point3 p2 = Point3(x, y, z);
                    if (isValidPlusMinusMove(lsc, p, p2)) {
                        candidates.push_back({p, p2});
                    }
                }
            }
        }
    }
    // std::cout << std::endl; lsc.debugPrint();
    if (candidates.empty()) {
        // lsc.debugPrint();
        std::cout << "$# " << lsc.negativeCnt << " " << (lsc.negativeCnt < K) << " " 
            << initial_points.size() << std::endl;
    }
    assert(not candidates.empty());
    auto [pos1, pos2] = sampleUniformly(candidates);
    applyJacobsonMatthewsMove(lsc, pos1, pos2);

#ifdef DEBUG_LATIN_SQUARE
    lsc.debugCheck();
#endif 
}


template<int N, int K>
void makeGeneralMove(LatinSquareAsCube<N, K>& lsc) {
    // TODO use K!!!
    std::vector<Point3> initial_points = lsc.positionsWithValue(-1);
    if (lsc.negativeCnt < K) {
        initial_points = mergeVectors(initial_points, lsc.positionsWithValue(0));
    }
    std::vector<std::pair<Point3,Point3>> candidates;
    for (const Point3& p : initial_points) {
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                for (int z = 0; z < N; ++z) {
                    Point3 p2 = Point3(x, y, z);
                    if (isValidPlusMinusMove(lsc, p, p2)) {
                        candidates.push_back({p, p2});
                    }
                }
            }
        }
    }
    // std::cout << std::endl; lsc.debugPrint();
    if (candidates.empty()) {
        // lsc.debugPrint();
        std::cout << "$# " << (lsc.negativeCnt < K) << " " 
            << initial_points.size() << std::endl;
    }
    assert(not candidates.empty());
    auto [pos1, pos2] = sampleUniformly(candidates);
    applyJacobsonMatthewsMove(lsc, pos1, pos2);

#ifdef DEBUG_LATIN_SQUARE
    lsc.debugCheck();
#endif 
}


template<int N, int K>
LatinSquare<N> sampleJacobsonMatthews(int moves) {
    assert(moves >= 0);

    LatinSquareAsCube<N, K> lsc(LatinSquare<N>::cyclicLatinSquare());
    while (moves > 0 or (not lsc.proper())) {
        makeJacobsonMatthewsMove(lsc);
        --moves;
    }

    return lsc.toLatinSquare();
}

#endif