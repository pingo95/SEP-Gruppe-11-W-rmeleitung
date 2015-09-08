#ifndef JACOBI_H
#define JACOBI_H

#include "Iterativesolver.h"

namespace algorithms {

    class Jacobi : public IterativeSolver
    {
    public:
        Jacobi();

        void solve(QVector<double>& result, CRS const &matrix, QVector<double> const &rhs);

    private:
        double eps;
        int itCount;
    };

}

#endif // JACOBI_H
