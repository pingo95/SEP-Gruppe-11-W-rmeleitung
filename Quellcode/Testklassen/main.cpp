#include "Testwindow.h"
#include <QApplication>
#include "Testercrs.h"
#include "Testeriterativesolver.h"
#include "Testerarea.h"
#include "Testergrid.h"
#include "Testerdco.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow w;

    algorithms::TesterCRS testerCRS;
    algorithms::TesterIterativeSolver testerIterativeSolver;
    model::TesterArea testerArea;
    model::TesterGrid testerGrid;
    model::TesterDCO testerDCO;

    int mainTabCount = -1;

    w.addMainTab("CRS Tests");
    ++mainTabCount;

    QWidget * crsTest1 = testerCRS.testA1();
    QWidget * crsTest2 = testerCRS.testDiag();
    QWidget * crsTest3 = testerCRS.testDiffCRS();
    QWidget * crsTest4 = testerCRS.testEye();
    QWidget * crsTest5 = testerCRS.testFull();
    QWidget * crsTest6 = testerCRS.testMultCRSCRS();
    QWidget * crsTest7 = testerCRS.testMultCRSQVector();
    QWidget * crsTest8 = testerCRS.testMultRowQVector();
    QWidget * crsTest9 = testerCRS.testMultRowQVectorAbs();
    QWidget * crsTest10 = testerCRS.testScalarCRS();
    QWidget * crsTest11 = testerCRS.testScalarQVector();
    QWidget * crsTest12 = testerCRS.testSumCRS();
    QWidget * crsTest13 = testerCRS.testSumQVector();

    w.addSubTab(crsTest1,"A1",mainTabCount);
    w.addSubTab(crsTest2,"Diag",mainTabCount);
    w.addSubTab(crsTest3,"DiffCRS",mainTabCount);
    w.addSubTab(crsTest4,"Eye",mainTabCount);
    w.addSubTab(crsTest5,"Full",mainTabCount);
    w.addSubTab(crsTest6,"MultCRSCRS",mainTabCount);
    w.addSubTab(crsTest7,"MultCRSQVector",mainTabCount);
    w.addSubTab(crsTest8,"MultRowQVector",mainTabCount);
    w.addSubTab(crsTest9,"MultRowQVectorAbs",mainTabCount);
    w.addSubTab(crsTest10,"ScalarCRS",mainTabCount);
    w.addSubTab(crsTest11,"ScalarQVector",mainTabCount);
    w.addSubTab(crsTest12,"SumCRS",mainTabCount);
    w.addSubTab(crsTest13,"SumQVector",mainTabCount);


    w.addMainTab("Solver Tests");
    ++mainTabCount;

    QWidget * solverTest1 = testerIterativeSolver.testSolveJacobi();
    QWidget * solverTest2 = testerIterativeSolver.testSolveGaussSeidel();
    QWidget * solverTest3 = testerIterativeSolver.testSolveLU();

    w.addSubTab(solverTest1,"Jacobi",mainTabCount);
    w.addSubTab(solverTest2,"GaussSeidel",mainTabCount);
    w.addSubTab(solverTest3,"LU",mainTabCount);

    w.addMainTab("Area Tests");
    ++mainTabCount;

    QWidget * areaTest1 = testerArea.robustnessWorstCaseTesterInsidePoint();
    QWidget * areaTest2 = testerArea.robustnessWorstCaseTesterOnLine();
    QWidget * areaTest3 = testerArea.specialSegIntersectTester();

    w.addSubTab(areaTest1,"InsidePointTest1",mainTabCount);
    w.addSubTab(areaTest2,"OnLineTest1",mainTabCount);
    w.addSubTab(areaTest3,"SegIntersectTest1",mainTabCount);

    w.addMainTab("Grid Tests");
    ++mainTabCount;

    QWidget * gridTest1 = testerGrid.areasToGridTest1();

    w.addSubTab(gridTest1,"AreasToGridTest1",mainTabCount);

    //w.addMainTab("DCO Tests");
    //++mainTabCount;

   // QWidget * dcoTest1 = testerDCO.constantLenghtTest();
   // QWidget * dcoTest2 = testerDCO.GoldsteinArmijoTest();

    //w.addSubTab(dcoTest1,"Konstante Schrittweite",mainTabCount);
    //w.addSubTab(dcoTest2,"Goldstein-Armijo",mainTabCount);

    w.show();

    return a.exec();
}
