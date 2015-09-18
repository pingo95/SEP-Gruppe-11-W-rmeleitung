#ifndef SOLVER_H
#define SOLVER_H

#include <QVector>

#include <math.h>
#include <cassert>
#include "Crs.h"

namespace algorithms {

    class Solver {
    //Funktionen:
    public:
        Solver();
        virtual ~Solver();

        virtual void decompose(CRS const & matrix);
        virtual double getEps() const;
        virtual int getItCount() const;
        virtual int getMaxIt() const;
        virtual void setEps(double const eps);
        virtual void setMaxIt(int const maxIt);
        virtual void solve(QVector<double> & result, CRS const & matrix, QVector<double> const & rhs)=0;

    //Attribute:
    protected:
        double eps;
        int itCount;
        int maxIt;
    };

    double norm2(QVector<double> const & vec);
    double normInf(QVector<double> const & vec);

}

#endif // SOLVER_H
