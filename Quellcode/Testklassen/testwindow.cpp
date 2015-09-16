#include "Testwindow.h"

TestWindow::TestWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
}

TestWindow::~TestWindow()
{

}

void TestWindow::addMainTab(QString name) {
    subTabs.append(new QTabWidget(tabWidget));
    tabWidget->addTab(subTabs.last(),name);
}

void TestWindow::addSubTab(QWidget *newTab, QString name, int index)
{
    subTabs[index]->addTab(newTab,name);
}

QString algorithms::printCRS(algorithms::CRS const &matrix) {
    QString string;
    int kl, ku, il, iu;
    for(int i=0; i<matrix.size; ++i) {
        kl = matrix.getRowsNumElem(i);
        ku = matrix.getRowsNumElem(i+1);
        if(kl==ku) {
            for(int j=0; j<matrix.size; ++j) string.append(QString::number(0)+"\t");
        }
        else {
            il = matrix.index[kl];
            iu = matrix.index[ku-1];
            for(int j=0; j<matrix.size; ++j) {
                if(j >= il && j <= iu) {
                    if(j==matrix.index[kl]) {
                        string.append(QString::number(matrix.value[kl])+"\t");
                        ++kl;
                    }
                    else string.append(QString::number(0)+"\t");
                }
                else string.append(QString::number(0)+"\t");
            }
        }
        string.append("\n");
    }
    return string;
}

QString algorithms::printFull(QVector<QVector<double> > const & matrix) {
    QString string;

    for(int i=0; i<matrix.size(); ++i) {
        for(int j=0; j<matrix.size(); ++j)
            string.append(QString::number(matrix[i][j])+"\t");
        string.append("\n");
    }
    return string;
}


QString algorithms::printQVector(QVector<double> const &vector) {
    QString string;

    for(int i=0; i<vector.size(); ++i) string.append(QString::number(vector[i])+"\n");
    return string;
}
