#include "Optimizationwidget.h"


presentation::OptimizationWidget::OptimizationWidget(QWidget *parent)
{
    widgetCentral = new QWidget(this);
    tabWidget = new QTabWidget(widgetCentral);
    widgetData = new QWidget(this);
    widgetSolution = new QWidget(this);

    tabWidget->addTab(widgetConfiguration,"Konfiguration");
    tabWidget->addTab(widgetData,"Messdaten");
    tabWidget->addTab(widgetSolution,"Ergebnis");

    layoutDataTab = new QGridLayout(widgetData);
    layoutSolutionTab = new QGridLayout(widgetSolution);
    layoutConfigurationTab = new QGridLayout(widgetConfiguration);

    //Tab Konfig
        //Labels
    labelConfiguration = new QLabel("Information",widgetConfiguration);

        //CheckBoxes
    checkBoxN = new QCheckBox("überschreibe Anzahl Messwerte",widgetConfiguration);
    checkBoxHeatSources = new QCheckBox("Nutze bereits bestehende Wärmequellen zur Simulation",widgetConfiguration);

        //Layout anwenden
    layoutConfigurationTab->addWidget(labelConfiguration,0,0);


    //Tab MessDaten
        //Buttons
    buttonLoad = new QPushButton(widgetData);
    buttonOptimization = new QPushButton(widgetData);

        //Labels
    labelData = new QLabel("Temperaturverteilung",widgetData);

        //Table
    tableWidgetData = new QTableWidget(widgetData);

        //Layout anwenden
    layoutDataTab->addWidget(buttonLoad,0,0);
    layoutDataTab->addWidget(buttonOptimization,0,1);
    layoutDataTab->addWidget(labelData,1,0);
    layoutDataTab->addWidget(tableWidgetData,2,0,1,2);


    //Tab Ergebnis
        //Label
    labelSolution = new QLabel("Gefittete Wärmeleitkoeffizienten",widgetSolution);
        //Table
    tableWidgetSolution = new QTableWidget(widgetSolution);

        //Layout anwenden
    layoutSolutionTab->addWidget(tableWidgetSolution,0,0);
    layoutSolutionTab->addWidget(tableWidgetSolution,1,0);



}
