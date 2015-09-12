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

    QWidget * crsTest1 = testerCRS.testSumQVector();
    QWidget * solverTest1 = testerIterativeSolver.testSolveJacobi();
    QWidget * areaTest1 = testerArea.robustnessWorstCaseTesterInsidePoint();

    w.addNewTab(crsTest1,"TesterCRS");
    w.addNewTab(solverTest1,"TesterIterativeSolver");
    w.addNewTab(areaTest1,"InsidePointTest1");

    w.show();

    int ret = a.exec();

//    delete crsTest1;
//    delete solverTest1;
//    delete areaTest1;

    return ret;
}
