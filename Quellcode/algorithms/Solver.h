#ifndef SOLVER_H
#define SOLVER_H

#include <QVector>

#include <math.h>
#include <cassert>
#include "Crs.h"

namespace algorithms {

    class Solver {
    public:
        Solver();
        virtual ~Solver();

        virtual void decompose(CRS & matrix);
        void equilibrate(CRS & matrix, QVector<double> & rhs);
        virtual double getEps() const;
        virtual int getItCount() const;
        virtual int getMaxIt() const;
        virtual void setEps(double const eps);
        virtual void setMaxIt(int const maxIt);
        virtual void solve(QVector<double> & result, CRS & matrix, QVector<double> & rhs)=0;

    protected:
        double eps;
        int itCount;
        int maxIt;
    };

    double norm2(QVector<double> const &vec);

}

#endif // SOLVER_H