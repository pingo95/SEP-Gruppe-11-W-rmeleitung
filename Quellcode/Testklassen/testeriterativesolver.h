#ifndef TESTERITERATIVESOLVER_H
#define TESTERITERATIVESOLVER_H

#include <QLabel>

#include <../algorithms/Solver.h>
#include "../algorithms/Jacobi.h"
#include "../algorithms/Gaussseidel.h"
#include "../algorithms/LU.h"

#include "Testwindow.h"

namespace algorithms {

    class TesterIterativeSolver
    {
    public:
        TesterIterativeSolver();

        QLabel* testSolveGaussSeidel();
        QLabel* testSolveJacobi();
        QLabel* testSolveLU();

    private:
        Solver<double>* itSolver;
    };

}

#endif // TESTERITERATIVESOLVER_H
