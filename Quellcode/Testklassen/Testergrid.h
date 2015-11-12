#ifndef TESTERGRID_H
#define TESTERGRID_H
#include <QWidget>
#include <QTabWidget>
#include <QVector>
#include <QGridLayout>
#include <QTableWidget>
#include <QTextEdit>

#include "../presentation/Qcustomplot.h"
#include "../model/Area.h"

namespace model {

    class TesterGrid
    {
    public:
        TesterGrid();
        ~TesterGrid();

        QWidget * areasToGridTest1();
    private:
        QTabWidget * tabs;
        QVector<QWidget *> ptrs;
        QVector<QGridLayout *> grids;
        QVector<QCustomPlot *> plots;
        QVector<QTableWidget *> tables;
        QVector<QTextEdit *> logs;
        QVector<QCPColorScale *> scales;
    };

}

#endif // TESTERGRID_H
