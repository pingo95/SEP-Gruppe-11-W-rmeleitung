#ifndef TESTERAREA_H
#define TESTERAREA_H
#include <QWidget>
#include <QTabWidget>
#include <QVector>
#include <QGridLayout>
#include <QTextEdit>

#include "../presentation/Qcustomplot.h"
#include "../model/Area.h"

namespace model {

    class TesterArea
    {
    public:
        TesterArea();
        ~TesterArea();

        void rotateKoords(QVector<double> & x, QVector<double> & y, int angle);
        QWidget *robustnessWorstCaseTesterInsidePoint();
        QWidget *robustnessWorstCaseTesterOnLine();
        QWidget *specialSegIntersectTester();
    private :
        QTabWidget * tabs;
        QVector<QWidget *> ptrs;
        QVector<QGridLayout *> grids;
        QVector<QCustomPlot *> plots;
        QVector<QTextEdit *> logs;
    };

}

#endif // TESTERAREA_H
