#include "Ui.h"
#include "../model/Model.h"
#include "Controller.h"

presentation::UI::UI(QWidget *parent)
    : QMainWindow(parent), activeTab(UI::TabThermalConductivity) , tabMainCount(5)
{
    connect(tabWidgetSub,SIGNAL(currentChanged(int)),this,SLOT(transformTabID(int)));

    //centrales Widget initialisieren
    widgetCentral = new QWidget(this);

    //Farben
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background,Qt::lightGray);

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
    //widgetKonfigurationThermalConductivities->setPalette(Pal);;
    //widgetKonfigurationThermalConductivities->setAutoFillBackground(true);
        //Labels
    labelTopThermalConductivity = new QLabel("Info",widgetKonfigurationThermalConductivities);
        //Tabelle
    tablewidgetThermalConductivities = new QTableWidget(widgetKonfigurationThermalConductivities);
    tablewidgetThermalConductivities->setColumnCount(3);
    listHeaderThermalConductivity <<"ID"<<"Wert"<<"Sichtbar";
    tablewidgetThermalConductivities->setHorizontalHeaderLabels(listHeaderThermalConductivity);
    tablewidgetThermalConductivities->setColumnWidth(0,20);
    tablewidgetThermalConductivities->setColumnWidth(1,60);
    tablewidgetThermalConductivities->setColumnWidth(2,60);
    tablewidgetThermalConductivities->setShowGrid(true);
    tablewidgetThermalConductivities->verticalHeader()->setVisible(false);

    tablewidgetThermalConductivities->setRowCount(20);
        //test Hinzufügen
//    tablewidgetThermalConductivities->setItem(0, 1, new QTableWidgetItem("Hello"));
//    QTableWidgetItem * checkboxItem = new QTableWidgetItem();
//    checkboxItem->setCheckState(Qt::Checked);
//    tablewidgetThermalConductivities->setItem(2,1,checkboxItem);

        //UndoKnopf
    buttonUndoThermalConductivity = new QPushButton("Undo",widgetKonfigurationThermalConductivities);
    buttonUndoThermalConductivity->setEnabled(false);
        //Platte
    plateThermalConductivity = new QCustomPlot(widgetKonfigurationThermalConductivities);
    plateThermalConductivity->addGraph();
    //plateThermalConductivity->graph(0)->setPen(Qt::SolidLine);
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
        //Layout
    plateThermalConductivity->setMinimumWidth(350);
    tablewidgetThermalConductivities->setMinimumWidth(142);
    tablewidgetThermalConductivities->setMaximumWidth(142);
    buttonUndoThermalConductivity->setMaximumWidth(100);
    subGridLayoutKonfigurationThermalConductivities->addWidget(labelTopThermalConductivity,0,0);
    subGridLayoutKonfigurationThermalConductivities->addWidget(tablewidgetThermalConductivities,1,0);
    subGridLayoutKonfigurationThermalConductivities->addWidget(plateThermalConductivity,1,1);
    subGridLayoutKonfigurationThermalConductivities->addWidget(buttonUndoThermalConductivity,1,2);


    //HeatSourceTab
        //Layouts initialisieren
    mainLayoutKonfigurationHeatSources = new QVBoxLayout(widgetKonfigurationHeatSources);
    subGridLayoutKonfigurationHeatSources = new QGridLayout(widgetKonfigurationHeatSources);
    subHBoxLayoutKonfigurationHeatSources = new QHBoxLayout(widgetKonfigurationHeatSources);
    mainLayoutKonfigurationHeatSources->addLayout(subGridLayoutKonfigurationHeatSources);
        //Labels
    labelTopHeatSource = new QLabel("Info",widgetKonfigurationHeatSources);
        //Tabelle
    tableWidgetHeatSources = new QTableWidget(widgetKonfigurationHeatSources);
    tableWidgetHeatSources->setColumnCount(3);
    listHeaderHeatSource <<"ID"<<"Wert"<<"Sichtbar";
    tableWidgetHeatSources->setHorizontalHeaderLabels(listHeaderHeatSource);
    tableWidgetHeatSources->setColumnWidth(0,20);
    tableWidgetHeatSources->setColumnWidth(1,60);
    tableWidgetHeatSources->setColumnWidth(2,60);
    tableWidgetHeatSources->setShowGrid(true);
    tableWidgetHeatSources->verticalHeader()->setVisible(false);

    tableWidgetHeatSources->setRowCount(20);
        //Undo Knopf
    buttonUndoHeatSource = new QPushButton("Undo",widgetKonfigurationHeatSources);
    buttonUndoHeatSource->setEnabled(false);
        //Platte
    plateHeatSource = new QCustomPlot(widgetKonfigurationHeatSources);
    plateHeatSource->addGraph();
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
        //Layout
    plateHeatSource->setMinimumWidth(350);
    tableWidgetHeatSources->setMinimumWidth(142);
    tableWidgetHeatSources->setMaximumWidth(142);
    buttonUndoHeatSource->setMaximumWidth(100);
    subGridLayoutKonfigurationHeatSources->addWidget(labelTopHeatSource,0,0);
    subGridLayoutKonfigurationHeatSources->addWidget(tableWidgetHeatSources,1,0);
    subGridLayoutKonfigurationHeatSources->addWidget(plateHeatSource,1,1);
    subGridLayoutKonfigurationHeatSources->addWidget(buttonUndoHeatSource,1,2);

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
        //Platzhalter
    spacerItemTabIBV = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);
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
    subGridLayoutKonfigurationIBVs->addItem(spacerItemTabIBV,6,0);


    //SimulationTab
        //Layouts initialisieren
    mainLayoutSimulation->addLayout(subGridLayoutSimulation,0);
        //Labels
    labelTopSimulation = new QLabel("Info",widgetSimulation);
    labelN = new QLabel("N eingeben",widgetSimulation);
    labelM = new QLabel("M eingeben",widgetSimulation);
    labelT = new QLabel("T eingeben",widgetSimulation);
    labelProgressBar = new QLabel("Fortschrittsbalken",widgetSimulation);
    labelSelectIntMethod = new QLabel("IntMethod",widgetSimulation);
    labelSelectSolver = new QLabel("Solver",widgetSimulation);
        //Boxes
    doubleSpinBoxT = new QDoubleSpinBox(widgetSimulation);
    spinBoxM = new QSpinBox(widgetSimulation);
    spinBoxN = new QSpinBox(widgetSimulation);
        //ComboBoxes
    comboBoxIntMethod = new QComboBox(widgetSimulation);
    comboBoxSolver = new QComboBox(widgetSimulation);
        //Progressbar
    progressBarProgress =  new QProgressBar(widgetSimulation);
    progressBarProgress->setMaximumWidth(1000);
        //Buttons
    buttonSimulate = new QPushButton("Simulieren",widgetSimulation);
        //Platzhalter
    spacerItemTabSimulation = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);
        //Layout
    subGridLayoutSimulation->addWidget(labelTopSimulation,0,0);
    subGridLayoutSimulation->addWidget(labelM,1,0);
    subGridLayoutSimulation->addWidget(spinBoxM,1,1);
    subGridLayoutSimulation->addWidget(labelN,2,0);
    subGridLayoutSimulation->addWidget(spinBoxN,2,1);
    subGridLayoutSimulation->addWidget(labelT,3,0);
    subGridLayoutSimulation->addWidget(doubleSpinBoxT,3,1);
    subGridLayoutSimulation->addWidget(labelSelectIntMethod,4,0);
    subGridLayoutSimulation->addWidget(comboBoxIntMethod,4,1);
    subGridLayoutSimulation->addWidget(labelSelectSolver,5,0);
    subGridLayoutSimulation->addWidget(comboBoxSolver,5,1);
    subGridLayoutSimulation->addWidget(buttonSimulate,6,0,1,2);
    subGridLayoutSimulation->addWidget(labelProgressBar,7,0);
    subGridLayoutSimulation->addWidget(progressBarProgress,7,1);
    subGridLayoutSimulation->addItem(spacerItemTabSimulation,8,0);


    //VisualisierungsTab
        //Layouts initialisieren
    mainLayoutVisualisation->addLayout(subGridLayoutVisualisation,0);
        //Labels
    labelTopVisualization = new QLabel("Info",widgetVisualisation);
        //Buttons
    buttonPlayVideo = new QPushButton("Play",widgetVisualisation);
    buttonPlayVideo->setEnabled(false);
        //Schieberegler
    sliderVideo = new QSlider(widgetVisualisation);
    sliderVideo->setEnabled(false);
    sliderVideo->setMaximumHeight(200);
    sliderVideo->setOrientation(Qt::Horizontal);
        //LcdNumber
    lcdNumberVideoTimestep = new QLCDNumber(widgetVisualisation);
        //Platzhalter
    spacerItemTabVisualisation = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);
        //Video anzeige
    plateVideo = new QCustomPlot(widgetVisualisation);
    plateVideo->addGraph();
            //Platte xAchse unten
    plateVideo->xAxis->setAutoTicks(false);
    plateVideo->xAxis->setAutoTickLabels(false);
    plateVideo->xAxis->setTickVector(Ticks);
    plateVideo->xAxis->setTickVectorLabels(Labels);
    plateVideo->xAxis->setRange(0,1);
    plateVideo->xAxis->grid()->setSubGridVisible(true);
            //Platte yAchse links
    plateVideo->yAxis->setAutoTicks(false);
    plateVideo->yAxis->setAutoTickLabels(false);
    plateVideo->yAxis->setTickVector(Ticks);
    plateVideo->yAxis->setTickVectorLabels(Labels);
    plateVideo->yAxis->setRange(0,1);
    plateVideo->yAxis->grid()->setSubGridVisible(true);
            //Platte xAchse oben
    plateVideo->xAxis2->setVisible(true);
    plateVideo->xAxis2->setAutoTicks(false);
    plateVideo->xAxis2->setAutoTickLabels(false);
    plateVideo->xAxis2->setTickVector(Ticks);
    plateVideo->xAxis2->setTickVectorLabels(Labels);
    plateVideo->xAxis2->setRange(0,1);
    plateVideo->xAxis2->grid()->setSubGridVisible(true);
            //Platte yAchse rechts
    plateVideo->yAxis2->setVisible(true);
    plateVideo->yAxis2->setAutoTicks(false);
    plateVideo->yAxis2->setAutoTickLabels(false);
    plateVideo->yAxis2->setTickVector(Ticks);
    plateVideo->yAxis2->setTickVectorLabels(Labels);
    plateVideo->yAxis2->setRange(0,1);
    plateVideo->yAxis2->grid()->setSubGridVisible(true);
        //Layout
    plateVideo->setMinimumWidth(350);
    plateVideo->setMinimumHeight(650);
    buttonPlayVideo->setMaximumWidth(100);
    subGridLayoutVisualisation->addWidget(labelTopVisualization,0,0);
    subGridLayoutVisualisation->addWidget(plateVideo,1,0);
    subGridLayoutVisualisation->addWidget(buttonPlayVideo,1,1);
    subGridLayoutVisualisation->addWidget(lcdNumberVideoTimestep,2,1);
    subGridLayoutVisualisation->addWidget(sliderVideo,2,0);
    subGridLayoutVisualisation->addItem(spacerItemTabVisualisation,3,0);




    //HelpTab
    mainLayoutHelp->addLayout(subGridLayoutHelp);
    labelTopHelp = new QLabel("Info",widgetHelp);
    spacerItemTabHelp = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);
    subGridLayoutHelp->addWidget(labelTopHelp,0,0);
    subGridLayoutHelp->addItem(spacerItemTabHelp,1,0);




    //Layout anwenden
    widgetCentral->setLayout(mainLayout);
    setCentralWidget(widgetCentral);


}

presentation::UI::~UI()
{

}


void presentation::UI::drawPartialHeatSource(QVector<double> const & partialAreaX,
                                             QVector<double> const & partialAreaY)
{
    plateHeatSource->graph(0)->setData(partialAreaX,partialAreaY);

}

void presentation::UI::drawPartialThermalConductivity(QVector<double> const & partialAreaX,
                                                      QVector<double> const & partialAreaY)
{
    plateThermalConductivity->graph(0)->setData(partialAreaX,partialAreaY);
}

QSize presentation::UI::getHeatSourcePlotSize() const
{
    return plateHeatSource->size();
}

int presentation::UI::getInitialFrame() const
{
    return sliderVideo->value();
}

double presentation::UI::getNewHeatSourceValue(int row) const
{
    return tableWidgetHeatSources->item(row,UI::ColumnValue)->text().toDouble();
}

double presentation::UI::getNewThermalConductivityValue(int row) const
{
    return tablewidgetThermalConductivities->item(row,UI::ColumnValue)->text().toDouble();
}

QSize presentation::UI::getThermalConductivityPlotSize() const
{
    return plateThermalConductivity->size();
}


void presentation::UI::heatSourcePixelToCoords(double const mouseX, double const mouseY, double &x, double &y)
{
    x = plateHeatSource->xAxis->pixelToCoord(mouseX);
    y = plateHeatSource->yAxis->pixelToCoord(mouseY);
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
        tabWidgetSub->setCurrentIndex(UI::TabThermalConductivity-tabMainCount);
    }


}

void presentation::UI::setActiveTab(UI::ActiveTab targetTab)
{
    activeTab = targetTab;
    updateNotification();
}

void presentation::UI::setController(Controller *controller)
{
    this->controller = controller;
}

void presentation::UI::setModel(model::Model *model)
{
    this->model = model;
}

void presentation::UI::thermalConductivityPixelToCoords(double const mouseX, double const mouseY, double &x, double &y)
{
    x = plateThermalConductivity->xAxis->pixelToCoord(mouseX);
    y = plateThermalConductivity->yAxis->pixelToCoord(mouseY);
}

void presentation::UI::updateNotification()
{
    switch(activeTab)
    {
    case TabHeatSources:
        updateHeatSources();
        break;
    case TabIBVs:
        updateIBVs();
        break;
    case TabParameterFitting:
        //updateParameterFitting;
        break;
    case TabSimulating:
        updateSimulating();
        break;
    case TabThermalConductivity:
        updateThermalConductivties();
        break;
    case TabVisualization:
        updateVisualization();
        break;
    default:
        break;
    }
}

void presentation::UI::visualizeState(int frame)
{

}

void presentation::UI::updateHeatSources()
{
    int hSCount = model->getHeatSourcesCount();
    int rowCount = tableWidgetHeatSources->rowCount();
    tableWidgetHeatSources->setRowCount(hSCount);
    int tmpBound = hSCount <= rowCount ? hSCount : rowCount;
    QList<model::Area*> const & heatSources = model->getHeatSources();
    QList<model::Area*>::const_iterator it = heatSources.begin();
    int i = 0;
    for(; i <= tmpBound; ++i, ++it)
    {
        tableWidgetHeatSources->item(i,UI::ColumnID)->
                setText(QString().number((*it)->getID()));
        tableWidgetHeatSources->item(i,UI::ColumnValue)->
                setText(QString().number((*it)->getValue()));
    }
    if(hSCount >= rowCount)
    {
        for(; i <= hSCount; ++i, ++it)
        {
            tableWidgetHeatSources->setItem(i,UI::ColumnID,new
                  QTableWidgetItem(QString().number((*it)->getID())));
            tableWidgetHeatSources->setItem(i,UI::ColumnValue,new
                  QTableWidgetItem(QString().number((*it)->getValue())));
        }
    }
    else
    {
        for(int j = rowCount; j >= i; --j)
        {
            delete tableWidgetHeatSources->takeItem(j,UI::ColumnID);
            delete tableWidgetHeatSources->takeItem(j,UI::ColumnValue);
        }
    }

    if(hSCount==0)
    {
        buttonUndoHeatSource->setEnabled(false);
    }
    else
    {
        buttonUndoHeatSource->setEnabled(true);
    }
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

void presentation::UI::valueToColour(const double value)
{

}

void presentation::UI::visualizeHeatSourceArea(model::Area *area)
{

}

void presentation::UI::visualizeThermalConductivityArea(model::Area *area)
{

}

void presentation::UI::transformTabID(int targetTab)
{
    emit subTabChange(targetTab + tabMainCount);
}
