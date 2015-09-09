#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QGridLayout>

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    TestWindow(QWidget *parent = 0);
    ~TestWindow();

    void addNewTab(QLabel* newTab,QString name);

private:
    QWidget* centralWidget;
    QHBoxLayout* HLayout;
    QVBoxLayout* mainLayout;
    QGridLayout* subGridLayout;
    QTabWidget* tabWidget;
};

#endif // TESTWINDOW_H
