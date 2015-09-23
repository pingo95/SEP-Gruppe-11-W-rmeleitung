#ifndef GAUSS_H
#define GAUSS_H

#include <cassert>
#include "Solver.h"

namespace algorithms {

    template <class T>
    class LU : public Solver<T> {
    //Funktionen:
    public:
        LU();

        void decompose(CRS<T> const & matrix);
        void solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs);

    //Attribute:
    private:
        QVector<QVector<T> > L;
        QVector<QVector<T> > U;

    };
}

#endif // GAUSS_H
