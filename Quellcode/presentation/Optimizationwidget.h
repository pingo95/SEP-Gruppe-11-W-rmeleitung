#ifndef OPTIMIZATIONWIDGET_H
#define OPTIMIZATIONWIDGET_H
#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QSpinBox>

namespace presentation {

    class OptimizationWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit OptimizationWidget(QWidget *parent = 0);

    signals:

    public slots:


    //Attribute
    private:
        //Tabs
        QTabWidget * tabWidget;

        //Widgets
        QWidget * widgetCentral;
        QWidget * widgetData;
        QWidget * widgetSolution;
        QWidget * widgetConfiguration;

        //Layout
        QGridLayout * layoutDataTab;
        QGridLayout * layoutSolutionTab;
        QGridLayout * layoutConfigurationTab;

        //Buttons
        QPushButton * buttonLoad;
        QPushButton * buttonOptimization;

        //Labels
        QLabel * labelData;
        QLabel * labelSolution;
        QLabel * labelConfiguration;

        //Table
        QTableWidget * tableWidgetData;
        QTableWidget * tableWidgetSolution;

        //CheckBox
        QCheckBox * checkBoxN;
        QCheckBox * checkBoxHeatSources;
        QCheckBox * checkBoxThermalConductivities;
    };

}


#endif // OPTIMIZATIONWIDGET_H
