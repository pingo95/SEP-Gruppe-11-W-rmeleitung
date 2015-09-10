#include "testeriterativesolver.h"

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

    algorithms::CRS A1, tmp; A1.A1(testSize); A1 = algorithms::multCRSCRS(tmp.diag(tC),A1);
    algorithms::CRS eye; eye.eye(testSize);
    tmp = deltaT/(deltaX*deltaX) * A1;
    tmp = eye - tmp;
    rhs = algorithms::addQVectors(last,deltaT * heatSources);

    itSolver = new Jacobi;

    itSolver->solve(result,tmp,rhs);
    testString = algorithms::printQVector(result);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterIterativeSolver::testSolveGaussSeidel() {
    int testSize = 3;
    QString testString;

    QVector<double> result(testSize*testSize), rhs(testSize*testSize);
    algorithms::CRS matrix; matrix.eye(testSize);
    for(int i=0; i<rhs.size(); ++i) rhs[i] = (i+1) * (i+1);

    itSolver = new GaussSeidel;

    itSolver->solve(result,matrix,rhs);
    testString = algorithms::printQVector(result);
    text->setText(testString);
    return text;
}
