#ifndef ITERATIVESOLVER_H
#define ITERATIVESOLVER_H

#include <QVector>
#include "Crs.h"
#include <math.h>

namespace algorithms {

    class IterativeSolver
    {
    public:
        IterativeSolver();
        virtual ~IterativeSolver();

        virtual void solve(QVector<double>& result, CRS const &matrix, QVector<double> const &rhs)=0;

    protected:
        double eps;
        int itCount;
    };

    double norm2(QVector<double> const &vec);

}

#endif // ITERATIVESOLVER_H
