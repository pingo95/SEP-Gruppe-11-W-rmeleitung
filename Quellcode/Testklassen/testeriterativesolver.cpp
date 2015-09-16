#include "Testeriterativesolver.h"

algorithms::TesterIterativeSolver::TesterIterativeSolver() {

}

QLabel* algorithms::TesterIterativeSolver::testSolveGaussSeidel() {
    QLabel* text;
    text = new QLabel;
    int testSize = 3;
    QString testString;
    double deltaT = .2;

    QVector<double> heatSources(testSize), last(testSize), result(testSize), rhs(testSize);
    for(int i=0; i<rhs.size(); ++i) {
        heatSources[i] = i;
        last[i] = 5;
        result[i] = 0;
    }

    algorithms::CRS matrix;
    matrix.size = 3;
    matrix.ptr.resize(matrix.size+1); matrix.index.resize(matrix.size*matrix.size); matrix.value.resize(matrix.size*matrix.size);
    for(int i=0; i<matrix.size*matrix.size; ++i) {
        matrix.index[i] = i%3;
    }
    matrix.ptr[0] = 0;
    matrix.value[0]=8.5;
    matrix.value[1]=-2.125;
    matrix.value[2]=-2.2;
    matrix.ptr[1] = 3;
    matrix.value[3]=-4.65;
    matrix.value[4]=12.5;
    matrix.value[5]=-1.132;
    matrix.ptr[2] = 6;
    matrix.value[6]=-0.5;
    matrix.value[7]=-2.235;
    matrix.value[8]=11;
    matrix.ptr[3] = 9;
    rhs = algorithms::addQVectors(last,deltaT * heatSources);

    itSolver = new GaussSeidel;

    itSolver->solve(result,matrix,rhs);
    testString = algorithms::printQVector(result);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterIterativeSolver::testSolveJacobi() {
    QLabel* text;
    text = new QLabel;
    int testSize = 3;
    QString testString;
    double deltaT = .2;

    QVector<double> heatSources(testSize), last(testSize), result(testSize), rhs(testSize);
    for(int i=0; i<rhs.size(); ++i) {
        heatSources[i] = i;
        last[i] = 5;
        result[i] = 0;
    }

    algorithms::CRS matrix;
    matrix.ptr.resize(4); matrix.index.resize(9); matrix.value.resize(9);
    for(int i=0; i<9; ++i) {
        matrix.index[i] = i%3;
    }
    matrix.ptr[0] = 0;
    matrix.value[0]=8.5;
    matrix.value[1]=-2.125;
    matrix.value[2]=-2.2;
    matrix.ptr[1] = 3;
    matrix.value[3]=-4.65;
    matrix.value[4]=12.5;
    matrix.value[5]=-1.132;
    matrix.ptr[2] = 6;
    matrix.value[6]=-0.5;
    matrix.value[7]=-2.235;
    matrix.value[8]=11;
    matrix.ptr[3] = 9;
    matrix.size = 3;
    rhs = algorithms::addQVectors(last,deltaT * heatSources);

    itSolver = new Jacobi;

    itSolver->solve(result,matrix,rhs);
    testString = algorithms::printQVector(result);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterIterativeSolver::testSolveLU() {
    QLabel* text;
    text = new QLabel;
    int testSize = 3;
    QString testString;
    double deltaT = .2;

    QVector<double> heatSources(testSize), last(testSize), result(testSize), rhs(testSize);
    for(int i=0; i<rhs.size(); ++i) {
        heatSources[i] = i;
        last[i] = 5;
        result[i] = 0;
    }

    algorithms::CRS matrix;
    matrix.size = 3;
    matrix.ptr.resize(matrix.size+1); matrix.index.resize(matrix.size*matrix.size); matrix.value.resize(matrix.size*matrix.size);
    for(int i=0; i<matrix.size*matrix.size; ++i) {
        matrix.index[i] = i%3;
    }
    matrix.ptr[0] = 0;
    matrix.value[0]=8.5;
    matrix.value[1]=-2.125;
    matrix.value[2]=-2.2;
    matrix.ptr[1] = 3;
    matrix.value[3]=-4.65;
    matrix.value[4]=12.5;
    matrix.value[5]=-1.132;
    matrix.ptr[2] = 6;
    matrix.value[6]=-0.5;
    matrix.value[7]=-2.235;
    matrix.value[8]=11;
    matrix.ptr[3] = 9;
    rhs = algorithms::addQVectors(last,deltaT * heatSources);

    itSolver = new LU;
    itSolver->decompose(matrix);

    itSolver->solve(result,matrix,rhs);
    testString = algorithms::printQVector(result);
    text->setText(testString);
    return text;
}
