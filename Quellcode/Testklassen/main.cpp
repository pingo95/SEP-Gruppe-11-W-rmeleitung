#include "testwindow.h"
#include <QApplication>
#include "../model/Model.h"
#include "testercrs.h"
#include "testeriterativesolver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestWindow w;
    algorithms::TesterCRS testerCRS;
    algorithms::TesterIterativeSolver testerIterativeSolver;
    QLabel* text;

    text = testerCRS.print();
    w.addNewTab(text,"TesterCRS");
    text = testerIterativeSolver.print();
    w.addNewTab(text,"TesterIterativeSolver");

    w.show();

    return a.exec();
}
