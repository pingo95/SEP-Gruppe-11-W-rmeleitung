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


    //Platzhalter
    spacerItem = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);


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
        //Für Platte
    QVector<double> Ticks;
    QVector<QString> Labels;
    Ticks << 0 << 0.1 << 0.2 << 0.3 << 0.4 << 0.5 << 0.6 << 0.7 << 0.8 << 0.9 << 1 ;
    Labels << QString().number(0)   << QString().number(0.1) << QString().number(0.2) << QString().number(0.3)
           << QString().number(0.4) << QString().number(0.5) << QString().number(0.6) << QString().number(0.7)
           << QString().number(0.8) << QString().number(0.9) << QString().number(1);

    //ThermalConductivityTab
        //Layouts initialisieren
    mainLayoutKonfigurationThermalConductivities = new QVBoxLayout(widgetKonfigurationThermalConductivities);
    subGridLayoutKonfigurationThermalConductivities = new QGridLayout(widgetKonfigurationThermalConductivities);
    subHBoxLayoutKonfigurationThermalConductivities = new QHBoxLayout(widgetKonfigurationThermalConductivities);
    mainLayoutKonfigurationThermalConductivities->addLayout(subGridLayoutKonfigurationThermalConductivities);
        //Labels
    labelTopThermalConductivity = new QLabel("Info",widgetKonfigurationThermalConductivities);
        //Tabelle
    tableViewThermalConductivities = new QTableView(widgetKonfigurationThermalConductivities);
        //Platte
    plateThermalConductivity = new QCustomPlot(widgetKonfigurationThermalConductivities);
            //Platte xAchse unten
    plateThermalConductivity->xAxis->setAutoTicks(false);
    plateThermalConductivity->xAxis->setAutoTickLabels(false);
    plateThermalConductivity->xAxis->setTickVector(Ticks);
    plateThermalConductivity->xAxis->setTickVectorLabels(Labels);
    plateThermalConductivity->xAxis->setRange(0,1);
    plateThermalConductivity->xAxis->grid()->setSubGridVisible(true);
            //Platte yAchse links
    plateThermalConductivity->yAxis->setAutoTicks(false);
    plateThermalConductivity->yAxis->setAutoTickLabels(false);
    plateThermalConductivity->yAxis->setTickVector(Ticks);
    plateThermalConductivity->yAxis->setTickVectorLabels(Labels);
    plateThermalConductivity->yAxis->setRange(0,1);
    plateThermalConductivity->yAxis->grid()->setSubGridVisible(true);
            //Platte xAchse oben
    plateThermalConductivity->xAxis2->setVisible(true);
    plateThermalConductivity->xAxis2->setAutoTicks(false);
    plateThermalConductivity->xAxis2->setAutoTickLabels(false);
    plateThermalConductivity->xAxis2->setTickVector(Ticks);
    plateThermalConductivity->xAxis2->setTickVectorLabels(Labels);
    plateThermalConductivity->xAxis2->setRange(0,1);
    plateThermalConductivity->xAxis2->grid()->setSubGridVisible(true);
            //Platte yAchse rechts
    plateThermalConductivity->yAxis2->setVisible(true);
    plateThermalConductivity->yAxis2->setAutoTicks(false);
    plateThermalConductivity->yAxis2->setAutoTickLabels(false);
    plateThermalConductivity->yAxis2->setTickVector(Ticks);
    plateThermalConductivity->yAxis2->setTickVectorLabels(Labels);
    plateThermalConductivity->yAxis2->setRange(0,1);
    plateThermalConductivity->yAxis2->grid()->setSubGridVisible(true);
    //plateThermalConductivity->setMinimumWidth(350);
        //Layout
    subGridLayoutKonfigurationThermalConductivities->addWidget(labelTopThermalConductivity,0,0);
    subGridLayoutKonfigurationThermalConductivities->addWidget(tableViewThermalConductivities,2,0,2,2);
    subGridLayoutKonfigurationThermalConductivities->addWidget(plateThermalConductivity,2,2,4,4);
    subGridLayoutKonfigurationThermalConductivities->addItem(spacerItem,4,1);

    //HeatSourceTab
        //Layouts initialisieren
    mainLayoutKonfigurationHeatSources = new QVBoxLayout(widgetKonfigurationHeatSources);
    subGridLayoutKonfigurationHeatSources = new QGridLayout(widgetKonfigurationHeatSources);
    subHBoxLayoutKonfigurationHeatSources = new QHBoxLayout(widgetKonfigurationHeatSources);
    mainLayoutKonfigurationHeatSources->addLayout(subGridLayoutKonfigurationHeatSources);
        //Labels
    labelTopHeatSource = new QLabel("Info",widgetKonfigurationHeatSources);
        //Tabelle
    tableViewHeatSources = new QTableView(widgetKonfigurationHeatSources);
        //Platte
    plateHeatSource = new QCustomPlot(widgetKonfigurationHeatSources);
            //Platte xAchse unten
    plateHeatSource->xAxis->setAutoTicks(false);
    plateHeatSource->xAxis->setAutoTickLabels(false);
    plateHeatSource->xAxis->setTickVector(Ticks);
    plateHeatSource->xAxis->setTickVectorLabels(Labels);
    plateHeatSource->xAxis->setRange(0,1);
    plateHeatSource->xAxis->grid()->setSubGridVisible(true);
            //Platte yAchse links
    plateHeatSource->yAxis->setAutoTicks(false);
    plateHeatSource->yAxis->setAutoTickLabels(false);
    plateHeatSource->yAxis->setTickVector(Ticks);
    plateHeatSource->yAxis->setTickVectorLabels(Labels);
    plateHeatSource->yAxis->setRange(0,1);
    plateHeatSource->yAxis->grid()->setSubGridVisible(true);
            //Platte xAchse oben
    plateHeatSource->xAxis2->setVisible(true);
    plateHeatSource->xAxis2->setAutoTicks(false);
    plateHeatSource->xAxis2->setAutoTickLabels(false);
    plateHeatSource->xAxis2->setTickVector(Ticks);
    plateHeatSource->xAxis2->setTickVectorLabels(Labels);
    plateHeatSource->xAxis2->setRange(0,1);
    plateHeatSource->xAxis2->grid()->setSubGridVisible(true);
            //Platte yAchse rechts
    plateHeatSource->yAxis2->setVisible(true);
    plateHeatSource->yAxis2->setAutoTicks(false);
    plateHeatSource->yAxis2->setAutoTickLabels(false);
    plateHeatSource->yAxis2->setTickVector(Ticks);
    plateHeatSource->yAxis2->setTickVectorLabels(Labels);
    plateHeatSource->yAxis2->setRange(0,1);
    plateHeatSource->yAxis2->grid()->setSubGridVisible(true);

    plateHeatSource->setMinimumWidth(250);
        //Layout
    subGridLayoutKonfigurationHeatSources->addWidget(labelTopHeatSource,0,0);
    subGridLayoutKonfigurationHeatSources->addWidget(tableViewHeatSources,2,0,4,2);
    subGridLayoutKonfigurationHeatSources->addWidget(plateHeatSource,2,2,4,4);
    subGridLayoutKonfigurationHeatSources->addItem(spacerItem,4,1);


    //IBVTab
        //Layouts initialisieren
    mainLayoutKonfigurationIBVs = new QVBoxLayout(widgetKonfigurationIBVs);
    subGridLayoutKonfigurationIBVs = new QGridLayout(widgetKonfigurationIBVs);
    subHBoxLayoutKonfigurationIBVs = new QHBoxLayout(widgetKonfigurationIBVs);
    mainLayoutKonfigurationIBVs->addLayout(subGridLayoutKonfigurationIBVs,0);
        //Labels
    labelTopIBV = new QLabel("Info",widgetKonfigurationIBVs);
    labelBottomBoundary = new QLabel("unteren Randwert eingeben",widgetKonfigurationIBVs);
    labelInitialValue = new QLabel("Anfangswert eingeben",widgetKonfigurationIBVs);
    labelLeftBoundary = new QLabel("linken Randwert eingeben",widgetKonfigurationIBVs);
    labelRightBoundary = new QLabel("rechten Randwert eingeben",widgetKonfigurationIBVs);
    labelTopBoundary = new QLabel("oberen Randwert eingeben",widgetKonfigurationIBVs);
        //DoubleSpinBoxes
    doubleSpinBoxBottomBoundary = new QDoubleSpinBox(widgetKonfigurationIBVs);
    doubleSpinBoxInitialValue = new QDoubleSpinBox(widgetKonfigurationIBVs);
    doubleSpinBoxLeftBoundary = new QDoubleSpinBox(widgetKonfigurationIBVs);
    doubleSpinBoxRightBoundary = new QDoubleSpinBox(widgetKonfigurationIBVs);
    doubleSpinBoxTopBoundary = new QDoubleSpinBox(widgetKonfigurationIBVs);
        //Layout
    subGridLayoutKonfigurationIBVs->addWidget(labelTopIBV,0,0);
    subGridLayoutKonfigurationIBVs->addWidget(labelInitialValue,1,0);
    subGridLayoutKonfigurationIBVs->addWidget(doubleSpinBoxInitialValue,1,1);
    subGridLayoutKonfigurationIBVs->addWidget(labelBottomBoundary,2,0);
    subGridLayoutKonfigurationIBVs->addWidget(doubleSpinBoxBottomBoundary,2,1);
    subGridLayoutKonfigurationIBVs->addWidget(labelLeftBoundary,3,0);
    subGridLayoutKonfigurationIBVs->addWidget(doubleSpinBoxLeftBoundary,3,1);
    subGridLayoutKonfigurationIBVs->addWidget(labelRightBoundary,4,0);
    subGridLayoutKonfigurationIBVs->addWidget(doubleSpinBoxRightBoundary,4,1);
    subGridLayoutKonfigurationIBVs->addWidget(labelTopBoundary,5,0);
    subGridLayoutKonfigurationIBVs->addWidget(doubleSpinBoxTopBoundary,5,1);
    subGridLayoutKonfigurationIBVs->addItem(spacerItem,6,1);

    //SimulationTab
    labelTopSimulation = new QLabel("Info",widgetSimulation);
    labelN = new QLabel("N eingeben",widgetSimulation);
    labelM = new QLabel("M eingeben",widgetSimulation);
    labelT = new QLabel("T eingeben",widgetSimulation);




    //HelpTab
    mainLayoutHelp->addLayout(subGridLayoutHelp);
    labelTopHelp = new QLabel("Info",widgetHelp);
    subGridLayoutHelp->addWidget(labelTopHelp,0,0);




    //Layout anwenden
    widgetCentral->setLayout(mainLayout);
    setCentralWidget(widgetCentral);

    //widgetKonfigurationIBVs->setLayout(mainLayoutKonfigurationIBVs);


}

presentation::UI::~UI()
{

}


void presentation::UI::drawPartialHeatSource(QVector<double> &partialAreaX, QVector<double> &partialAreaY)
{

}

void presentation::UI::drawPartialThermalConductivity(QVector<double> &partialAreaX, QVector<double> &partialAreaY)
{

}

QSize presentation::UI::getHeatSourcePlotSize(){
    return plateHeatSource->size();
}

int presentation::UI::getInitialFrame()
{
    return 5;
}

double presentation::UI::getNewHeatSourceValue(int row)
{
    return 5;
}

double presentation::UI::getNewThermalConductivityValue(int row)
{
    return 5;
}

QSize presentation::UI::getThermalConductivityPlotSize()
{
    return plateThermalConductivity->size();
}

int presentation::UI::getHeatSourceAreaID(int pos)
{
    return 5;
}

int presentation::UI::getThermalConductivityAreaID(int pos)
{
    return 5;
}

void presentation::UI::heatSourcePixelToCoords(double mouseX, double mouseY, double &x, double &y)
{

}

void presentation::UI::revertTabChange(UI::ActiveTab targetTab)
{

}

void presentation::UI::setController(Controller *controller)
{

}

void presentation::UI::setModel(model::Model *model)
{

}

void presentation::UI::thermalConductivityPixelToCoords(double mouseX, double mouseY, double &x, double &y)
{

}

void presentation::UI::updateNotification()
{

}

void presentation::UI::visualizeState(int frame)
{

}

void presentation::UI::updateHeatSources()
{

}

void presentation::UI::updateIBVs()
{

}

void presentation::UI::updateSimulating()
{

}

void presentation::UI::updateThermalConductivties()
{

}

void presentation::UI::updateVisualization()
{

}

void presentation::UI::visualizeHeatSourceArea(model::Area *area)
{

}

void presentation::UI::visualizeThermalConductivityArea(model::Area *area)
{

}
