#include "Testeriterativesolver.h"

algorithms::TesterIterativeSolver::TesterIterativeSolver() {
    text = new QLabel();
}

QLabel* algorithms::TesterIterativeSolver::testSolveJacobi() {
    int testSize = 3;
    QString testString;
    double deltaT = .2;
    double deltaX = .3;

    QVector<double> heatSources(testSize*testSize), last(testSize*testSize), result(testSize*testSize), rhs(testSize*testSize), tC(testSize*testSize);
    for(int i=0; i<rhs.size(); ++i) {
        heatSources[i] = i;
        last[i] = 5;
        result[i] = 0;
        tC[i] = (i+1)*10;
    }

    algorithms::CRS A1, diag; A1.A1(testSize); diag.diag(tC); diag = diag.multCRSCRS(A1);
    algorithms::CRS eye; eye.eye(testSize);
    diag = deltaT/(deltaX*deltaX) * diag;
    diag = eye - diag;
    rhs = algorithms::addQVectors(last,deltaT * heatSources);

    itSolver = new Jacobi;

    itSolver->solve(result,diag,rhs);
    testString = algorithms::printQVector(result);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterIterativeSolver::testSolveGaussSeidel() {
    int testSize = 3;
    QString testString;
    double deltaT = .2;
    double deltaX = .3;

    QVector<double> heatSources(testSize*testSize), last(testSize*testSize), result(testSize*testSize), rhs(testSize*testSize), tC(testSize*testSize);
    for(int i=0; i<rhs.size(); ++i) {
        heatSources[i] = i;
        last[i] = 5;
        result[i] = 0;
        tC[i] = (i+1)*10;
    }

    algorithms::CRS A1, diag; A1.A1(testSize); diag.diag(tC); diag = diag.multCRSCRS(A1);
    algorithms::CRS eye; eye.eye(testSize);
    diag = deltaT/(deltaX*deltaX) * diag;
    diag = eye - diag;
    rhs = algorithms::addQVectors(last,deltaT * heatSources);

    itSolver = new Jacobi;

    itSolver->solve(result,diag,rhs);
    testString = algorithms::printQVector(result);
    text->setText(testString);
    return text;
}
