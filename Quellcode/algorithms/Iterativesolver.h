#ifndef ITERATIVESOLVER_H
#define ITERATIVESOLVER_H
#include <QVector>
class CRS;

namespace algorithms {

    class IterativeSolver
    {
    public:
        IterativeSolver();

        virtual void solve(QVector<double>& result, CRS& matrix, QVector<double>& rhs)=0;
    };

}

#endif // ITERATIVESOLVER_H
