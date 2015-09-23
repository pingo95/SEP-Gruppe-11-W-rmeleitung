#ifndef ITERATIVESOLVER_H
#define ITERATIVESOLVER_H

#include "Solver.h"

namespace algorithms {

    template <class T>
    class IterativeSolver : public Solver<T> {
    public:
        IterativeSolver();
        virtual ~IterativeSolver();

        double getEps() const;
        int getItCount() const;
        int getMaxIt() const;
        void setEps(double const eps);
        void setMaxIt(int const maxIt);
        virtual void solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs)=0;
    };
}

#endif // ITERATIVESOLVER_H
