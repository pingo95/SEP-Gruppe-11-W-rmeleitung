#ifndef TESTERAREA_H
#define TESTERAREA_H
#include <QWidget>
#include <QTabWidget>
#include <QVector>
#include <QGridLayout>
#include "../presentation/Qcustomplot.h"
#include <QTextEdit>

class TesterArea
{
public:
    TesterArea();
    ~TesterArea();

    QWidget *robustnessWorstCaseTesterInsidePoint();
private :
    QTabWidget * tabs;
    QVector<QWidget *> ptr;
    QVector<QGridLayout *> grid;
    QVector<QCustomPlot *> plot;
    QVector<QTextEdit *> log;
};

#endif // TESTERAREA_H
