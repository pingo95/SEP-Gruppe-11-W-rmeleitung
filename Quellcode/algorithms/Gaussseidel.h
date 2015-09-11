#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H

#include "Iterativesolver.h"

namespace algorithms {

    class GaussSeidel : public IterativeSolver
    {
    public:
        GaussSeidel();

        void solve(QVector<double> &result, CRS const &matrix, QVector<double> const &rhs);
    };

}

#endif // GAUSSSEIDEL_H
