#ifndef OPTIMIZATIONWIDGET_H
#define OPTIMIZATIONWIDGET_H
#include <QTabWidget>
#include <QGridLayout>
#include <QVBoxLayout>
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

    class OptimizationWidget : public QTabWidget
    {
        Q_OBJECT
    public:
        explicit OptimizationWidget(QWidget *parent = 0);

        double const MaxTemperature;

    signals:

    public slots:


    //Attribute
    private:
        //Widgets
        QWidget * widgetSolution;
        QWidget * widgetConfiguration;

        //Layouts
        QVBoxLayout * layoutSolutionTab;
        QGridLayout * layoutConfigurationTab;
        QGridLayout * gridBoxUserSettings;
        QGridLayout * gridBoxSettings;

        //Group Boxen
        QGroupBox * groupBoxUserSettings;
        QGroupBox * groupBoxSettings;

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
