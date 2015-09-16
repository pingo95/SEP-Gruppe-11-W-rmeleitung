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
#include <QGroupBox>
#include <QProgressBar>

#include "Qcustomplot.h"

namespace presentation {

    class OptimizationWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit OptimizationWidget(QWidget *parent = 0);

        double const MaxTemperature;

    signals:

    public slots:


    //Attribute
    private:
        //Tabs
        QTabWidget * tabWidget;

        //Widgets
        QWidget * widgetCentral;
        QWidget * widgetSolution;
        QWidget * widgetConfiguration;

        //Layout
        QGridLayout * layoutDataTab;
        QGridLayout * layoutSolutionTab;
        QGridLayout * layoutConfigurationTab;
        QGridLayout * layoutWidgetCentral;

        //Buttons
        QPushButton * buttonLoad;
        QPushButton * buttonOptimization;

        //Labels
        QLabel * labelData;
        QLabel * labelSolution;
        QLabel * labelConfiguration;
        QLabel * labelSpinBoxN;
        QLabel * labelInitialValue;
        QLabel * labelSettings;
        QLabel * labelM;
        QLabel * labelT;
        QLabel * labelEpsilon;
        QLabel * labelMaxIt;
        QLabel * labelProgressBar;

        //Table
        QTableWidget * tableWidgetData;
        QTableWidget * tableWidgetSolution;

        //CheckBox
        QCheckBox * checkBoxN;
        QCheckBox * checkBoxHeatSources;
        QCheckBox * checkBoxThermalConductivities;

        //SpinBoxes
        QSpinBox * spinBoxN;
        QSpinBox * spinBoxM;
        QSpinBox * spinBoxMaxIt;

        //doubleSpinBoxes
        QDoubleSpinBox * doubleSpinBoxInitialValue;
        QDoubleSpinBox * doubleSpinBoxEpsilon;
        QDoubleSpinBox * doubleSpinBoxT;

        //Platte
        QCustomPlot * plateOptimization;
        QCPColorMap * colorMap;
        QCPColorScale * colorScale;

        //Progressbar
        QProgressBar * progressBar;

    };

}


#endif // OPTIMIZATIONWIDGET_H
