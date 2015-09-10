#include "testwindow.h"
#include <QApplication>

#include "testercrs.h"
#include "testeriterativesolver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow w;

    QLabel* text;

    algorithms::TesterCRS testerCRS;
    algorithms::TesterIterativeSolver testerIterativeSolver;



    text = testerCRS.testMultCRSCRS();
    w.addNewTab(text,"TesterCRS");
    text = testerIterativeSolver.testSolveJacobi();
    w.addNewTab(text,"TesterIterativeSolver");

    w.show();

    return a.exec();
}
