#ifndef ITERATIVESOLVER_H
#define ITERATIVESOLVER_H

namespace algorithms {

    class IterativeSolver
    {
    public:
        IterativeSolver();

        virtual void solve(QVector<double>& result, CRS& matrix, Qvector<double>& rhs)=0;
    };

}

#endif // ITERATIVESOLVER_H
