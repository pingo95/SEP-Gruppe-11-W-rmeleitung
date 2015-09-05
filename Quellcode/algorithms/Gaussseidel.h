#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H
#include "Iterativesolver.h"

namespace algorithms {

    class GaussSeidel : public IterativeSolver
    {
    public:
        GaussSeidel();

        void solve(QVector<double>& result, CRS& matrix, QVector<double>& rhs);
    };

}

#endif // GAUSSSEIDEL_H
