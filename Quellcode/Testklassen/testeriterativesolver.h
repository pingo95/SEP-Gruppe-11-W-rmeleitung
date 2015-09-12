#ifndef TESTERITERATIVESOLVER_H
#define TESTERITERATIVESOLVER_H

#include <QLabel>

#include <../algorithms/Iterativesolver.h>
#include "Testwindow.h"
#include "../algorithms/Jacobi.h"
#include "../algorithms/Gaussseidel.h"

namespace algorithms {

    class TesterIterativeSolver
    {
    public:
        TesterIterativeSolver();

        QLabel* testSolveJacobi();
        QLabel* testSolveGaussSeidel();

    private:
        IterativeSolver* itSolver;
        QLabel* text;
    };

}

#endif // TESTERITERATIVESOLVER_H
