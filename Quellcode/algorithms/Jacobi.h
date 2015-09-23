#ifndef JACOBI_H
#define JACOBI_H

#include "Iterativesolver.h"

namespace algorithms {

    template <class T>
    class Jacobi : public IterativeSolver<T> {
    public:
        Jacobi();

        void solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs);
    };

}

#endif // JACOBI_H
