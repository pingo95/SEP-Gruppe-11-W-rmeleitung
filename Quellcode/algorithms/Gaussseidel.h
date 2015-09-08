#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H

#include "Intmethod.h"
#include "Iterativesolver.h"

namespace algorithms {

    class GaussSeidel : public IterativeSolver
    {
    public:
        GaussSeidel();

        void solve(QVector<double> &result, CRS const &matrix, QVector<double> const &rhs);

    private:
        double eps=1e-5;
        int itCount=0;
    };

}

#endif // GAUSSSEIDEL_H
