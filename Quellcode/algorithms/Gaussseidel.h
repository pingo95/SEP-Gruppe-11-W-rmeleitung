#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H

#include "Iterativesolver.h"

namespace algorithms {

    template <class T>
    class GaussSeidel : public IterativeSolver<T> {
    public:
        GaussSeidel();

        void solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs);
    };

}

#endif // GAUSSSEIDEL_H
