#ifndef TESTERAREA_H
#define TESTERAREA_H
#include <QWidget>
#include <QTabWidget>
#include <QVector>
#include <QGridLayout>
#include <QTextEdit>

#include "../presentation/Qcustomplot.h"
#include "../model/Area.h"

class TesterArea
{
public:
    TesterArea();
    ~TesterArea();

    void rotateKoords(QVector<double> & x, QVector<double> & y, int angle);
    QWidget *robustnessWorstCaseTesterInsidePoint();
    QWidget *robustnessWorstCaseTesterOnLine();
private :
    QTabWidget * tabs;
    QVector<QWidget *> ptr;
    QVector<QGridLayout *> grid;
    QVector<QCustomPlot *> plot;
    QVector<QTextEdit *> log;
};

#endif // TESTERAREA_H
