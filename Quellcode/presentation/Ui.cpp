#include "Ui.h"
#include "../model/Model.h"
#include "Controller.h"

presentation::UI::UI(QWidget *parent)
    : QMainWindow(parent), activeTab(UI::TabThermalConductivity) , tabMainCount(5)
{
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
    subGridLayout = new QGridLayout();
    subGridLayoutHelp = new QGridLayout();
    subGridLayoutKonfiguration = new QGridLayout();
    subGridLayoutSimulation = new QGridLayout();
    subGridLayoutVisualisation = new QGridLayout();
    //subHBoxLayouts
    subHBoxLayout = new QHBoxLayout();
    subHBoxLayoutHelp = new QHBoxLayout();
    subHBoxLayoutKonfiguration = new QHBoxLayout();
    subHBoxLayoutSimualtion = new QHBoxLayout();
    subHBoxLayoutVisualisation = new QHBoxLayout();

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
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCrossSquare);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::green);
    myScatter.setSize(5);

    //ThermalConductivityTab
        //Layouts initialisieren
    mainLayoutKonfigurationThermalConductivities = new QVBoxLayout(widgetKonfigurationThermalConductivities);
    subGridLayoutKonfigurationThermalConductivities = new QGridLayout();
    subHBoxLayoutKonfigurationThermalConductivities = new QHBoxLayout();
    mainLayoutKonfigurationThermalConductivities->addLayout(subGridLayoutKonfigurationThermalConductivities);
    //widgetKonfigurationThermalConductivities->setPalette(Pal);;
    //widgetKonfigurationThermalConductivities->setAutoFillBackground(true);
        //Labels
    labelTopThermalConductivity = new QLabel("Info",widgetKonfigurationThermalConductivities);
        //Tabelle
    tableWidgetThermalConductivities = new QTableWidget(widgetKonfigurationThermalConductivities);
    tableWidgetThermalConductivities->setColumnCount(3);
    listHeaderThermalConductivity <<"ID"<<"Wert"<<"Sichtbar";
    tableWidgetThermalConductivities->setHorizontalHeaderLabels(listHeaderThermalConductivity);
    tableWidgetThermalConductivities->setColumnWidth(0,20);
    tableWidgetThermalConductivities->setColumnWidth(1,60);
    tableWidgetThermalConductivities->setColumnWidth(2,60);
    tableWidgetThermalConductivities->setShowGrid(true);
    tableWidgetThermalConductivities->verticalHeader()->setVisible(false);

//    tableWidgetThermalConductivities->horizontalHeader()->setSelectionBehavior(QAbstractItemView::SelectItems);
    tableWidgetThermalConductivities->horizontalHeader()->setSectionsClickable(false);
    tableWidgetThermalConductivities->setRowCount(0);
//        //test Hinzufügen
//    QTableWidgetItem * tmpItemPtr = new
//            QTableWidgetItem(QString().number(4));
//    tmpItemPtr->setFlags(Qt::ItemIsEnabled);
//    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    tableWidgetThermalConductivities->setItem(0,UI::ColumnID,tmpItemPtr);
//    tmpItemPtr = new
//            QTableWidgetItem(QString().number(1.0));
//    tmpItemPtr->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    tableWidgetThermalConductivities->setItem(0,UI::ColumnValue,tmpItemPtr);
//    tmpItemPtr = new QTableWidgetItem();
//    tmpItemPtr->setFlags(Qt::ItemIsEnabled| Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
//    tmpItemPtr->setCheckState(Qt::Checked);
//    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignRight);
//    tableWidgetThermalConductivities->setItem(0,UI::ColumnVisibility,tmpItemPtr);

        //UndoKnopf
    buttonUndoThermalConductivity = new QPushButton("Zurück",widgetKonfigurationThermalConductivities);
    buttonUndoThermalConductivity->setEnabled(false);
        //Platte
    plateThermalConductivity = new QCustomPlot(widgetKonfigurationThermalConductivities,false);
    plateThermalConductivity->addGraph();
    plateThermalConductivity->graph(0)->setScatterStyle(myScatter);
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
    tableWidgetThermalConductivities->setMinimumWidth(142);
    tableWidgetThermalConductivities->setMaximumWidth(142);
    buttonUndoThermalConductivity->setMaximumWidth(100);
    subGridLayoutKonfigurationThermalConductivities->addWidget(labelTopThermalConductivity,0,0);
    subGridLayoutKonfigurationThermalConductivities->addWidget(tableWidgetThermalConductivities,1,0);
    subGridLayoutKonfigurationThermalConductivities->addWidget(plateThermalConductivity,1,1);
    subGridLayoutKonfigurationThermalConductivities->addWidget(buttonUndoThermalConductivity,1,2);


    //HeatSourceTab
        //Layouts initialisieren
    mainLayoutKonfigurationHeatSources = new QVBoxLayout(widgetKonfigurationHeatSources);
    subGridLayoutKonfigurationHeatSources = new QGridLayout();
    subHBoxLayoutKonfigurationHeatSources = new QHBoxLayout();
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

    tableWidgetHeatSources->horizontalHeader()->setSectionsClickable(false);
    tableWidgetHeatSources->setRowCount(0);
        //Undo Knopf
    buttonUndoHeatSource = new QPushButton("Zurück",widgetKonfigurationHeatSources);
    buttonUndoHeatSource->setEnabled(false);
        //Platte
    plateHeatSource = new QCustomPlot(widgetKonfigurationHeatSources,false);
    plateHeatSource->addGraph();
    myScatter.setShape(QCPScatterStyle::ssCrossSquare);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::green);
    myScatter.setSize(5);
    plateHeatSource->graph(0)->setScatterStyle(myScatter);
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
    subGridLayoutKonfigurationIBVs = new QGridLayout();
    subHBoxLayoutKonfigurationIBVs = new QHBoxLayout();
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
    spinBoxM->setMinimum(0);
    spinBoxM->setMaximum(800);
    spinBoxN = new QSpinBox(widgetSimulation);
    spinBoxN->setMaximum(0);
    spinBoxN->setMaximum(800);
        //ComboBoxes
    comboBoxIntMethod = new QComboBox(widgetSimulation);
    comboBoxIterativeSolver = new QComboBox(widgetSimulation);
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
    subGridLayoutSimulation->addWidget(comboBoxIterativeSolver,5,1);
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
    plateVideo = new QCustomPlot(widgetVisualisation,false);
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
    this->setMinimumSize(750,750);
    widgetCentral->setLayout(mainLayout);
    setCentralWidget(widgetCentral);

    // internen Slot verbinden
    connect(tabWidgetSub,SIGNAL(currentChanged(int)),this,SLOT(transformTabID(int)));
}

presentation::UI::~UI()
{

}


void presentation::UI::drawPartialHeatSource(QVector<double> const & partialAreaX,
                                             QVector<double> const & partialAreaY)
{
    plateHeatSource->graph(0)->setData(partialAreaX,partialAreaY);
    plateHeatSource->replot();

}

void presentation::UI::drawPartialThermalConductivity(QVector<double> const & partialAreaX,
                                                      QVector<double> const & partialAreaY)
{
    plateThermalConductivity->graph(0)->setData(partialAreaX,partialAreaY);
    plateThermalConductivity->replot();
}

int presentation::UI::getHeatSourceID(const int pos) const
{
    return tableWidgetHeatSources->item(pos,UI::ColumnID)->text().toInt();
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
    bool ok;
    double value = tableWidgetHeatSources->item(row,UI::ColumnValue)->text().toDouble(&ok);
    return ok ? value : 0;
}

double presentation::UI::getNewThermalConductivityValue(int row) const
{
    bool ok;
    double value = tableWidgetThermalConductivities->item(row,UI::ColumnValue)->text().toDouble(&ok);
    return ok ? value : 0;
}

int presentation::UI::getThermalConductivityID(const int pos) const
{
    return tableWidgetThermalConductivities->item(pos,UI::ColumnID)->text().toInt();
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

void presentation::UI::setActiveTab(int targetTab)
{
    activeTab = targetTab;
    updateNotification();
}

void presentation::UI::setController(Controller *controller)
{
    this->controller = controller;
    connect(plateHeatSource,SIGNAL(mousePress(QMouseEvent*)),controller,SLOT(heatSourcesClickSlot(QMouseEvent*)));
    connect(tableWidgetHeatSources,SIGNAL(cellChanged(int,int)),controller,SLOT(heatSourceValueChangedSlot(int,int)));
    connect(doubleSpinBoxBottomBoundary,SIGNAL(valueChanged(double)),controller,SLOT(newBottomBoundarySlot(double)));
    connect(doubleSpinBoxInitialValue,SIGNAL(valueChanged(double)),controller,SLOT(newInitialValueSlot(double)));
    connect(doubleSpinBoxLeftBoundary,SIGNAL(valueChanged(double)),controller,SLOT(newLeftBoundarySlot(double)));
    connect(spinBoxM,SIGNAL(valueChanged(int)),controller,SLOT(newMSlot(int)));
    connect(spinBoxN,SIGNAL(valueChanged(int)),controller,SLOT(newNSlot(int)));
    connect(doubleSpinBoxRightBoundary,SIGNAL(valueChanged(double)),controller,SLOT(newRightBoundarySlot(double)));
    connect(doubleSpinBoxTopBoundary,SIGNAL(valueChanged(double)),controller,SLOT(newTopBoundarySlot(double)));
    connect(doubleSpinBoxT,SIGNAL(valueChanged(double)),controller,SLOT(newTSlot(double)));
    connect(buttonPlayVideo,SIGNAL(pressed()),controller,SLOT(playVideoSlot()));
    connect(comboBoxIntMethod,SIGNAL(activated(QString)),controller,SLOT(selectIntMethodSlot(QString)));
    connect(comboBoxIterativeSolver,SIGNAL(activated(QString)),controller,SLOT(selectIterativeSolverSlot(QString)));
    connect(buttonSimulate,SIGNAL(clicked(bool)),controller,SLOT(simulateSlot()));
    connect(tabWidgetMain,SIGNAL(currentChanged(int)),controller,SLOT(tabChangedSlot(int)));
    connect(this,SIGNAL(subTabChange(int)),controller,SLOT(tabChangedSlot(int)));
    connect(plateThermalConductivity,SIGNAL(mousePress(QMouseEvent*)),controller,SLOT(thermalConductivitiesClickSlot(QMouseEvent*)));
    connect(tableWidgetHeatSources,SIGNAL(cellChanged(int,int)),controller,SLOT(thermalConductivityValueChangedSlot(int,int)));
    connect(buttonUndoHeatSource,SIGNAL(clicked(bool)),controller,SLOT(undoHeatSourceSlot()));
    connect(buttonUndoThermalConductivity,SIGNAL(clicked(bool)),controller,SLOT(undoThermalConductivitySlot()));
    connect(sliderVideo,SIGNAL(valueChanged(int)),controller,SLOT(visualizeStateSlot(int)));
}

void presentation::UI::setModel(model::Model *model)
{
    this->model = model;
    QList<QString> tmpList = model->getIntMethodNames();
    QList<QString>::const_iterator it = tmpList.begin();
    for(; it < tmpList.end(); ++it)
        comboBoxIntMethod->addItem((*it));
    tmpList = model->getIterativeSolverNames();
    it = tmpList.begin();
    for(; it < tmpList.end(); ++it)
        comboBoxIterativeSolver->addItem((*it));
    // Initialen Tab laden/updaten
    updateNotification();
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
    for(; i < tmpBound; ++i, ++it)
    {
        tableWidgetHeatSources->item(i,UI::ColumnID)->
                setText(QString().number((*it)->getID()));
        tableWidgetHeatSources->item(i,UI::ColumnValue)->
                setText(QString().number((*it)->getValue()));
    }
    if(hSCount >= rowCount)
    {
        for(; i < hSCount; ++i, ++it)
        {
            QTableWidgetItem * tmpItemPtr = new
                    QTableWidgetItem(QString().number((*it)->getID()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetHeatSources->setItem(i,UI::ColumnID,tmpItemPtr);
            tmpItemPtr = new
                    QTableWidgetItem(QString().number((*it)->getValue()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetHeatSources->setItem(i,UI::ColumnValue,tmpItemPtr);
            tmpItemPtr = new QTableWidgetItem();
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            tmpItemPtr->setCheckState(Qt::Checked);
            tableWidgetHeatSources->setItem(i,UI::ColumnVisibility,tmpItemPtr);
        }
    }
    else
    {
        for(int j = rowCount-1; j >= i; --j)
        {
            delete tableWidgetHeatSources->takeItem(j,UI::ColumnID);
            delete tableWidgetHeatSources->takeItem(j,UI::ColumnValue);
        }
    }


    for(int j = 1; j <= rowCount; ++j)
        plateHeatSource->graph(j)->setVisible(false);
    it = heatSources.begin();
    for(; it != heatSources.end(); ++it)
    {
        for(int j = rowCount+1; j <= (*it)->getID(); ++j)
            plateHeatSource->addGraph();
        visualizeHeatSourceArea(*it);
    }
    plateHeatSource->replot();
    controller->testPartialHeatSource();

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
    doubleSpinBoxBottomBoundary->setValue(model->getBottomBoundary());
    doubleSpinBoxInitialValue->setValue(model->getInitialValue());
    doubleSpinBoxLeftBoundary->setValue(model->getLeftBoundary());
    doubleSpinBoxRightBoundary->setValue(model->getRightBoundary());
    doubleSpinBoxTopBoundary->setValue(model->getTopBoundary());
}


void presentation::UI::updateSimulating()
{
    doubleSpinBoxT->setValue(model->getT());
    spinBoxM->setValue(model->getM());
    spinBoxN->setValue(model->getN());
}

void presentation::UI::updateThermalConductivties()
{
    int tCCount = model->getThermalConductivitiesCount();
    int rowCount = tableWidgetThermalConductivities->rowCount();
    tableWidgetThermalConductivities->setRowCount(tCCount);
    int tmpBound = tCCount <= rowCount ? tCCount : rowCount;
    QList<model::Area*> const & thermalConductivities = model->getThermalConductivities();
    QList<model::Area*>::const_iterator it = thermalConductivities.begin();
    int i = 0;
    for (; i < tmpBound; ++i, ++it)
    {
        tableWidgetThermalConductivities->item(i,UI::ColumnID)->
                setText(QString().number((*it)->getID()));
        tableWidgetThermalConductivities->item(i,UI::ColumnValue)->
                setText(QString().number((*it)->getValue()));
    }
    if (tCCount >= rowCount)
    {
        for(; i < tCCount; ++i, ++it)
        {
            QTableWidgetItem * tmpItemPtr = new
                    QTableWidgetItem(QString().number((*it)->getID()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetThermalConductivities->setItem(i,UI::ColumnID,tmpItemPtr);
            tmpItemPtr = new
                    QTableWidgetItem(QString().number((*it)->getValue()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetThermalConductivities->setItem(i,UI::ColumnValue,tmpItemPtr);
            tmpItemPtr = new QTableWidgetItem();
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            tmpItemPtr->setCheckState(Qt::Checked);
            tableWidgetThermalConductivities->setItem(i,UI::ColumnVisibility,tmpItemPtr);
        }
    }
    else
    {
        for(int j = rowCount-1; j>=i; --j)
        {
            delete tableWidgetThermalConductivities->takeItem(j,UI::ColumnID);
            delete tableWidgetThermalConductivities->takeItem(j,UI::ColumnValue);
        }
    }

    for(int j = 1; j <= rowCount; ++j)
        plateThermalConductivity->graph(j)->setVisible(false);
    it = thermalConductivities.begin();
    for(; it != thermalConductivities.end(); ++it)
    {
        for(int j = rowCount+1; j <= (*it)->getID(); ++j)
            plateThermalConductivity->addGraph();
        visualizeThermalConductivityArea(*it);
    }
    plateThermalConductivity->replot();
    controller->testPartialThermalConductivity();

    if(tCCount==0)
    {
        buttonUndoThermalConductivity->setEnabled(false);
    }
    else
    {
        buttonUndoThermalConductivity->setEnabled(true);
    }
}

void presentation::UI::updateVisualization()
{

}

QColor presentation::UI::valueToColour(const double value)
{
    return Qt::blue;
}

void presentation::UI::visualizeHeatSourceArea(model::Area *area)
{
    int id = area->getID();
    QVector<double> x,y;
    area->getPoints(x,y);
    plateHeatSource->graph(id)->setData(x,y);
    plateHeatSource->graph(id)->setPen(QPen(Qt::black));
    plateHeatSource->graph(id)->setBrush(QBrush(
                valueToColour(area->getValue()),Qt::SolidPattern));
    plateHeatSource->graph(id)->setVisible(true);
}

void presentation::UI::visualizeThermalConductivityArea(model::Area *area)
{
    int id = area->getID();
    QVector<double> x,y;
    area->getPoints(x,y);
    plateThermalConductivity->graph(id)->setData(x,y);
    plateThermalConductivity->graph(id)->setPen(QPen(Qt::black));
    plateThermalConductivity->graph(id)->setBrush(QBrush(
                valueToColour(area->getValue()),Qt::SolidPattern));
    plateThermalConductivity->graph(id)->setVisible(true);
}

void presentation::UI::transformTabID(int targetTab)
{
    emit subTabChange(targetTab + tabMainCount);
}
