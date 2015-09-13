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

    QWidget * crsTest1 = testerCRS.testA1();
    QWidget * crsTest2 = testerCRS.testDiag();
    QWidget * crsTest3 = testerCRS.testDiffCRS();
    QWidget * crsTest4 = testerCRS.testEye();
    QWidget * crsTest5 = testerCRS.testMultCRSCRS();
    QWidget * crsTest6 = testerCRS.testMultCRSQVector();
    QWidget * crsTest7 = testerCRS.testScalarCRS();
    QWidget * crsTest8 = testerCRS.testScalarQVector();
    QWidget * crsTest9 = testerCRS.testSumCRS();
    QWidget * crsTest10 = testerCRS.testSumQVector();
    QWidget * solverTest1 = testerIterativeSolver.testSolveJacobi();
    QWidget * solverTest2 = testerIterativeSolver.testSolveGaussSeidel();
    QWidget * areaTest1 = testerArea.robustnessWorstCaseTesterInsidePoint();

    w.addNewTab(crsTest1,"A1");
    w.addNewTab(crsTest2,"Diag");
    w.addNewTab(crsTest3,"DiffCRS");
    w.addNewTab(crsTest4,"Eye");
    w.addNewTab(crsTest5,"MultCRSCRS");
    w.addNewTab(crsTest6,"MultCRSQVector");
    w.addNewTab(crsTest7,"ScalarCRS");
    w.addNewTab(crsTest8,"ScalarQVector");
    w.addNewTab(crsTest9,"SumCRS");
    w.addNewTab(crsTest10,"SumQVector");
    w.addNewTab(solverTest1,"Jacobi");
    w.addNewTab(solverTest2,"GaussSeidel");
    w.addNewTab(areaTest1,"InsidePointTest1");

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
