#ifndef TESTERITERATIVESOLVER_H
#define TESTERITERATIVESOLVER_H

#include <QLabel>

#include <../algorithms/Iterativesolver.h>

namespace algorithms {

    class TesterIterativeSolver
    {
    public:
        TesterIterativeSolver();

        QLabel* print();

    private:
        IterativeSolver* itSolver;
        QLabel* text;
    };

}

#endif // TESTERITERATIVESOLVER_H
