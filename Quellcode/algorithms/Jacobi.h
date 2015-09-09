#ifndef JACOBI_H
#define JACOBI_H

#include "Iterativesolver.h"

namespace algorithms {

    class Jacobi : public IterativeSolver
    {
    public:
        Jacobi();

        void solve(QVector<double>& result, CRS const &matrix, QVector<double> const &rhs);
    };

}

#endif // JACOBI_H
