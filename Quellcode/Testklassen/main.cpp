#include "Testwindow.h"
#include <QApplication>

#include "Testercrs.h"
#include "Testeriterativesolver.h"
#include "Testerarea.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow w;

    algorithms::TesterCRS testerCRS;
    algorithms::TesterIterativeSolver testerIterativeSolver;
    TesterArea testerArea;

    int mainTabCount = -1;

    w.addMainTab("CRS Tests");
    ++mainTabCount;

    QWidget * crsTest1 = testerCRS.testA1();
    QWidget * crsTest2 = testerCRS.testDiag();
    QWidget * crsTest3 = testerCRS.testDiffCRS();
    QWidget * crsTest4 = testerCRS.testEye();
    QWidget * crsTest5 = testerCRS.testMultCRSCRS();
    QWidget * crsTest6 = testerCRS.testMultCRSQVector();
    QWidget * crsTest7 = testerCRS.testMultRowQVector();
    QWidget * crsTest8 = testerCRS.testScalarCRS();
    QWidget * crsTest9 = testerCRS.testScalarQVector();
    QWidget * crsTest10 = testerCRS.testSumCRS();
    QWidget * crsTest11 = testerCRS.testSumQVector();

    w.addSubTab(crsTest1,"A1",mainTabCount);
    w.addSubTab(crsTest2,"Diag",mainTabCount);
    w.addSubTab(crsTest3,"DiffCRS",mainTabCount);
    w.addSubTab(crsTest4,"Eye",mainTabCount);
    w.addSubTab(crsTest5,"MultCRSCRS",mainTabCount);
    w.addSubTab(crsTest6,"MultCRSQVector",mainTabCount);
    w.addSubTab(crsTest7,"MultRowQVector",mainTabCount);
    w.addSubTab(crsTest8,"ScalarCRS",mainTabCount);
    w.addSubTab(crsTest9,"ScalarQVector",mainTabCount);
    w.addSubTab(crsTest10,"SumCRS",mainTabCount);
    w.addSubTab(crsTest11,"SumQVector",mainTabCount);


    w.addMainTab("Solver Tests");
    ++mainTabCount;

    QWidget * solverTest1 = testerIterativeSolver.testSolveJacobi();
    QWidget * solverTest2 = testerIterativeSolver.testSolveGaussSeidel();

    w.addSubTab(solverTest1,"Jacobi",mainTabCount);
    w.addSubTab(solverTest2,"GaussSeidel",mainTabCount);

    w.addMainTab("Area Tests");
    ++mainTabCount;

    QWidget * areaTest1 = testerArea.robustnessWorstCaseTesterInsidePoint();
    QWidget * areaTest2 = testerArea.robustnessWorstCaseTesterOnLine();

    w.addSubTab(areaTest1,"InsidePointTest1",mainTabCount);
    w.addSubTab(areaTest2,"OnLineTest1",mainTabCount);

    w.show();

    int ret = a.exec();

//    delete crsTest1;
//    delete crsTest2;
//    delete crsTest3;
//    delete crsTest4;
//    delete crsTest5;
//    delete crsTest6;
//    delete crsTest7;
//    delete crsTest8;
//    delete crsTest9;
//    delete crsTest10;
//    delete solverTest1;
//    delete solverTest2;
//    delete areaTest1;

    return ret;
}
