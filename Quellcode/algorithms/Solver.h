#ifndef SOLVER_H
#define SOLVER_H

#include <QVector>

#include <math.h>
#include <cassert>
#include "Crs.h"

namespace algorithms {

    template <class T>
    class Solver {
    //Funktionen:
    public:
        Solver();
        virtual ~Solver();

        virtual void decompose(CRS<T> const & matrix);
        virtual double getEps() const;
        virtual int getItCount() const;
        virtual int getMaxIt() const;
        virtual void setEps(double const eps);
        virtual void setMaxIt(int const maxIt);
        virtual void solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs)=0;

    //Attribute:
    protected:
        double eps;
        int itCount;
        int maxIt;
    };

    template <class T>
    double norm2(QVector<T> const & vec);

    template <class T>
    double normInf(QVector<T> const & vec);

}

#endif // SOLVER_H
