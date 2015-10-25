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
