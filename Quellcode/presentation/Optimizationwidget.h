#ifndef OPTIMIZATIONWIDGET_H
#define OPTIMIZATIONWIDGET_H

#include <QTabWidget>

#include "../model/Model.h"

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
#include <QLineEdit>


namespace presentation {
class Controller;

    class OptimizationWidget : public QTabWidget
    {
        Q_OBJECT

    //Funktionen:
    public:
        explicit OptimizationWidget(QWidget *parent = 0);
//        ~OptimizationWidget();

        void setController(Controller * controller);
        void setModel(model::Model * model);

        void update();

        // Slots in spe
        void nextStage(QString stage, int maximum);
        void updateProgress(int step);

    signals:
        void subTabChange(int targetTab);

    public slots:

    private slots:
       void transformTabIDSlot(int targetTab);

    private:
        explicit OptimizationWidget();

    //Attribute
    private:
        enum SubTab{
            TabConfiguration = 0,
            TabSolution = 1
        };

        int activeSubTab;
        Controller * controller;
        model::Model * model;

        // Config Tab
        QWidget * configurationTab;

        QGridLayout * configurationTabLayout;

        QLabel * topLabelConfiguration;

        QPushButton * loadDataButton;
        QPushButton * startOptimizationButton;

        QGroupBox * boxOverride;
        QGridLayout * boxOverrideLayout;
        QCheckBox * overrideHeatSources;
        QCheckBox * overrideThermalDiffusivities;
        QLabel * labelInitialValue;
        QDoubleSpinBox * inputInitialValue;

        QGroupBox * boxSettings;
        QGridLayout * boxSettingsLayout;
        QLabel * labelSettings;
        QLabel * labelIntMethod;
        QLineEdit * displayIntMethod;
        QLabel * labelM;
        QSpinBox * displayM;
        QLabel * labelT;
        QDoubleSpinBox * displayT;
        QLabel * labelSolver;
        QLineEdit * displaySolver;
        QLabel * labelMaxError;
        QDoubleSpinBox * displayMaxError;
        QLabel * labelMaxIt;
        QSpinBox * displayMaxIt;

        QCustomPlot * plate;
        QCPColorMap * colorMap;
        QCPColorScale * colorScale;

        QLabel * labelData;
        QTableWidget * dataTable;

        QLabel * labelProgressBar;
        QProgressBar * progressBar;

        QSpacerItem * spacerItem;

        // Solution Tab
        QWidget * solutionTab;
        QVBoxLayout * solutionTabLayout;
        QLabel * topLabelSolution;
        QTableWidget * solutionTable;
    };

}


#endif // OPTIMIZATIONWIDGET_H
