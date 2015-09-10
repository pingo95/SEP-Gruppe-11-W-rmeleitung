#include "Ui.h"
#include "../model/Model.h"
#include "Controller.h"

presentation::UI::UI(QWidget *parent)
    : QMainWindow(parent), MaxConductivity(430), MaxTemperature(1000), // höchste Leitfähigkeit eines Metalls -> Silber
      activeTab(UI::TabThermalConductivity) , tabMainCount(5)
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
    tabWidgetSub->addTab(widgetKonfigurationThermalConductivities,"Wärmeleitkoeffizienten");
    tabWidgetSub->addTab(widgetKonfigurationHeatSources,"Wärmequellen");
    tabWidgetSub->addTab(widgetKonfigurationIBVs,"IBV");
    mainLayoutKonfiguration->addWidget(tabWidgetSub);
        //Für Platte
    QVector<double> Ticks,TicksColorBar,TicksEmpty;
    QVector<QString> Labels,LabelsColorBar,LabelsEmpty;
    Ticks << 0 << 0.1 << 0.2 << 0.3 << 0.4 << 0.5 << 0.6 << 0.7 << 0.8 << 0.9 << 1 ;
    Labels << QString().number(0)   << QString().number(0.1) << QString().number(0.2) << QString().number(0.3)
           << QString().number(0.4) << QString().number(0.5) << QString().number(0.6) << QString().number(0.7)
           << QString().number(0.8) << QString().number(0.9) << QString().number(1);
    TicksColorBar << 0 << MaxTemperature * 0.1 << MaxTemperature * 0.2  << MaxTemperature * 0.3 << MaxTemperature * 0.4
                  << MaxTemperature * 0.5 << MaxTemperature * 0.6 << MaxTemperature * 0.7 << MaxTemperature * 0.8
                  << MaxTemperature * 0.9 << MaxTemperature;
    LabelsColorBar << QString().number(0) << QString().number(MaxTemperature * 0.1) << QString().number(MaxTemperature * 0.2)
                   << QString().number(MaxTemperature * 0.3) << QString().number(MaxTemperature * 0.4) << QString().number(MaxTemperature * 0.5)
                   << QString().number(MaxTemperature * 0.6) << QString().number(MaxTemperature * 0.7) << QString().number(MaxTemperature * 0.8)
                   << QString().number(MaxTemperature * 0.9) << QString().number(MaxTemperature);

    //ThermalConductivityTab
        //Layouts initialisieren
    mainLayoutKonfigurationThermalConductivities = new QVBoxLayout(widgetKonfigurationThermalConductivities);
    subGridLayoutKonfigurationThermalConductivities = new QGridLayout();
    subHBoxLayoutKonfigurationThermalConductivities = new QHBoxLayout();
    mainLayoutKonfigurationThermalConductivities->addLayout(subGridLayoutKonfigurationThermalConductivities);
    //widgetKonfigurationThermalConductivities->setPalette(Pal);;
    //widgetKonfigurationThermalConductivities->setAutoFillBackground(true);
        //Labels
    labelTopThermalConductivity = new QLabel("Dies ist der Tab zur Einstellung der Wärmeleitkoeffizienten.\n"
                                             "Sie können hier die Gebiete der einzelnen Schrottkomponenten festlegen "
                                             "sowie den zugehörigen Wärmeleitkoeffizienten eingeben."
                                             "Für weitere Informationen wechseln Sie in den Hilfs-Tab.",widgetKonfigurationThermalConductivities);
    labelTopThermalConductivity->setWordWrap(true);
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

        //UndoKnopf
    buttonUndoThermalConductivity = new QPushButton("Zurück",widgetKonfigurationThermalConductivities);
    buttonUndoThermalConductivity->setEnabled(false);

    //Color Bar
    colorBarThermalConductivity = new QCustomPlot(widgetKonfigurationHeatSources,false);
            //Color Bar unten
    colorBarThermalConductivity->xAxis->setAutoTicks(false);
    colorBarThermalConductivity->xAxis->setAutoTickLabels(false);
    colorBarThermalConductivity->xAxis->setTickVector(TicksEmpty);
    colorBarThermalConductivity->xAxis->setTickVectorLabels(LabelsEmpty);
            //Color Bar links
    colorBarThermalConductivity->yAxis->setAutoTicks(false);
    colorBarThermalConductivity->yAxis->setAutoTickLabels(false);
    colorBarThermalConductivity->yAxis->setTickVector(TicksColorBar);
    colorBarThermalConductivity->yAxis->setTickVectorLabels(LabelsColorBar);
    colorBarThermalConductivity->yAxis->setRange(0,MaxTemperature);
            //Color Bar links
    colorBarThermalConductivity->xAxis2->setAutoTicks(false);
    colorBarThermalConductivity->xAxis2->setAutoTickLabels(false);
    colorBarThermalConductivity->xAxis2->setVisible(true);
    colorBarThermalConductivity->xAxis2->setTickVector(TicksEmpty);
    colorBarThermalConductivity->xAxis2->setTickVectorLabels(LabelsEmpty);
            //Color Bar rechts
    colorBarThermalConductivity->yAxis2->setAutoTicks(false);
    colorBarThermalConductivity->yAxis2->setAutoTickLabels(false);
    colorBarThermalConductivity->yAxis2->setVisible(true);
    colorBarThermalConductivity->yAxis2->setTickVector(TicksEmpty);
    colorBarThermalConductivity->yAxis2->setTickVectorLabels(LabelsEmpty);

    colorBarThermalConductivity->setMaximumWidth(100);

        //Platte
    plateThermalConductivity = new QCustomPlot(widgetKonfigurationThermalConductivities,false);
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
    subGridLayoutKonfigurationThermalConductivities->addWidget(labelTopThermalConductivity,0,0,1,4);
    subGridLayoutKonfigurationThermalConductivities->addWidget(colorBarThermalConductivity,1,1);
    subGridLayoutKonfigurationThermalConductivities->addWidget(tableWidgetThermalConductivities,1,0);
    subGridLayoutKonfigurationThermalConductivities->addWidget(plateThermalConductivity,1,2);
    subGridLayoutKonfigurationThermalConductivities->addWidget(buttonUndoThermalConductivity,1,3);


    //HeatSourceTab
        //Layouts initialisieren
    mainLayoutKonfigurationHeatSources = new QVBoxLayout(widgetKonfigurationHeatSources);
    subGridLayoutKonfigurationHeatSources = new QGridLayout();
    subHBoxLayoutKonfigurationHeatSources = new QHBoxLayout();
    mainLayoutKonfigurationHeatSources->addLayout(subGridLayoutKonfigurationHeatSources);
        //Labels
    labelTopHeatSource = new QLabel("Dies ist der Tab zur Einstellung der Wärmequellen. \n"
                                    "Sie können hier die Gebiete der Wärmequellen markieren "
                                    "und dessen Wert eingeben."
                                    "Für weitere Informationen wechseln Sie in den Hilfe-Tab",widgetKonfigurationHeatSources);
    labelTopHeatSource->setWordWrap(true);
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
    //Color Bar
    colorBarHeatSource = new QCustomPlot(widgetKonfigurationHeatSources,false);
            //Color Bar links
    colorBarHeatSource->yAxis->setAutoTicks(false);
    colorBarHeatSource->yAxis->setAutoTickLabels(false);
    colorBarHeatSource->yAxis->setTickVector(TicksColorBar);
    colorBarHeatSource->yAxis->setTickVectorLabels(LabelsColorBar);
    colorBarHeatSource->yAxis->setRange(0,MaxTemperature);
            //Color Bar unten
    colorBarHeatSource->xAxis->setAutoTicks(false);
    colorBarHeatSource->xAxis->setAutoTickLabels(false);
    colorBarHeatSource->xAxis->setTickVector(TicksEmpty);
    colorBarHeatSource->xAxis->setTickVectorLabels(LabelsEmpty);
            //Color Bar rechts
    colorBarHeatSource->yAxis2->setAutoTicks(false);
    colorBarHeatSource->yAxis2->setAutoTickLabels(false);
    colorBarHeatSource->yAxis2->setVisible(true);
    colorBarHeatSource->yAxis2->setTickVector(TicksEmpty);
    colorBarHeatSource->yAxis2->setTickVectorLabels(LabelsEmpty);
            //Color Bar links
    colorBarHeatSource->xAxis2->setAutoTicks(false);
    colorBarHeatSource->xAxis2->setAutoTickLabels(false);
    colorBarHeatSource->xAxis2->setVisible(true);
    colorBarHeatSource->xAxis2->setTickVector(TicksEmpty);
    colorBarHeatSource->xAxis2->setTickVectorLabels(LabelsEmpty);

    colorBarHeatSource->setMaximumWidth(100);
        //Platte
    plateHeatSource = new QCustomPlot(widgetKonfigurationHeatSources,false);
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
    subGridLayoutKonfigurationHeatSources->addWidget(labelTopHeatSource,0,0,1,4);
    subGridLayoutKonfigurationHeatSources->addWidget(tableWidgetHeatSources,1,0);
    subGridLayoutKonfigurationHeatSources->addWidget(colorBarHeatSource,1,1);
    subGridLayoutKonfigurationHeatSources->addWidget(plateHeatSource,1,2);
    subGridLayoutKonfigurationHeatSources->addWidget(buttonUndoHeatSource,1,3);

    //IBVTab
        //Layouts initialisieren
    mainLayoutKonfigurationIBVs = new QVBoxLayout(widgetKonfigurationIBVs);
    subGridLayoutKonfigurationIBVs = new QGridLayout();
    subHBoxLayoutKonfigurationIBVs = new QHBoxLayout();
    mainLayoutKonfigurationIBVs->addLayout(subGridLayoutKonfigurationIBVs,0);
        //Labels
    labelTopIBV = new QLabel("Dies ist der Tab zur Einstellung der Anfangs- und Randwerte."
            "Für weitere Informationen wechseln Sie in den Hifle-Tab",widgetKonfigurationIBVs);
    labelTopIBV->setWordWrap(true);
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

    doubleSpinBoxBottomBoundary->setKeyboardTracking(false);
    doubleSpinBoxInitialValue->setKeyboardTracking(false);
    doubleSpinBoxLeftBoundary->setKeyboardTracking(false);
    doubleSpinBoxRightBoundary->setKeyboardTracking(false);
    doubleSpinBoxTopBoundary->setKeyboardTracking(false);

    doubleSpinBoxBottomBoundary->setMinimum(0);
    doubleSpinBoxBottomBoundary->setMaximum(MaxTemperature);
    doubleSpinBoxInitialValue->setMinimum(0);
    doubleSpinBoxInitialValue->setMaximum(MaxTemperature);
    doubleSpinBoxLeftBoundary->setMinimum(0);
    doubleSpinBoxLeftBoundary->setMaximum(MaxTemperature);
    doubleSpinBoxRightBoundary->setMinimum(0);
    doubleSpinBoxRightBoundary->setMaximum(MaxTemperature);
    doubleSpinBoxTopBoundary->setMinimum(0);
    doubleSpinBoxTopBoundary->setMaximum(MaxTemperature);

        //Platzhalter
    spacerItemTabIBVHorizontal = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);
    spacerItemTabIBVVertical = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
        //Layout
    subGridLayoutKonfigurationIBVs->addWidget(labelTopIBV,0,0,1,2);
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
    subGridLayoutKonfigurationIBVs->addItem(spacerItemTabIBVHorizontal,6,0);
    subGridLayoutKonfigurationIBVs->addItem(spacerItemTabIBVVertical,1,2,5,1);
    subGridLayoutKonfigurationIBVs->setColumnMinimumWidth(0,200);
    subGridLayoutKonfigurationIBVs->setColumnMinimumWidth(1,150);


    //SimulationTab
        //Layouts initialisieren
    mainLayoutSimulation->addLayout(subGridLayoutSimulation,0);
    labelTopSimulation = new QLabel("",widgetSimulation);
    labelTopSimulation->setWordWrap(true);
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

    doubleSpinBoxT->setKeyboardTracking(false);
    spinBoxM->setKeyboardTracking(false);
    spinBoxN->setKeyboardTracking(false);

        //ComboBoxes
    comboBoxIntMethod = new QComboBox(widgetSimulation);
    comboBoxIterativeSolver = new QComboBox(widgetSimulation);
        //Progressbar
    progressBarProgress =  new QProgressBar(widgetSimulation);

        //Buttons
    buttonSimulate = new QPushButton("Simulieren",widgetSimulation);
    buttonSimulate->setMaximumWidth(150);
        //Platzhalter
    spacerItemTabSimulationHorizontal = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);
    spacerItemTabSimulationVertical = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
        //Layout
    subGridLayoutSimulation->addWidget(labelTopSimulation,0,0,1,2);
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
    subGridLayoutSimulation->addWidget(buttonSimulate,6,1);
    subGridLayoutSimulation->addItem(spacerItemTabSimulationHorizontal,7,0);
    subGridLayoutSimulation->addItem(spacerItemTabSimulationVertical,1,2,7,1);
    subGridLayoutSimulation->addWidget(labelProgressBar,8,0);
    subGridLayoutSimulation->addWidget(progressBarProgress,8,1,1,2);
    subGridLayoutSimulation->setColumnMinimumWidth(0,200);
    subGridLayoutSimulation->setColumnMinimumWidth(1,150);


    //VisualisierungsTab
        //Layouts initialisieren
    mainLayoutVisualisation->addLayout(subGridLayoutVisualisation,0);
        //Labels
    labelTopVisualization = new QLabel("",widgetVisualisation);
    labelTopVisualization->setWordWrap(true);
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
    //Color Bar
    colorBarVisualization = new QCustomPlot(widgetKonfigurationHeatSources,false);
            //Color Bar unten
    colorBarVisualization->xAxis->setAutoTicks(false);
    colorBarVisualization->xAxis->setAutoTickLabels(false);
    colorBarVisualization->xAxis->setTickVector(TicksEmpty);
    colorBarVisualization->xAxis->setTickVectorLabels(LabelsEmpty);
            //Color Bar links
    colorBarVisualization->yAxis->setAutoTicks(false);
    colorBarVisualization->yAxis->setAutoTickLabels(false);
    colorBarVisualization->yAxis->setTickVector(TicksColorBar);
    colorBarVisualization->yAxis->setTickVectorLabels(LabelsColorBar);
    colorBarVisualization->yAxis->setRange(0,MaxTemperature);
            //Color Bar links
    colorBarVisualization->xAxis2->setAutoTicks(false);
    colorBarVisualization->xAxis2->setAutoTickLabels(false);
    colorBarVisualization->xAxis2->setVisible(true);
    colorBarVisualization->xAxis2->setTickVector(TicksEmpty);
    colorBarVisualization->xAxis2->setTickVectorLabels(LabelsEmpty);
            //Color Bar rechts
    colorBarVisualization->yAxis2->setAutoTicks(false);
    colorBarVisualization->yAxis2->setAutoTickLabels(false);
    colorBarVisualization->yAxis2->setVisible(true);
    colorBarVisualization->yAxis2->setTickVector(TicksEmpty);
    colorBarVisualization->yAxis2->setTickVectorLabels(LabelsEmpty);

    colorBarVisualization->setMinimumWidth(100);
    colorBarVisualization->setMaximumWidth(100);
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
    subGridLayoutVisualisation->addWidget(labelTopVisualization,0,0,1,2);
    subGridLayoutVisualisation->addWidget(colorBarVisualization,1,0);
    subGridLayoutVisualisation->addWidget(plateVideo,1,1);
    subGridLayoutVisualisation->addWidget(buttonPlayVideo,1,2);
    subGridLayoutVisualisation->addWidget(sliderVideo,2,1);
    subGridLayoutVisualisation->addWidget(lcdNumberVideoTimestep,2,2);
    subGridLayoutVisualisation->addItem(spacerItemTabVisualisation,3,0);
    subGridLayoutVisualisation->addItem(spacerItemTabVisualisation,3,0);


    //HelpTab
    mainLayoutHelp->addLayout(subGridLayoutHelp);
    //Layout initialisieren
    mainLayoutHelp->addLayout(subGridLayoutHelp);
        //Labels
    labelHelpTabKonfiguration = new QLabel("Info zum Tab Konfiguration",widgetHelp);
    labelHelpTabKonfiguration->setWordWrap(true);
    labelHelpTabThermalConductivity = new QLabel("Info zum Tab Wärmeleitkoeff");
    labelHelpTabThermalConductivity->setWordWrap(true);
    labelHelpTabHeatSource = new QLabel("Info zum Tab Wärmequellen");
    labelHelpTabHeatSource->setWordWrap(true);
    labelHelpTabIBVs = new QLabel("Info zum Tab IBV");
    labelHelpTabIBVs->setWordWrap(true);
    labelHelpTabSimuation = new QLabel("Info zum Tab Simualtion");
    labelHelpTabSimuation->setWordWrap(true);
    labelHelpTabVisualization = new QLabel("Info zum Tab Visualisierung");
    labelHelpTabVisualization->setWordWrap(true);
        //Platzhalter
    spacerItemTabHelp = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);
        //Layout
    subGridLayoutHelp->addWidget(labelHelpTabKonfiguration,0,0);
    subGridLayoutHelp->addWidget(labelHelpTabThermalConductivity,1,0);
    subGridLayoutHelp->addWidget(labelHelpTabHeatSource,2,0);
    subGridLayoutHelp->addWidget(labelHelpTabIBVs,3,0);
    subGridLayoutHelp->addWidget(labelHelpTabSimuation,4,0);
    subGridLayoutHelp->addWidget(labelHelpTabVisualization,5,0);
    subGridLayoutHelp->addItem(spacerItemTabHelp,6,0);


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
    if(plateHeatSource->graphCount() <= tableWidgetHeatSources->rowCount())
        plateHeatSource->addGraph();
    plateHeatSource->graph(plateHeatSource->graphCount()-1)->setData(partialAreaX,partialAreaY);
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    QPen myPen(Qt::gray);
    myScatter.setPen(myPen);
    myScatter.setBrush(Qt::gray);
    myScatter.setSize(5);
    plateHeatSource->graph(plateHeatSource->graphCount()-1)->setScatterStyle(myScatter);
    myPen.setWidth(2* myPen.width());
    plateHeatSource->graph(plateHeatSource->graphCount()-1)->setPen(myPen);
    plateHeatSource->replot();
}

void presentation::UI::drawPartialThermalConductivity(QVector<double> const & partialAreaX,
                                                      QVector<double> const & partialAreaY)
{
    if(plateThermalConductivity->graphCount() <= tableWidgetThermalConductivities->rowCount())
        plateThermalConductivity->addGraph();
    plateThermalConductivity->graph(plateThermalConductivity->graphCount()-1)->setData(partialAreaX,partialAreaY);
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    QPen myPen(Qt::gray);
    myScatter.setPen(myPen);
    myScatter.setBrush(Qt::gray);
    myScatter.setSize(5);
    plateThermalConductivity->graph(plateThermalConductivity->graphCount()-1)->setScatterStyle(myScatter);
    myPen.setWidth(2* myPen.width());
    plateThermalConductivity->graph(plateThermalConductivity->graphCount()-1)->setPen(myPen);
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

QString presentation::UI::getNewHeatSourceValue(int row) const
{
    return tableWidgetHeatSources->item(row,UI::ColumnValue)->text();
}

QString presentation::UI::getNewThermalConductivityValue(int row) const
{
    return tableWidgetThermalConductivities->item(row,UI::ColumnValue)->text();
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
    connect(tableWidgetThermalConductivities,SIGNAL(cellChanged(int,int)),controller,SLOT(thermalConductivityValueChangedSlot(int,int)));
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

void presentation::UI::updateVisibilityHeatSource(const int pos)
{
    bool visible = tableWidgetHeatSources->item(pos,UI::ColumnVisibility)->checkState() == Qt::Checked ? true : false;
    bool ok;
    int id = tableWidgetHeatSources->item(pos,UI::ColumnID)->text().toInt(&ok);
    if(ok)
    {
        visibilityHeatSources[id] = visible;
        plateHeatSource->graph(pos)->setVisible(visible);
    }
}

void presentation::UI::updateVisibilityThermalConductivity(const int pos)
{
    bool visible = tableWidgetThermalConductivities->item(pos,UI::ColumnVisibility)->checkState() == Qt::Checked ? true : false;
    bool ok;
    int id = tableWidgetThermalConductivities->item(pos,UI::ColumnID)->text().toInt(&ok);
    if(ok)
    {
        visibilityThermalConductivities[id] = visible;
        plateThermalConductivity->graph(pos)->setVisible(visible);
    }
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
        tableWidgetHeatSources->item(i,UI::ColumnVisibility)->
                setCheckState(visibilityHeatSources.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
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
            tmpItemPtr->setCheckState(visibilityHeatSources.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
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


    for(int j = 0; j < rowCount; ++j)
        plateHeatSource->graph(j)->setVisible(false);
    for(int j = rowCount+1; j < hSCount; ++j)
        plateHeatSource->addGraph();
    it = heatSources.begin();
    for(int j = 0; j < hSCount; ++j,++it)
    {
        QVector<double> x,y;
        (*it)->getPoints(x,y);
        plateHeatSource->graph(j)->setData(x,y);
        plateHeatSource->graph(j)->setPen(QPen(Qt::black));
        plateHeatSource->graph(j)->setBrush(QBrush(valueToColour((*it)->getValue(),
                                  model::Model::HeatSourceArea),Qt::SolidPattern));
        plateHeatSource->graph(j)->setScatterStyle(QCPScatterStyle::ssNone);
        plateHeatSource->graph(j)->setVisible(visibilityHeatSources.value((*it)->getID(),true));
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
    doubleSpinBoxT->setValue(model->getT());
    if(model->getSimulating())
    {
        buttonSimulate->setEnabled(false);
        labelTopSimulation->setText("Es kann nicht noch einmal simuliert werden, "
                                    "wenn die vorherige Simulation noch nicht abgeschlossen wurde");
    }
    else
    {
       buttonSimulate->setEnabled(true);
       labelTopSimulation->setText("Dies ist der Tab zur Einstellung der Simulation.\n"
                                   "Für weitere Informationen wechseln Sie in den Hilfe-Tab.");
    }

    comboBoxIntMethod->setCurrentText(model->getSelectedIntMethod());
    comboBoxIterativeSolver->setCurrentText(model->getSelectedIterativeSolver());
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
        tableWidgetThermalConductivities->item(i,UI::ColumnVisibility)->
                setCheckState(visibilityThermalConductivities.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
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
            tmpItemPtr->setCheckState(visibilityThermalConductivities.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
            tableWidgetThermalConductivities->setItem(i,UI::ColumnVisibility,tmpItemPtr);
        }
    }
    else
    {
        for(int j = rowCount-1; j >= i; --j)
        {
            delete tableWidgetThermalConductivities->takeItem(j,UI::ColumnID);
            delete tableWidgetThermalConductivities->takeItem(j,UI::ColumnValue);
        }
    }

    for(int j = 0; j < rowCount; ++j)
        plateThermalConductivity->graph(j)->setVisible(false);
    for(int j = rowCount+1; j < tCCount; ++j)
        plateThermalConductivity->addGraph();
    it = thermalConductivities.begin();
    for(int j = 0; j < tCCount; ++j,++it)
    {
        QVector<double> x,y;
        (*it)->getPoints(x,y);
        plateThermalConductivity->graph(j)->setData(x,y);
        plateThermalConductivity->graph(j)->setPen(QPen(Qt::black));
        plateThermalConductivity->graph(j)->setBrush(QBrush(valueToColour((*it)->getValue(),
                                  model::Model::ThermalConductivityArea),Qt::SolidPattern));
        plateThermalConductivity->graph(j)->setScatterStyle(QCPScatterStyle::ssNone);
        plateThermalConductivity->graph(j)->setVisible(visibilityThermalConductivities.value((*it)->getID(),true));
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
    if (model->getSimulated())
    {
        sliderVideo->setEnabled(true);
        buttonPlayVideo->setEnabled(true);
        labelTopVisualization->setText("Dies ist der Tab zur Visualisierung der Simulationsergebnisse.\n"
                                       "Hier können Sie sich, mit Hilfe des Schiebereglers, Einzelbilder oder ein Video anzeigen lassen."
                                       "Für genauere Informationen wechseln Sie in den Hilfe-Tab.");
        resultM = model->getResultM();
        resultN = model->getResultN();
        resultT = model->getResultT();
        result = model->getResult();
    }
    else
    {
        labelTopVisualization->setText("Ergebnisse können erst angezeigt werden, "
                                       "nachdem eine Simulation durchgeführt wurde.");
    }
}

QColor presentation::UI::valueToColour(const double value, model::Model::AreaTyp type)
{
    double maxValue;
    switch(type)
    {
    case model::Model::HeatSourceArea:
        maxValue = MaxTemperature;
        break;
    case model::Model::ThermalConductivityArea:
        maxValue = MaxConductivity;
        break;
    default:
        return Qt::blue;
    }
    // value in Wellenlänge umwandeln
    double minValue = 0, minVisibleWaveLength = 380.0,
            maxVisibileWaveLength = 780.0;
    double wavelength = (value - minValue) / (maxValue - minValue) *
            (maxVisibileWaveLength - minVisibleWaveLength) + minVisibleWaveLength;

    // Wellenlänge in rgb umrechnen
    double factor = 1.0, red, green, blue;
    if(wavelength >= 380 && wavelength <= 439)
    {
        red = -(wavelength -440) / (440-380);
        green = 0.0;
        blue = 1.0;
    }
    else
    {
        if(wavelength >= 440 && wavelength <= 489)
        {
            red = 0.0;
            green = (wavelength - 440) / (490 - 440);
            blue = 1.0;
        }
        else
        {
            if(wavelength >= 490 && wavelength <= 509)
            {
                red = 0.0;
                green = 1.0;
                blue = -(wavelength - 510) / (510 - 490);
            }
            else
            {
                if(wavelength >= 510 && wavelength <= 579)
                {
                    red = (wavelength - 510) / (580 - 510);
                    green = 1.0;
                    blue = 0.0;
                }
                else
                {
                    if(wavelength >= 580 && wavelength <= 644)
                    {
                        red = 1.0;
                        green = -(wavelength - 645) / (645 - 580);
                        blue = 0.0;
                    }
                    else
                    {
                        if(wavelength >= 645 && wavelength <= 780)
                        {
                            red = 1.0;
                            green = 0.0;
                            blue = 0.0;
                        }
                        else
                        {
                            red = 0.0;
                            green = 0.0;
                            blue = 0.0;
                        }
                    }
                }
            }
        }
    }
    if(wavelength >= 380 && wavelength <= 419)
        factor = 0.3 + 0.7*(wavelength - 380) / (420 -380);
    else
        if(wavelength >= 701 && wavelength <= 780)
            factor = 0.3 + 0.7*(780 -  wavelength) / (780 - 700);
    double gamma = 0.8, intensityMax = 255;
    int r = round(intensityMax * pow(red*factor,gamma));
    int g = round(intensityMax * pow(green*factor,gamma));
    int b = round(intensityMax * pow(blue*factor,gamma));
    return QColor(r,g,b,255);
}

void presentation::UI::transformTabID(int targetTab)
{
    emit subTabChange(targetTab + tabMainCount);
}
