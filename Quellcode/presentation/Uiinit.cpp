#include "Ui.h"

void presentation::UI::initConfiguration()
{
    tabWidgetSub = new QTabWidget(tabWidgetMain);
    tabWidgetMain->addTab(tabWidgetSub,"Konfiguration");
    tabWidgetSub->setMinimumSize(350,250);

    // internen Slot verbinden
    connect(tabWidgetSub,SIGNAL(currentChanged(int)),this,SLOT(transformTabID(int)));
}

void presentation::UI::initHeatSources()
{
    widgetConfigurationHeatSources = new QWidget(this);
    tabWidgetSub->addTab(widgetConfigurationHeatSources,"Wärmequellen");

        //Layouts initialisieren
    subGridLayoutKonfigurationHeatSources = new QGridLayout(widgetConfigurationHeatSources);

        //Labels
    labelTopHeatSource = new QLabel("Dies ist der Tab zur Einstellung der Wärmequellen. \n"
                                    "Sie können hier die Gebiete der Wärmequellen markieren "
                                    "und dessen Wert eingeben."
                                    "Für weitere Informationen wechseln Sie in den Hilfe-Tab",widgetConfigurationHeatSources);
    labelTopHeatSource->setWordWrap(true);
        //Tabelle
    tableWidgetHeatSources = new QTableWidget(widgetConfigurationHeatSources);
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
    buttonUndoHeatSource = new QPushButton("Zurück",widgetConfigurationHeatSources);
    buttonUndoHeatSource->setEnabled(false);

        //Für Platte
    QVector<double> Ticks,TicksColorBar,TicksEmpty;
    QVector<QString> Labels,LabelsColorBar,LabelsEmpty;
    Ticks << 0 << 0.1 << 0.2 << 0.3 << 0.4 << 0.5
          << 0.6 << 0.7 << 0.8 << 0.9 << 1 ;
    Labels << QString::number(0)   << QString::number(0.1)
           << QString::number(0.2) << QString::number(0.3)
           << QString::number(0.4) << QString::number(0.5)
           << QString::number(0.6) << QString::number(0.7)
           << QString::number(0.8) << QString::number(0.9)
           << QString::number(1);
    TicksColorBar << 0 << MaxTemperature * 0.1 << MaxTemperature * 0.2
                  << MaxTemperature * 0.3 << MaxTemperature * 0.4
                  << MaxTemperature * 0.5 << MaxTemperature * 0.6
                  << MaxTemperature * 0.7 << MaxTemperature * 0.8
                  << MaxTemperature * 0.9 << MaxTemperature;
    LabelsColorBar << QString::number(0) << QString::number(MaxTemperature * 0.1)
                   << QString::number(MaxTemperature * 0.2) << QString::number(MaxTemperature * 0.3)
                   << QString::number(MaxTemperature * 0.4) << QString::number(MaxTemperature * 0.5)
                   << QString::number(MaxTemperature * 0.6) << QString::number(MaxTemperature * 0.7)
                   << QString::number(MaxTemperature * 0.8) << QString::number(MaxTemperature * 0.9)
                   << QString::number(MaxTemperature);

    //Color Bar
    colorBarHeatSource = new QCustomPlot(widgetConfigurationHeatSources,false);
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
    plateHeatSource = new QCustomPlot(widgetConfigurationHeatSources,false);
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
}

void presentation::UI::initHelp()
{
    widgetHelp = new QWidget(this);
    tabWidgetMain->addTab(widgetHelp, "Hilfe");

    subGridLayoutHelp = new QGridLayout(widgetHelp);

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
}

void presentation::UI::initIBVs()
{
    widgetConfigurationIBVs = new QWidget(this);
    tabWidgetSub->addTab(widgetConfigurationIBVs,"IBV");

    subGridLayoutKonfigurationIBVs = new QGridLayout(widgetConfigurationIBVs);

        //Labels
    labelTopIBV = new QLabel("Dies ist der Tab zur Einstellung der Anfangs- und Randwerte."
            "Für weitere Informationen wechseln Sie in den Hifle-Tab",widgetConfigurationIBVs);
    labelTopIBV->setWordWrap(true);
    labelBottomBoundary = new QLabel("unteren Randwert eingeben",widgetConfigurationIBVs);
    labelInitialValue = new QLabel("Anfangswert eingeben",widgetConfigurationIBVs);
    labelLeftBoundary = new QLabel("linken Randwert eingeben",widgetConfigurationIBVs);
    labelRightBoundary = new QLabel("rechten Randwert eingeben",widgetConfigurationIBVs);
    labelTopBoundary = new QLabel("oberen Randwert eingeben",widgetConfigurationIBVs);
        //DoubleSpinBoxes
    doubleSpinBoxBottomBoundary = new QDoubleSpinBox(widgetConfigurationIBVs);
    doubleSpinBoxInitialValue = new QDoubleSpinBox(widgetConfigurationIBVs);
    doubleSpinBoxLeftBoundary = new QDoubleSpinBox(widgetConfigurationIBVs);
    doubleSpinBoxRightBoundary = new QDoubleSpinBox(widgetConfigurationIBVs);
    doubleSpinBoxTopBoundary = new QDoubleSpinBox(widgetConfigurationIBVs);

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
}

void presentation::UI::initSimulating()
{
    widgetSimulation = new QWidget(this);
    tabWidgetMain->addTab(widgetSimulation,"Simulation");

    subGridLayoutSimulation = new QGridLayout(widgetSimulation);

        //Layouts initialisieren
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
}

void presentation::UI::initThermalConductivities()
{
    widgetConfigurationThermalConductivities = new QWidget(this);
    tabWidgetSub->addTab(widgetConfigurationThermalConductivities,
                         "Wärmeleitkoeffizienten");

    subGridLayoutKonfigurationThermalConductivities = new QGridLayout(widgetConfigurationThermalConductivities);

        //Labels
    labelTopThermalConductivity = new QLabel("Dies ist der Tab zur Einstellung der Wärmeleitkoeffizienten.\n"
                                             "Sie können hier die Gebiete der einzelnen Schrottkomponenten festlegen "
                                             "sowie den zugehörigen Wärmeleitkoeffizienten eingeben."
                                             "Für weitere Informationen wechseln Sie in den Hilfs-Tab.",widgetConfigurationThermalConductivities);
    labelTopThermalConductivity->setWordWrap(true);
        //Tabelle
    tableWidgetThermalConductivities = new QTableWidget(widgetConfigurationThermalConductivities);
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
    buttonUndoThermalConductivity = new QPushButton("Zurück",widgetConfigurationThermalConductivities);
    buttonUndoThermalConductivity->setEnabled(false);

        //Für Platte
    QVector<double> Ticks,TicksColorBar,TicksEmpty;
    QVector<QString> Labels,LabelsColorBar,LabelsEmpty;
    Ticks << 0 << 0.1 << 0.2 << 0.3 << 0.4 << 0.5
          << 0.6 << 0.7 << 0.8 << 0.9 << 1 ;
    Labels << QString::number(0)   << QString::number(0.1)
           << QString::number(0.2) << QString::number(0.3)
           << QString::number(0.4) << QString::number(0.5)
           << QString::number(0.6) << QString::number(0.7)
           << QString::number(0.8) << QString::number(0.9)
           << QString::number(1);
    TicksColorBar << 0 << MaxConductivity * 0.1 << MaxConductivity * 0.2
                  << MaxConductivity * 0.3 << MaxConductivity * 0.4
                  << MaxConductivity * 0.5 << MaxConductivity * 0.6
                  << MaxConductivity * 0.7 << MaxConductivity * 0.8
                  << MaxConductivity * 0.9 << MaxConductivity;
    LabelsColorBar << QString::number(0) << QString::number(MaxConductivity * 0.1)
                   << QString::number(MaxConductivity * 0.2) << QString::number(MaxConductivity * 0.3)
                   << QString::number(MaxConductivity * 0.4) << QString::number(MaxConductivity * 0.5)
                   << QString::number(MaxConductivity * 0.6) << QString::number(MaxConductivity * 0.7)
                   << QString::number(MaxConductivity * 0.8) << QString::number(MaxConductivity * 0.9)
                   << QString::number(MaxConductivity);

    //Color Bar
    colorBarThermalConductivity = new QCustomPlot(widgetConfigurationThermalConductivities,false);
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
    colorBarThermalConductivity->yAxis->setRange(0,MaxConductivity);
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
    plateThermalConductivity = new QCustomPlot(widgetConfigurationThermalConductivities,false);
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

}

void presentation::UI::initVisualization()
{
    widgetVisualisation = new QWidget(this);
    tabWidgetMain->addTab(widgetVisualisation, "Visualisierung");

    subGridLayoutVisualisation = new QGridLayout(widgetVisualisation);

        //Für Platte
    QVector<double> Ticks,TicksColorBar,TicksEmpty;
    QVector<QString> Labels,LabelsColorBar,LabelsEmpty;
    Ticks << 0 << 0.1 << 0.2 << 0.3 << 0.4 << 0.5
          << 0.6 << 0.7 << 0.8 << 0.9 << 1 ;
    Labels << QString::number(0)   << QString::number(0.1)
           << QString::number(0.2) << QString::number(0.3)
           << QString::number(0.4) << QString::number(0.5)
           << QString::number(0.6) << QString::number(0.7)
           << QString::number(0.8) << QString::number(0.9)
           << QString::number(1);
    TicksColorBar << 0 << MaxTemperature * 0.1 << MaxTemperature * 0.2
                  << MaxTemperature * 0.3 << MaxTemperature * 0.4
                  << MaxTemperature * 0.5 << MaxTemperature * 0.6
                  << MaxTemperature * 0.7 << MaxTemperature * 0.8
                  << MaxTemperature * 0.9 << MaxTemperature;
    LabelsColorBar << QString::number(0) << QString::number(MaxTemperature * 0.1)
                   << QString::number(MaxTemperature * 0.2) << QString::number(MaxTemperature * 0.3)
                   << QString::number(MaxTemperature * 0.4) << QString::number(MaxTemperature * 0.5)
                   << QString::number(MaxTemperature * 0.6) << QString::number(MaxTemperature * 0.7)
                   << QString::number(MaxTemperature * 0.8) << QString::number(MaxTemperature * 0.9)
                   << QString::number(MaxTemperature);

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
    colorBarVisualization = new QCustomPlot(widgetVisualisation,false);
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
}
