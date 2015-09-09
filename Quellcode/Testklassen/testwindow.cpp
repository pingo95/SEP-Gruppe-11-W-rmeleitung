#include "testwindow.h"

TestWindow::TestWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
}

TestWindow::~TestWindow()
{

}

void TestWindow::addNewTab(QLabel* text, QString name) {
    tabWidget->addTab(text,name);
}
