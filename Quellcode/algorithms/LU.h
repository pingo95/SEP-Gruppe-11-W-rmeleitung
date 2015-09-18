#ifndef GAUSS_H
#define GAUSS_H

#include <cassert>
#include "Solver.h"

namespace algorithms {

    class LU : public Solver {
    //Funktionen:
    public:
        LU();

        void decompose(CRS const & matrix);
        void solve(QVector<double> & result, CRS const & matrix, QVector<double> const & rhs);

    //Attribute:
    private:
        QVector<QVector<double> > L;
        QVector<QVector<double> > U;

    };
}

#endif // GAUSS_H
