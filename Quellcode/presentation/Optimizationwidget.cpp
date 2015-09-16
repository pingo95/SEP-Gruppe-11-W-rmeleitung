#include "Optimizationwidget.h"


presentation::OptimizationWidget::OptimizationWidget(QWidget *parent)
    : QWidget(parent) , MaxTemperature(1000)
{
    widgetCentral = new QWidget(this);
    layoutWidgetCentral = new QGridLayout(widgetCentral);
    tabWidget = new QTabWidget(widgetCentral);
    layoutWidgetCentral->addWidget(tabWidget);

    widgetSolution = new QWidget(this);
    widgetConfiguration = new QWidget(this);

    tabWidget->addTab(widgetConfiguration,"Konfiguration");
    tabWidget->addTab(widgetSolution,"Ergebnis");

    layoutSolutionTab = new QGridLayout(widgetSolution);
    layoutConfigurationTab = new QGridLayout(widgetConfiguration);

    //Tab Konfig
        //Labels
    labelConfiguration = new QLabel("Information",widgetConfiguration);
    labelSpinBoxN = new QLabel("neues N eingeben",widgetConfiguration);
    labelSpinBoxN->setAlignment(Qt::AlignRight);
    labelSpinBoxN->setEnabled(false);
    labelInitialValue = new QLabel("Manueller Anfangswert",widgetConfiguration);
    labelInitialValue->setAlignment(Qt::AlignRight);
    labelInitialValue->setEnabled(false);
    labelSettings = new QLabel("Simulationseinstellungen",widgetConfiguration);
    labelM = new QLabel("M: ",widgetConfiguration);
    labelT = new QLabel("T: ",widgetConfiguration);
    labelEpsilon = new QLabel("Epsilon: ",widgetConfiguration);
    labelMaxIt = new QLabel("Maximale Iterationsanzahl: ",widgetConfiguration);
    labelData = new QLabel("Temperaturverteilung",widgetConfiguration);
    labelProgressBar = new QLabel("Fortschrittsbalken",widgetConfiguration);

        //CheckBoxes
    checkBoxN = new QCheckBox("Überschreibe Anzahl Messwerte",widgetConfiguration);
    checkBoxHeatSources = new QCheckBox("Nutze bereits vorhandene Wärmequellen zur Simulation",widgetConfiguration);
    checkBoxThermalConductivities = new QCheckBox("Überschreibe bereits vorhandene Wärmeleitkoeffizienten zur Simulation",widgetConfiguration);


        //spinBoxes
    spinBoxN = new QSpinBox(widgetConfiguration);
    spinBoxN->setMaximum(0);
    spinBoxN->setMaximum(800);
    spinBoxN->setEnabled(false);

    spinBoxM = new QSpinBox(widgetConfiguration);
    spinBoxM->setMinimum(0);
    spinBoxM->setMaximum(800);
    spinBoxM->setReadOnly(true);
    spinBoxM->setButtonSymbols(QAbstractSpinBox::NoButtons);

    spinBoxMaxIt = new QSpinBox(widgetConfiguration);
    spinBoxMaxIt->setMinimum(0);
    spinBoxMaxIt->setMaximum(1000);
    spinBoxMaxIt->setReadOnly(true);
    spinBoxMaxIt->setButtonSymbols(QAbstractSpinBox::NoButtons);

    doubleSpinBoxInitialValue = new QDoubleSpinBox(widgetConfiguration);
    doubleSpinBoxInitialValue->setMinimum(0);
    doubleSpinBoxInitialValue->setMaximum(430);
    doubleSpinBoxInitialValue->setEnabled(false);

    doubleSpinBoxEpsilon = new QDoubleSpinBox(widgetConfiguration);
    doubleSpinBoxEpsilon->setDecimals(10);
    doubleSpinBoxEpsilon->setMinimum(1e-10);
    doubleSpinBoxEpsilon->setMaximum(1e-5);
    doubleSpinBoxEpsilon->setReadOnly(true);
    doubleSpinBoxEpsilon->setButtonSymbols(QAbstractSpinBox::NoButtons);

    doubleSpinBoxT = new QDoubleSpinBox(widgetConfiguration);
    doubleSpinBoxT->setDecimals(3);
    doubleSpinBoxT->setMinimum(0);
    doubleSpinBoxT->setMaximum(1000);
    doubleSpinBoxT->setSuffix("s");
    doubleSpinBoxT->setReadOnly(true);
    doubleSpinBoxT->setButtonSymbols(QAbstractSpinBox::NoButtons);

        //Gruppen
    QGridLayout * gridBoxUserSettings = new QGridLayout();
    gridBoxUserSettings->addWidget(checkBoxN,0,0,1,2);
    gridBoxUserSettings->addWidget(labelSpinBoxN,1,0);
    gridBoxUserSettings->addWidget(spinBoxN,1,1);
    gridBoxUserSettings->addWidget(checkBoxHeatSources,2,0,1,2);
    gridBoxUserSettings->addWidget(checkBoxThermalConductivities,3,0,1,2);
    gridBoxUserSettings->addWidget(labelInitialValue,4,0);
    gridBoxUserSettings->addWidget(doubleSpinBoxInitialValue,4,1);
    QGroupBox * groupBoxUserSettings = new QGroupBox(widgetConfiguration);
    groupBoxUserSettings->setLayout(gridBoxUserSettings);

    QGridLayout * gridBoxSettings = new QGridLayout();
    gridBoxSettings->addWidget(labelSettings,0,0);
    gridBoxSettings->addWidget(labelM,1,0);
    gridBoxSettings->addWidget(spinBoxM,1,1);
    gridBoxSettings->addWidget(labelT,2,0);
    gridBoxSettings->addWidget(doubleSpinBoxT,2,1);
    gridBoxSettings->addWidget(labelEpsilon,3,0);
    gridBoxSettings->addWidget(doubleSpinBoxEpsilon,3,1);
    gridBoxSettings->addWidget(labelMaxIt,4,0);
    gridBoxSettings->addWidget(spinBoxMaxIt,4,1);
    QGroupBox * groupBoxSettings = new QGroupBox(widgetConfiguration);
    groupBoxSettings->setLayout(gridBoxSettings);

        //Buttons
    buttonLoad = new QPushButton("Laden",widgetConfiguration);
    buttonOptimization = new QPushButton("Optimieren starten",widgetConfiguration);

    QVector<double> ticks,ticksColorBar;
    QVector<QString> labels,labelsColorBar;
    ticks << 0 << 0.1 << 0.2 << 0.3 << 0.4 << 0.5
          << 0.6 << 0.7 << 0.8 << 0.9 << 1 ;
    labels << QString::number(0)   << QString::number(0.1)
           << QString::number(0.2) << QString::number(0.3)
           << QString::number(0.4) << QString::number(0.5)
           << QString::number(0.6) << QString::number(0.7)
           << QString::number(0.8) << QString::number(0.9)
           << QString::number(1);
    ticksColorBar << 0 << MaxTemperature * 0.1 << MaxTemperature * 0.2
                  << MaxTemperature * 0.3 << MaxTemperature * 0.4
                  << MaxTemperature * 0.5 << MaxTemperature * 0.6
                  << MaxTemperature * 0.7 << MaxTemperature * 0.8
                  << MaxTemperature * 0.9 << MaxTemperature;
    labelsColorBar << QString::number(0) << QString::number(MaxTemperature * 0.1)
                   << QString::number(MaxTemperature * 0.2) << QString::number(MaxTemperature * 0.3)
                   << QString::number(MaxTemperature * 0.4) << QString::number(MaxTemperature * 0.5)
                   << QString::number(MaxTemperature * 0.6) << QString::number(MaxTemperature * 0.7)
                   << QString::number(MaxTemperature * 0.8) << QString::number(MaxTemperature * 0.9)
                   << QString::number(MaxTemperature);

        //Platte
    plateOptimization = new QCustomPlot(widgetConfiguration,false);
            //Platte xAchse unten
    plateOptimization->xAxis->setAutoTicks(false);
    plateOptimization->xAxis->setAutoTickLabels(false);
    plateOptimization->xAxis->setTickVector(ticks);
    plateOptimization->xAxis->setTickVectorLabels(labels);
    plateOptimization->xAxis->setRange(0,1);
    plateOptimization->xAxis->grid()->setSubGridVisible(true);
            //Platte yAchse links
    plateOptimization->yAxis->setAutoTicks(false);
    plateOptimization->yAxis->setAutoTickLabels(false);
    plateOptimization->yAxis->setTickVector(ticks);
    plateOptimization->yAxis->setTickVectorLabels(labels);
    plateOptimization->yAxis->setRange(0,1);
    plateOptimization->yAxis->grid()->setSubGridVisible(true);
            //Platte xAchse oben
    plateOptimization->xAxis2->setVisible(true);
    plateOptimization->xAxis2->setAutoTicks(false);
    plateOptimization->xAxis2->setAutoTickLabels(false);
    plateOptimization->xAxis2->setTickVector(ticks);
    plateOptimization->xAxis2->setTickVectorLabels(labels);
    plateOptimization->xAxis2->setRange(0,1);
    plateOptimization->xAxis2->grid()->setSubGridVisible(true);
            //Platte yAchse rechts
    plateOptimization->yAxis2->setVisible(true);
    plateOptimization->yAxis2->setAutoTicks(false);
    plateOptimization->yAxis2->setAutoTickLabels(false);
    plateOptimization->yAxis2->setTickVector(ticks);
    plateOptimization->yAxis2->setTickVectorLabels(labels);
    plateOptimization->yAxis2->setRange(0,1);
    plateOptimization->yAxis2->grid()->setSubGridVisible(true);

            //ColorScale
    colorScale = new QCPColorScale(plateOptimization);
    plateOptimization->plotLayout()->addElement(0,1,colorScale);
    colorScale->setLabel("Robin Tim");

    QCPMarginGroup * group = new QCPMarginGroup(plateOptimization);
    plateOptimization->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, group);
    colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, group);
    colorScale->setGradient(QCPColorGradient::gpThermal);
    colorScale->setDataRange(QCPRange(0,MaxTemperature));
    colorScale->axis()->setAutoTicks(false);
    colorScale->axis()->setAutoTickLabels(false);
    colorScale->axis()->setTickVector(ticksColorBar);
    colorScale->axis()->setTickVectorLabels(labelsColorBar);
    colorScale->axis()->setRange(QCPRange(0,MaxTemperature));

    colorMap = new QCPColorMap(plateOptimization->yAxis,plateOptimization->xAxis);
    colorMap->data()->setRange(QCPRange(0,1),QCPRange(0,1));
    colorMap->setColorScale(colorScale);
    plateOptimization->addPlottable(colorMap);


    plateOptimization->setMinimumWidth(350);

        //ProgressBar
    progressBar = new QProgressBar(widgetConfiguration);

        //Tabelle
    tableWidgetData = new QTableWidget(widgetConfiguration);

        //Layout anwenden
    layoutConfigurationTab->addWidget(labelConfiguration,0,0);
    layoutConfigurationTab->addWidget(buttonLoad,1,0);
    layoutConfigurationTab->addWidget(buttonOptimization,1,1);
    layoutConfigurationTab->addWidget(groupBoxUserSettings,2,0,1,2);
    layoutConfigurationTab->addWidget(groupBoxSettings,2,2,1,2);
    layoutConfigurationTab->addWidget(plateOptimization,1,4,2,1);

    layoutConfigurationTab->addWidget(labelData,3,0);
    layoutConfigurationTab->addWidget(tableWidgetData,4,0,1,2);

    layoutConfigurationTab->addWidget(labelProgressBar,3,2);
    layoutConfigurationTab->addWidget(progressBar,3,3,1,2);




        //Labels







    //Tab Ergebnis
        //Label
    labelSolution = new QLabel("Gefittete Wärmeleitkoeffizienten",widgetSolution);
        //Table
    tableWidgetSolution = new QTableWidget(widgetSolution);

        //Layout anwenden
    layoutSolutionTab->addWidget(tableWidgetSolution,0,0);
    layoutSolutionTab->addWidget(tableWidgetSolution,1,0);



}
