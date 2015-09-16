#ifndef ITERATIVESOLVER_H
#define ITERATIVESOLVER_H

#include "Solver.h"

namespace algorithms {

    class IterativeSolver : public Solver {
    public:
        IterativeSolver();
        virtual ~IterativeSolver();

        double getEps() const;
        int getItCount() const;
        int getMaxIt() const;
        void setEps(double const eps);
        void setMaxIt(int const maxIt);
        virtual void solve(QVector<double> & result, CRS const & matrix, QVector<double> const & rhs)=0;
    };
}

#endif // ITERATIVESOLVER_H
