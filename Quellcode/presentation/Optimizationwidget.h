#ifndef OPTIMIZATIONWIDGET_H
#define OPTIMIZATIONWIDGET_H
#include <QTabWidget>

#include <QCheckBox>
#include "Qcustomplot.h"
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>


namespace presentation {

    class OptimizationWidget : public QTabWidget
    {
        Q_OBJECT
    public:
        explicit OptimizationWidget(QWidget *parent = 0);

        double const MaxTemperature;

    signals:

    public slots:

    private:
        explicit OptimizationWidget();
        //~OptimizationWidget();

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
        QCheckBox * checkBoxThermalDiffusivities;

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

        //SpacerItem
        QSpacerItem * spacerItem;
    };

}


#endif // OPTIMIZATIONWIDGET_H
