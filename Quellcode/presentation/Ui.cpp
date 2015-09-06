#include "ui.h"

presentation::UI::UI(QWidget *parent)
    : QMainWindow(parent)
{
    //centrales Widget initialisieren
    widgetCentral = new QWidget(this);

    //TabBar initialisieren
    tabWidgetMain = new QTabWidget(widgetCentral);

    //Tabs erstellen
    widgetHelp = new QWidget(this);
    widgetVisualisation = new QWidget(this);
    widgetKonfiguration = new QWidget(this);
    widgetSimulation = new QWidget(this);

    //Tabs einsetzen
    tabWidgetMain->setMinimumSize(350,250);
    tabWidgetMain->addTab(widgetKonfiguration,"Konfiguration");
    tabWidgetMain->addTab(widgetSimulation,"Simulation");
    tabWidgetMain->addTab(widgetVisualisation, "Visualisierung");
    tabWidgetMain->addTab(widgetHelp, "Hilfe");

    //Layouts initialliesieren
    //mainLayouts
    mainLayout = new QVBoxLayout(widgetCentral);
    mainLayoutHelp = new QVBoxLayout(widgetHelp);
    mainLayoutKonfiguration = new QVBoxLayout(widgetKonfiguration);
    mainLayoutSimulation = new QVBoxLayout(widgetSimulation);
    mainLayoutVisualisation = new QVBoxLayout(widgetVisualisation);
    //subGridLayouts
    subGridLayout = new QGridLayout(widgetCentral);
    subGridLayoutHelp = new QGridLayout(widgetHelp);
    subGridLayoutKonfiguration = new QGridLayout(widgetKonfiguration);
    subGridLayoutSimulation = new QGridLayout(widgetSimulation);
    subGridLayoutVisualisation = new QGridLayout(widgetVisualisation);
    //subHBoxLayouts
    subHBoxLayout = new QHBoxLayout(widgetCentral);
    subHBoxLayoutHelp = new QHBoxLayout(widgetHelp);
    subHBoxLayoutKonfiguration = new QHBoxLayout(widgetKonfiguration);
    subHBoxLayoutSimualtion = new QHBoxLayout(widgetSimulation);
    subHBoxLayoutVisualisation = new QHBoxLayout(widgetVisualisation);



    mainLayout->addWidget(tabWidgetMain);


    //KonfigurationTab
    tabWidgetSub = new QTabWidget(widgetKonfiguration);
    widgetKonfigurationHeatSources = new QWidget(this);
    widgetKonfigurationIBVs = new QWidget(this);
    widgetKonfigurationThermalConductivities = new QWidget(this);
    tabWidgetSub->setMinimumSize(350,250);
    tabWidgetSub->addTab(widgetKonfigurationThermalConductivities,"Waermeleitkoeff");
    tabWidgetSub->addTab(widgetKonfigurationHeatSources,"Waermequellen");
    tabWidgetSub->addTab(widgetKonfigurationIBVs,"IBV");
    mainLayoutKonfiguration->addWidget(tabWidgetSub);

    //ThermalConductivityTab
    mainLayoutKonfigurationThermalConductivities = new QVBoxLayout(widgetKonfigurationThermalConductivities);
    subGridLayoutKonfigurationThermalConductivities = new QGridLayout(widgetKonfigurationThermalConductivities);
    subHBoxLayoutKonfigurationThermalConductivities = new QHBoxLayout(widgetKonfigurationThermalConductivities);
    mainLayoutKonfigurationThermalConductivities->addLayout(subGridLayoutKonfigurationThermalConductivities);

    labelTopThermalConductivity = new QLabel("Info",widgetKonfigurationThermalConductivities);
    subGridLayoutKonfigurationThermalConductivities->addWidget(labelTopThermalConductivity,0,0);
    tableViewThermalConductivities = new QTableView(widgetKonfigurationThermalConductivities);
    subGridLayoutKonfigurationThermalConductivities->addWidget(tableViewThermalConductivities,2,0,4,2);
    plateThermalConductivity = new QCustomPlot(widgetKonfigurationThermalConductivities);
    plateThermalConductivity->setMinimumWidth(250);
    subGridLayoutKonfigurationThermalConductivities->addWidget(plateThermalConductivity,2,2,4,4);


    //HeatSourceTab
    mainLayoutKonfigurationHeatSources = new QVBoxLayout(widgetKonfigurationHeatSources);
    subGridLayoutKonfigurationHeatSources = new QGridLayout(widgetKonfigurationHeatSources);
    subHBoxLayoutKonfigurationHeatSources = new QHBoxLayout(widgetKonfigurationHeatSources);
    mainLayoutKonfigurationHeatSources->addLayout(subGridLayoutKonfigurationHeatSources);

    labelTopHeatSource = new QLabel("Info",widgetKonfigurationHeatSources);
    subGridLayoutKonfigurationHeatSources->addWidget(labelTopHeatSource,0,0);
    tableViewHeatSources = new QTableView(widgetKonfigurationHeatSources);
    subGridLayoutKonfigurationHeatSources->addWidget(tableViewHeatSources,2,0,4,2);
    plateHeatSource = new QCustomPlot(widgetKonfigurationHeatSources);
    plateHeatSource->setMinimumWidth(250);
    subGridLayoutKonfigurationHeatSources->addWidget(plateHeatSource,2,2,4,4);




    //IBVTab
    mainLayoutKonfigurationIBVs = new QVBoxLayout(widgetKonfigurationIBVs);
    subGridLayoutKonfigurationIBVs = new QGridLayout(widgetKonfigurationIBVs);
    subHBoxLayoutKonfigurationIBVs = new QHBoxLayout(widgetKonfigurationIBVs);
    mainLayoutKonfigurationIBVs->addLayout(subGridLayoutKonfigurationIBVs);






    //HelpTab
    mainLayoutHelp->addLayout(subGridLayoutHelp);
    labelTopHelp = new QLabel("Info",widgetHelp);
    subGridLayoutHelp->addWidget(labelTopHelp,0,0);




    //Layout anwenden
    widgetCentral->setLayout(mainLayout);
    setCentralWidget(widgetCentral);


}

presentation::UI::~UI()
{

}
