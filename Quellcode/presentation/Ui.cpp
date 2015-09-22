#include "Ui.h"
#include "../model/Model.h"
#include "Controller.h"

presentation::UI::UI(QWidget *parent)
    : QMainWindow(parent), activeTab(UI::TabThermalDiffusivities) ,
      tabMainCount(5)
{
        //TabBar initialisieren
    tabWidgetMain = new QTabWidget(this);

        //Tabs anlegen
    tabWidgetSub = new QTabWidget(tabWidgetMain);
    thermalDiffusivitiesTab = new AreaWidget(tabWidgetSub,model::SimulationSetup::AreaThermalDiffusivity,
                                             "Temperaturleitkoeffizienten", "m²/s", 0.000001);
    heatSourcesTab = new AreaWidget(tabWidgetSub,model::SimulationSetup::AreaHeatSource,
                                    "Wärmequellen", "K/s");
    iBVsTab = new IBVsWidget(tabWidgetSub);

    simulationTab = new SimulationWidget(tabWidgetMain);
    visualizationTab = new VisualizationWidget(tabWidgetMain);
    optimizationTab = new OptimizationWidget(tabWidgetMain);
    helpTab = new HelpWidget(this);


        //Tabs einfügen
    tabWidgetMain->addTab(tabWidgetSub,"Konfiguration");
    tabWidgetSub->addTab(thermalDiffusivitiesTab,
                         "Temperaturleitkoeffizienten");
    tabWidgetSub->addTab(heatSourcesTab,"Wärmequellen");
    tabWidgetSub->addTab(iBVsTab,"IBV");

    tabWidgetMain->addTab(simulationTab,"Simulation");
    tabWidgetMain->addTab(visualizationTab,"Visualisierung");
    tabWidgetMain->addTab(optimizationTab,"Optimierung");
    tabWidgetMain->addTab(helpTab, "Hilfe");

        //Layout anwenden
    setCentralWidget(tabWidgetMain);

        //internen Slot verbinden
    connect(tabWidgetSub,SIGNAL(currentChanged(int)),this,SLOT(transformTabIDSlot(int)));

        //minimale Fenstergröße
    this->setMinimumSize(1360,765);
}

presentation::UI::~UI()
{

}

void presentation::UI::drawPartialArea(QVector<double> const & partialAreaX,
                                       QVector<double> const & partialAreaY,
                                       model::SimulationSetup::AreaType type)
{
    if(type == model::SimulationSetup::AreaHeatSource)
        heatSourcesTab->drawPartialArea(partialAreaX,partialAreaY);
    else
        thermalDiffusivitiesTab->drawPartialArea(partialAreaX,partialAreaY);
}

int presentation::UI::getInitialFrame() const
{
    return visualizationTab->getInitialFrame();
}

void presentation::UI::revertTabChange(UI::ActiveTab targetTab)
{
    if(targetTab == UI::TabHeatSources)
    {
        tabWidgetMain->setCurrentIndex(UI::TabConfiguration);
        tabWidgetSub->setCurrentIndex(UI::TabHeatSources-tabMainCount);
    }
    else
    {
        tabWidgetMain->setCurrentIndex(UI::TabConfiguration);
        tabWidgetSub->setCurrentIndex(UI::TabThermalDiffusivities-tabMainCount);
    }
    setActiveTab(targetTab);
}

void presentation::UI::setActiveTab(int targetTab)
{
    if(targetTab == UI::TabConfiguration)
        activeTab = tabWidgetSub->currentIndex() + tabMainCount;
    else
        activeTab = targetTab;
    updateNotification();
}

void presentation::UI::setController(Controller *controller)
{
    this->controller = controller;

    connect(tabWidgetMain,SIGNAL(currentChanged(int)),controller,SLOT(tabChangedSlot(int)));
    connect(this,SIGNAL(subTabChange(int)),controller,SLOT(tabChangedSlot(int)));

    thermalDiffusivitiesTab->setController(controller);
    heatSourcesTab->setController(controller);
    iBVsTab->setController(controller);
    simulationTab->setController(controller);
    visualizationTab->setController(controller);
    optimizationTab->setController(controller);
}

void presentation::UI::setModel(model::Model *model)
{
    this->model = model;

    thermalDiffusivitiesTab->setModel(model);
    heatSourcesTab->setModel(model);
    iBVsTab->setModel(model);
    simulationTab->setModel(model);
    visualizationTab->setModel(model);
    optimizationTab->setModel(model);

    // Initialen Tab laden/updaten
    updateNotification();
}

void presentation::UI::updateNotification()
{
    switch(activeTab)
    {
    case TabHeatSources:
        heatSourcesTab->update();
        break;
    case TabIBVs:
        iBVsTab->update();
        break;
    case TabParameterFitting:
        optimizationTab->update();
        break;
    case TabSimulating:
        simulationTab->update();
        break;
    case TabThermalDiffusivities:
        thermalDiffusivitiesTab->update();
        break;
    case TabVisualization:
        visualizationTab->update();
        break;
    default:
        break;
    }
}

void presentation::UI::visualizeState(int frame)
{
  visualizationTab->visualizeState(frame);
}

void presentation::UI::appendToSimulationLogSlot(QString text)
{
    simulationTab->appendToSimulationLog(text);
}

void presentation::UI::nextStageSlot(QString stage, int maximum, bool simulation)
{
    if(simulation)
        simulationTab->nextStage(stage,maximum);
    else
        optimizationTab->nextStage(stage,maximum);
}

void presentation::UI::updateProgressSlot(int step, bool simulation)
{
    if(simulation)
        simulationTab->updateProgress(step);
    else
        optimizationTab->updateProgress(step);
}

void presentation::UI::transformTabIDSlot(int targetTab)
{
    emit subTabChange(targetTab + tabMainCount);
}
