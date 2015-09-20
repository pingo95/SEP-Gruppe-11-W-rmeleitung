#ifndef UI_H
#define UI_H

#include <QMainWindow>

#include <QTabWidget>

#include "Areawidget.h"
#include "Ibvswidget.h"
#include "Simulationwidget.h"
#include "Visualizationwidget.h"
#include "Optimizationwidget.h"
#include "Helpwidget.h"

namespace presentation {
class Controller;

    class UI : public QMainWindow
    {
        Q_OBJECT

    //Enum
    public:
        // TODO: IDs setzten
        enum ActiveTab
        {
            TabConfiguration = 0,
            TabThermalDiffusivities = 5,
            TabHeatSources = 6,
            TabIBVs = 7,
            TabSimulating = 1,
            TabVisualization = 2,
            TabParameterFitting = 3,
            TabHelp = 4
        };

    //Funktionen:
    public:
        UI(QWidget *parent = 0);
        ~UI();

        void revertTabChange(UI::ActiveTab targetTab);
        void setActiveTab(int targetTab);

        void setController(Controller * controller);
        void setModel(model::Model * model);

        void updateNotification();

        // Weiterleitungen
        void drawPartialArea(QVector<double> const & partialAreaX,
                             QVector<double> const & partialAreaY,
                             model::SimulationSetup::AreaType type);
        int getInitialFrame() const;
        void visualizeState(int const frame);

    signals:
        void subTabChange(int targetTab);

    public slots:
        // Weiterleitungen
        void appendToSimulationLogSlot(QString text);
        void nextSimulationStageSlot(QString stage, int maximum);
        void updateSimulationProgressSlot(int step);

    private slots:
        void transformTabIDSlot(int targetTab);

    //Attribute:
    private:
        int activeTab;
        Controller * controller;
        model::Model * model;
        int const tabMainCount;

        //Qt Elemente:
        QTabWidget * tabWidgetMain;
        QTabWidget * tabWidgetSub;

        AreaWidget * thermalDiffusivitiesTab;
        AreaWidget * heatSourcesTab;
        IBVsWidget * iBVsTab;
        SimulationWidget * simulationTab;
        OptimizationWidget * optimizationTab;
        VisualizationWidget * visualizationTab;
        HelpWidget * helpTab;


    };
}

#endif // UI_H
