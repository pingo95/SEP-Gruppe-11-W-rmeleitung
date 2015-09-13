#ifndef TESTERITERATIVESOLVER_H
#define TESTERITERATIVESOLVER_H

#include <QLabel>

#include <../algorithms/Iterativesolver.h>
#include "../algorithms/Jacobi.h"
#include "../algorithms/Gaussseidel.h"

#include "Testwindow.h"

namespace algorithms {

    class TesterIterativeSolver
    {
    public:
        TesterIterativeSolver();

        QLabel* testSolveJacobi();
        QLabel* testSolveGaussSeidel();

    private:
        IterativeSolver* itSolver;
    };

}

#endif // TESTERITERATIVESOLVER_H
