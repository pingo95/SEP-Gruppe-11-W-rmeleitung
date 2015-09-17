#ifndef GAUSS_H
#define GAUSS_H

#include <cassert>
#include "Solver.h"

namespace algorithms {

    class LU : public Solver {
    public:
        LU();

        void decompose(CRS & matrix);
        void solve(QVector<double> & result, CRS & matrix, QVector<double> & rhs);

    private:
        QVector<QVector<double> > L;
        QVector<QVector<double> > U;

    };
}

#endif // GAUSS_H