#ifndef __ONE_FACTORIZATION__
#define __ONE_FACTORIZATION__

#include "latin_square.hpp"

template<int N, int K>
class OneFactorizationAsCube : public LatinSquareAsCube<N, K> {
public:
    OneFactorizationAsCube(const LatinSquare<N>& ls) : LatinSquareAsCube<N, K>(ls) {
        assert(OneFactorizationAsCube::symmetric(ls));
    }

    static bool symmetric(const LatinSquare<N>& ls) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < i; ++j) {
                if (ls.table[i][j] != ls.table[j][i]) {
                    return false;
                }
            }
        }
        return true;
    }

    static bool validOneFactorization(const LatinSquare<N>& ls) {
        for (int i = 0; i < N; ++i) {
            if (ls.table[i][i] != N-1) return false;
        }
        return OneFactorizationAsCube<N,K>::symmetric(ls);
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
};


template<int N, int K>
struct OneFactorizationAsCubeHash {
    std::size_t operator()(const OneFactorizationAsCube<N,K>& ls) const {
        return CubeHash<N>()(ls.cubeArray);
    }
};


template<int N, int K>
void applySymmetricJacobsonMatthewsMove(LatinSquareAsCube<N, K>& lsc, Point3 pos1, Point3 pos2, int value) {
    lsc.cubeAdd(pos1.x, pos1.y, pos1.z, +value); // ++
    lsc.cubeAdd(pos1.x, pos1.y, pos2.z, -value); // --
    lsc.cubeAdd(pos1.x, pos2.y, pos1.z, -value); // --
    lsc.cubeAdd(pos2.x, pos1.y, pos1.z, -value); // --
    lsc.cubeAdd(pos1.x, pos2.y, pos2.z, +value); // ++
    lsc.cubeAdd(pos2.x, pos1.y, pos2.z, +value); // ++
    lsc.cubeAdd(pos2.x, pos2.y, pos1.z, +value); // ++
    lsc.cubeAdd(pos2.x, pos2.y, pos2.z, -value); // --

    lsc.cubeAdd(pos1.y, pos1.x, pos1.z, +value); // ++
    lsc.cubeAdd(pos1.y, pos1.x, pos2.z, -value); // --
    lsc.cubeAdd(pos2.y, pos1.x, pos1.z, -value); // --
    lsc.cubeAdd(pos1.y, pos2.x, pos1.z, -value); // --
    lsc.cubeAdd(pos2.y, pos1.x, pos2.z, +value); // ++
    lsc.cubeAdd(pos1.y, pos2.x, pos2.z, +value); // ++
    lsc.cubeAdd(pos2.y, pos2.x, pos1.z, +value); // ++
    lsc.cubeAdd(pos2.y, pos2.x, pos2.z, -value); // --
}


template<int N, int K>
bool isValidSymmetricJacobsonMatthewsMove(LatinSquareAsCube<N, K>& lsc, Point3 pos1, Point3 pos2) {
    applySymmetricJacobsonMatthewsMove(lsc, pos1, pos2, +1);

    bool ok = lsc.negativeCnt <= K
            and inRange(-1, lsc.atCube(pos1.x, pos1.y, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos1.x, pos1.y, pos2.z), 1)
            and inRange(-1, lsc.atCube(pos1.x, pos2.y, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos2.x, pos1.y, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos1.x, pos2.y, pos2.z), 1)
            and inRange(-1, lsc.atCube(pos2.x, pos1.y, pos2.z), 1)
            and inRange(-1, lsc.atCube(pos2.x, pos2.y, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos2.x, pos2.y, pos2.z), 1)

            and inRange(-1, lsc.atCube(pos1.y, pos1.x, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos1.y, pos1.x, pos2.z), 1)
            and inRange(-1, lsc.atCube(pos2.y, pos1.x, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos1.y, pos2.x, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos2.y, pos1.x, pos2.z), 1)
            and inRange(-1, lsc.atCube(pos1.y, pos2.x, pos2.z), 1)
            and inRange(-1, lsc.atCube(pos2.y, pos2.x, pos1.z), 1)
            and inRange(-1, lsc.atCube(pos2.y, pos2.x, pos2.z), 1);

    // undo
    applySymmetricJacobsonMatthewsMove(lsc, pos1, pos2, -1);

    return ok;
}


template<int N, int K>
void makeSymmetricJacobsonMatthewsMove(LatinSquareAsCube<N, K>& lsc) {
    std::vector<Point3> initial_positions = lsc.positionsWithValue(-1);
    if (lsc.negativeCnt < K - 1) {
        initial_positions = mergeVectors(initial_positions, lsc.positionsWithValue(0));
    }
    std::vector<std::pair<Point3,Point3>> candidates;
    for (Point3 p : initial_positions) {
        if (p.x != p.y) {
            for (int x : lsc.valueInLine(1, -1, p.y, p.z)) {
                for (int y : lsc.valueInLine(1, p.x, -1, p.z)) {
                    for (int z : lsc.valueInLine(1, p.x, p.y, -1)) {
                        Point3 p2(x, y, z);
                        if (x != y and isValidSymmetricJacobsonMatthewsMove(lsc, p, p2)) {
                            candidates.push_back({p, p2});
                        }
                    }
                }
            }
        }
    }
    auto [pos1, pos2] = sampleUniformly(candidates);
    applySymmetricJacobsonMatthewsMove(lsc, pos1, pos2, +1);

#ifdef DEBUG_LATIN_SQUARE
    lsc.debugCheck();
#endif 
}


#endif
