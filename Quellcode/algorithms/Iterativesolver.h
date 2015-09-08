#ifndef ITERATIVESOLVER_H
#define ITERATIVESOLVER_H
#include <QVector>
#include "Crs.h"

namespace algorithms {

    class IterativeSolver
    {
    public:
        IterativeSolver();
        virtual ~IterativeSolver();

        virtual void solve(QVector<double>& result, CRS const &matrix, QVector<double> const &rhs)=0;
    };

}

#endif // ITERATIVESOLVER_H
