#include "ui.h"

presentation::UI::UI(QWidget *parent)
    : QMainWindow(parent)
{
    //centrales Widget initialisieren
    widgetCentral = new QWidget(this);

    widgetHelp = new QWidget(this);
    widgetVisualisation = new QWidget(this);
    widgetKonfiguration = new QWidget(this);
    widgetSimulation = new QWidget(this);

    tabWidgetMain = new QTabWidget(widgetCentral);

    //tabWidgetMain->setFixedSize(600,245);
    tabWidgetMain->addTab(widgetKonfiguration,"Konfig");
    tabWidgetMain->addTab(widgetSimulation,"Simulation");
    tabWidgetMain->addTab(widgetVisualisation, "Visualisierung");
    tabWidgetMain->addTab(widgetHelp, "Hilfe");



    //TabHelp
    labelHelp = new QLabel("Info",widgetCentral);


    //Layout initialliesieren
    mainLayout = new QVBoxLayout(widgetCentral);
    subGridLayout = new QGridLayout(widgetCentral);
    subHBoxLayout = new QHBoxLayout(widgetCentral);

    mainLayout->addLayout(subGridLayout,0);


    //Hilfe Info
    mainLayout->addWidget(labelHelp,5,0);

    //Layout anwenden
    widgetCentral->setLayout(mainLayout);
    setCentralWidget(widgetCentral);

}

presentation::UI::~UI()
{

}
