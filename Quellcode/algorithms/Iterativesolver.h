#ifndef ITERATIVESOLVER_H
#define ITERATIVESOLVER_H

#include <math.h>
#include <cassert>

#include <QVector>

#include "Crs.h"

namespace algorithms {

    class IterativeSolver
    {
    public:
        IterativeSolver();
        virtual ~IterativeSolver();

        double getEps() const;
        int getItCount() const;
        int getMaxIt() const;
        void setEps(double eps);
        void setMaxIt(int maxIt);
        virtual void solve(QVector<double>& result, CRS const &matrix, QVector<double> const &rhs)=0;

    protected:
        double eps;
        int itCount;
        int maxIt;
    };

    double norm2(QVector<double> const &vec);

}

#endif // ITERATIVESOLVER_H
