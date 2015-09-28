#define ___TESTING

#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QGridLayout>

#include "../algorithms/Crs.hpp"

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    TestWindow(QWidget *parent = 0);
    ~TestWindow();

    void addMainTab(QString name);
    void addSubTab(QWidget *newTab, QString name, int index);

private:
    QWidget* centralWidget;
    QHBoxLayout* HLayout;
    QVBoxLayout* mainLayout;
    QGridLayout* subGridLayout;
    QTabWidget* tabWidget;
    QVector<QTabWidget*> subTabs;
};

namespace algorithms {
    template<class U>
    QString printCRS(CRS<U> const & matrix);
    QString printFull(QVector<QVector<double> > const & matrix);
    QString printQVector(QVector<double> const & vector);
}

#endif // TESTWINDOW_H
