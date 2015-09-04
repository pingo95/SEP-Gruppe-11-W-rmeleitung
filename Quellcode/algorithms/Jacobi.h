#ifndef JACOBI_H
#define JACOBI_H
#include "Iterativesolver.h"

namespace algorithms {

    class Jacobi : public IterativeSolver
    {
    public:
        Jacobi();

        void solve(QVector<double>& result, CRS& matrix, Qvector<double>& rhs);
    };

}

#endif // JACOBI_H
