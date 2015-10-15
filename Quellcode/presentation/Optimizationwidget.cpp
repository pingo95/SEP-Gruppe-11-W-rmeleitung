#include "Optimizationwidget.h"

#include "Controller.h"


presentation::OptimizationWidget::OptimizationWidget(QWidget *parent)
    : QTabWidget(parent), activeSubTab(OptimizationWidget::TabConfiguration),
      controller(NULL), model(NULL), updating(false), valueShift(1e-6)
{
        //Tab Konfig
    configurationTab = new QWidget(this);
    this->addTab(configurationTab,"Konfiguration");

    topLabelConfiguration = new QLabel("Dies ist der Tab zur Optimierung der "
                                       "Temperaturleitkoeffizienten.\nFür weitere "
                                       "Informationen wechseln Sie bitte in den Hi"
                                       "lfe-Tab.",configurationTab);
    topLabelConfiguration->setWordWrap(true);

    loadDataButton = new QPushButton("Messwerte laden",configurationTab);
    startOptimizationButton = new QPushButton("Optimieren starten",configurationTab);
    startOptimizationButton->setEnabled(false);

        //Override-Box
    overrideHeatSources = new QCheckBox("Nutze bereits vorhandene Wärmequellen "
                                        "zur Simulation",configurationTab);

    overrideThermalDiffusivities = new QCheckBox("Überschreibe bereits vorhandene "
                                                 "Wärmeleitkoeffizienten zur Simu"
                                                 "lation",configurationTab);

    labelInitialValue = new QLabel("Manueller Anfangswert",configurationTab);
    labelInitialValue->setAlignment(Qt::AlignRight);
    labelInitialValue->setEnabled(false);
    inputInitialValue = new QDoubleSpinBox(configurationTab);
    inputInitialValue->setMinimum(model::SimulationSetup::AreaMinValue[
                                  model::SimulationSetup::AreaThermalDiffusivity]/valueShift);
    inputInitialValue->setMaximum(model::SimulationSetup::AreaMaxValue[
                                  model::SimulationSetup::AreaThermalDiffusivity]/valueShift);
    inputInitialValue->setDecimals(0);
    inputInitialValue->setSuffix("\t[1e-6 m²/s]");
    inputInitialValue->setEnabled(false);
    inputInitialValue->setKeyboardTracking(false);
    inputInitialValue->setSingleStep(1);
    inputInitialValue->setValue(model::SimulationSetup::AreaMinValue[
                                model::SimulationSetup::AreaThermalDiffusivity]/valueShift);

    boxOverride = new QGroupBox(configurationTab);
    boxOverrideLayout = new QGridLayout();
    boxOverrideLayout->addWidget(overrideHeatSources,0,0,1,2);
    boxOverrideLayout->addWidget(overrideThermalDiffusivities,1,0,1,2);
    boxOverrideLayout->addWidget(labelInitialValue,2,0);
    boxOverrideLayout->addWidget(inputInitialValue,2,1);
    boxOverride->setLayout(boxOverrideLayout);

        //Settings-Box
    labelSettings = new QLabel("Simulationseinstellungen (Änderbar im "
                               "Tab \"Simulation\")",configurationTab);

    labelIntMethod = new QLabel("Integrationsmethode:",configurationTab);
    displayIntMethod = new QLineEdit(configurationTab);
    displayIntMethod->setReadOnly(true);

    labelM = new QLabel("M:",configurationTab);
    displayM = new QSpinBox(configurationTab);
    displayM->setMinimum(0);
    displayM->setMaximum(800);
    displayM->setReadOnly(true);
    displayM->setButtonSymbols(QAbstractSpinBox::NoButtons);

    labelT = new QLabel("T:",configurationTab);
    displayT = new QDoubleSpinBox(configurationTab);
    displayT->setDecimals(3);
    displayT->setMinimum(0);
    displayT->setMaximum(3600);
    displayT->setSuffix("s");
    displayT->setReadOnly(true);
    displayT->setButtonSymbols(QAbstractSpinBox::NoButtons);

    labelSolver = new QLabel("Löser:",configurationTab);
    displaySolver = new QLineEdit(configurationTab);
    displaySolver->setReadOnly(true);

    labelMaxError = new QLabel("Relative Genauigkeit:\t\t\t1e-",configurationTab);
    displayMaxError = new QDoubleSpinBox(configurationTab);
    displayMaxError->setDecimals(0);
    displayMaxError->setMinimum(2);
    displayMaxError->setMaximum(10);
    displayMaxError->setReadOnly(true);
    displayMaxError->setButtonSymbols(QAbstractSpinBox::NoButtons);

    labelMaxIt = new QLabel("Maximale Iterationsanzahl:",configurationTab);
    displayMaxIt = new QSpinBox(configurationTab);
    displayMaxIt->setMinimum(0);
    displayMaxIt->setMaximum(1000);
    displayMaxIt->setReadOnly(true);
    displayMaxIt->setButtonSymbols(QAbstractSpinBox::NoButtons);

    boxSettings = new QGroupBox(configurationTab);
    boxSettingsLayout = new QGridLayout();
    boxSettingsLayout->addWidget(labelSettings,0,0);
    boxSettingsLayout->addWidget(labelIntMethod,1,0);
    boxSettingsLayout->addWidget(displayIntMethod,1,1);
    boxSettingsLayout->addWidget(labelM,2,0);
    boxSettingsLayout->addWidget(displayM,2,1);
    boxSettingsLayout->addWidget(labelT,3,0);
    boxSettingsLayout->addWidget(displayT,3,1);
    boxSettingsLayout->addWidget(labelSolver,4,0);
    boxSettingsLayout->addWidget(displaySolver,4,1);
    boxSettingsLayout->addWidget(labelMaxError,5,0);
    boxSettingsLayout->addWidget(displayMaxError,5,1);
    boxSettingsLayout->addWidget(labelMaxIt,6,0);
    boxSettingsLayout->addWidget(displayMaxIt,6,1);
    boxSettings->setLayout(boxSettingsLayout);

        //Für Platte
    QVector<double> ticks,ticksColorBar;
    QVector<QString> tickLabels,tickLabelsColorBar;
    double valueSpan = model::SimulationSetup::AreaMaxValue[
            model::SimulationSetup::AreaThermalDiffusivity] -
            model::SimulationSetup::AreaMinValue[
            model::SimulationSetup::AreaThermalDiffusivity];
    for(int i = 0; i < 11; ++i)
    {
        ticks << (double) i / 10;
        tickLabels << QString::number(ticks[i]);

        ticksColorBar << round((valueSpan * ticks[i] +
                                model::SimulationSetup::AreaMinValue[
                                model::SimulationSetup::AreaThermalDiffusivity])/valueShift);
        tickLabelsColorBar << QString::number(ticksColorBar[i]);
    }

        //Platte
    plate = new QCustomPlot(configurationTab,false);
    plate->setMinimumWidth(300);
    plate->setMinimumHeight(300);
            //Platte xAchse unten
    plate->xAxis->setAutoTicks(false);
    plate->xAxis->setAutoTickLabels(false);
    plate->xAxis->setTickVector(ticks);
    plate->xAxis->setTickVectorLabels(tickLabels);
    plate->xAxis->setRange(0,1);
    plate->xAxis->grid()->setSubGridVisible(true);
            //Platte yAchse links
    plate->yAxis->setAutoTicks(false);
    plate->yAxis->setAutoTickLabels(false);
    plate->yAxis->setTickVector(ticks);
    plate->yAxis->setTickVectorLabels(tickLabels);
    plate->yAxis->setRange(0,1);
    plate->yAxis->grid()->setSubGridVisible(true);
            //Platte xAchse oben
    plate->xAxis2->setVisible(true);
    plate->xAxis2->setAutoTicks(false);
    plate->xAxis2->setAutoTickLabels(false);
    plate->xAxis2->setTickVector(ticks);
    plate->xAxis2->setTickVectorLabels(tickLabels);
    plate->xAxis2->setRange(0,1);
    plate->xAxis2->grid()->setSubGridVisible(true);
            //Platte yAchse rechts
    plate->yAxis2->setVisible(true);
    plate->yAxis2->setAutoTicks(false);
    plate->yAxis2->setAutoTickLabels(false);
    plate->yAxis2->setTickVector(ticks);
    plate->yAxis2->setTickVectorLabels(tickLabels);
    plate->yAxis2->setRange(0,1);
    plate->yAxis2->grid()->setSubGridVisible(true);

            //ColorScale
    colorScale = new QCPColorScale(plate);
    plate->plotLayout()->addElement(0,1,colorScale);
    colorScale->setLabel("Temperaturleitkoeffizienten\n[1e-6 m²/s]");

    QCPMarginGroup * group = new QCPMarginGroup(plate);
    plate->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, group);
    colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, group);
    colorScale->setGradient(QCPColorGradient::gpThermal);
    QCPRange range(model::SimulationSetup::AreaMinValue[
                        model::SimulationSetup::AreaThermalDiffusivity]/valueShift,
                   model::SimulationSetup::AreaMaxValue[
                        model::SimulationSetup::AreaThermalDiffusivity]/valueShift);
    colorScale->setDataRange(range);
    colorScale->axis()->setAutoTicks(false);
    colorScale->axis()->setAutoTickLabels(false);
    colorScale->axis()->setTickVector(ticksColorBar);
    colorScale->axis()->setTickVectorLabels(tickLabelsColorBar);
    colorScale->axis()->setRange(range);

    colorMap = new QCPColorMap(plate->yAxis,plate->xAxis);
    colorMap->data()->setRange(QCPRange(0,1),QCPRange(0,1));
    colorMap->setColorScale(colorScale);
    plate->addPlottable(colorMap);

        //Daten-Tabelle
    labelData = new QLabel("Eingelesene Messdaten (in Kelvin):",configurationTab);
    dataTable = new QTableWidget(configurationTab);

        //Fortschritts-Balken
    labelMainProgressBar = new QLabel("Fortschrittsbalken:",configurationTab);
    mainProgressBar = new QProgressBar(configurationTab);
    labelSubProgressBar = new QLabel("",configurationTab);
    subProgressBar = new QProgressBar(configurationTab);

        //Platzhalter
    spacerItem = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);

        //Layout
    configurationTabLayout = new QGridLayout(configurationTab);

    configurationTabLayout->addWidget(topLabelConfiguration,0,0,1,4);
    configurationTabLayout->addWidget(loadDataButton,1,0);
    configurationTabLayout->addWidget(startOptimizationButton,1,1);
    configurationTabLayout->addWidget(labelData,1,2);
    configurationTabLayout->addWidget(boxOverride,2,0,1,2);
    configurationTabLayout->addWidget(dataTable,2,2,3,2);
    configurationTabLayout->addWidget(boxSettings,3,0,1,2);
    configurationTabLayout->addWidget(plate,4,0,3,2);
    configurationTabLayout->addWidget(labelMainProgressBar,5,2);
    configurationTabLayout->addWidget(mainProgressBar,5,3);
    configurationTabLayout->addWidget(labelSubProgressBar,6,2);
    configurationTabLayout->addWidget(subProgressBar,6,3);
    configurationTabLayout->addItem(spacerItem,7,0);

    configurationTabLayout->setColumnStretch(0,0);
    configurationTabLayout->setColumnStretch(1,0);
    configurationTabLayout->setColumnStretch(2,0);
    configurationTabLayout->setColumnStretch(3,1);

    configurationTabLayout->setRowStretch(0,0);
    configurationTabLayout->setRowStretch(1,0);
    configurationTabLayout->setRowStretch(2,0);
    configurationTabLayout->setRowStretch(3,0);
    configurationTabLayout->setRowStretch(4,0);
    configurationTabLayout->setRowStretch(5,0);
    configurationTabLayout->setRowStretch(6,0);
    configurationTabLayout->setRowStretch(7,1);

//    configurationTabLayout->setColumnMinimumWidth(0,200);
//    configurationTabLayout->setColumnMinimumWidth(1,200);

    //Tab Ergebnis
    solutionTab = new QWidget(this);
    this->addTab(solutionTab,"Ergebnis");

    topLabelSolution = new QLabel("Dies ist der Tab zum Anzeigen der Optimerungsergebnisse."
                                  "\nNach einer Optimierung werden die Ergebnisse "
                                  "automatisch hier angezeigt (in 1e-6 m²/s).",solutionTab);

    solutionTable = new QTableWidget(solutionTab);

    solutionTabLayout = new QVBoxLayout(solutionTab);

    solutionTabLayout->addWidget(topLabelSolution,0);
    solutionTabLayout->addWidget(solutionTable,1);

    // interne Signal & Slots verbinden
    connect(this,SIGNAL(currentChanged(int)),this,SLOT(transformTabIDSlot(int)));

    connect(inputInitialValue,SIGNAL(valueChanged(double)),this,SLOT(shiftInitialValueSlot(double)));
#ifdef _WIN32
 topLabelConfiguration->setText("Dies ist der Tab zur Optimierung der Temperaturleitkoeffizienten."
                                "\nDie Optimierung der Temperaturleitkoeffizienten ist unter "
                                "Windows nicht möglich, um diese Funktionalität nutzen zu "
                                "können, muss ein Linux-System genutzt werden.");
 loadDataButton->setEnabled(false);
 startOptimizationButton->setEnabled(false);
 topLabelSolution->setText("Dies ist der Tab zum Anzeigen der Optimerungsergebnisse."
                           "\nDie Optimierung der Temperaturleitkoeffizienten ist unter "
                           "Windows nicht möglich, um diese Funktionalität nutzen zu "
                           "können, muss ein Linux-System genutzt werden.");
 overrideHeatSources->setEnabled(false);
 overrideThermalDiffusivities->setEnabled(false);
#endif
}

void presentation::OptimizationWidget::setController(Controller *controller)
{
    this->controller = controller;

    connect(this,SIGNAL(subTabChange(int)),controller,SLOT(tabChangedSlot(int)));

    connect(loadDataButton,SIGNAL(clicked(bool)),controller,SLOT(loadObservationsSlot()));
    connect(startOptimizationButton,SIGNAL(clicked(bool)),controller,SLOT(optimizationSlot()));

    connect(overrideHeatSources,SIGNAL(clicked(bool)),controller,SLOT(useHeatSourcesSlot(bool)));
    connect(overrideThermalDiffusivities,SIGNAL(clicked(bool)),controller,SLOT(overrideThermalDiffusivitiesSlot(bool)));
    connect(this,SIGNAL(newInitialValue(double)),controller,SLOT(newOverrideValueSlot(double)));
}

void presentation::OptimizationWidget::setModel(model::Model *model)
{
    this->model = model;
}

void presentation::OptimizationWidget::update()
{
    if(updating)
        return;
    updating = true;
    if(activeSubTab == OptimizationWidget::TabConfiguration)
    {
        //Config Tab updaten
#ifndef _WIN32
        if(model->isWorking())
        {
            //label ändern:
            topLabelConfiguration->setText("Es wird gerade eine Simulation oder Optimierung durchgeführt.");

            // Knöpfe deaktivieren:
            loadDataButton->setEnabled(false);
            startOptimizationButton->setEnabled(false);
        }
        else
        {
            //label ändern:
            topLabelConfiguration->setText("Dies ist der Tab zur Optimierung der "
                                           "Temperaturleitkoeffizienten.\nFür weitere "
                                           "Informationen wechseln Sie bitte in den Hi"
                                           "lfe-Tab.");

            // Knöpfe aktivieren:
            loadDataButton->setEnabled(true);
            if(model->getDataRead())
                startOptimizationButton->setEnabled(true);
        }
#endif

        inputInitialValue->setValue(model->getOverrideValue()/valueShift);
        if(model->getOverrideThermalDiffusivities())
        {
            overrideThermalDiffusivities->setChecked(true);
            labelInitialValue->setEnabled(true);
            inputInitialValue->setEnabled(true);
        }
        else
        {
            overrideThermalDiffusivities->setChecked(false);
            labelInitialValue->setEnabled(false);
            inputInitialValue->setEnabled(false);
        }
        overrideHeatSources->setChecked(model->getUseHeatSources());

        //Übernommene Werte aktualisieren
        displayIntMethod->setText(model->getSimulationSetup()->getSelectedIntMethod());
        displayM->setValue(model->getSimulationSetup()->getM());
        displayT->setValue(model->getSimulationSetup()->getT());
        displaySolver->setText(model->getSimulationSetup()->getSelectedSolver());
        int exp = 1;
        double tmp = model->getSimulationSetup()->getSolverMaxError();
        while(tmp < 1e-1)
        {
            tmp *= 10;
            ++exp;
        }
        displayMaxError->setValue(exp);
        displayMaxIt->setValue(model->getSimulationSetup()->getSolverMaxIt());

        //TODO: Endergebnis oder Anfangswert auf platte darstellen valueShift!!!
        if(model->getOverrideThermalDiffusivities())
        {
            QCPRange range(model::SimulationSetup::AreaMinValue[
                                model::SimulationSetup::AreaThermalDiffusivity]/valueShift,
                           model::SimulationSetup::AreaMaxValue[
                                model::SimulationSetup::AreaThermalDiffusivity]/valueShift);
            colorScale->axis()->setRange(range);
            colorMap->data()->setSize(1,1);
            colorMap->data()->setCell(0,0,model->getOverrideValue()/valueShift);
            colorScale->setLabel("(Initiale) Temperaturleitkoeffizienten\n[1e-6 m²/s]");
            plate->replot();
        }
        else
        {
            colorMap->setVisible(false);
            plate->replot();
        }

        //TODO: Falls geladen, Messungen in Tabelle darstellen
        if(!model->isWorking() && model->getDataRead())
        {
            int count = model->getObservationsDim();
            double ** observations = model->getObservations();

            dataTable->clear();
            dataTable->setColumnCount(count);
            dataTable->setRowCount(count);
            for(int i = count-1; i >= 0; --i)
            {
                dataTable->setColumnWidth(count-1-i,70);
                for(int j = 0; j < count; ++j)
                {
                    QTableWidgetItem * tmpItemPtr = new QTableWidgetItem(QString::number(observations[i][j]));
                    tmpItemPtr->setFlags(Qt::ItemIsEnabled);
                    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    dataTable->setItem(count-1-i,j,tmpItemPtr);
                }
            }
            dataTable->horizontalHeader()->setSectionsClickable(false);
            dataTable->verticalHeader()->setSectionsClickable(false);
        }


    }
    else
    {
        // Ergebnis Tab updaten plus ColorMap im anderen Tab
        if(!model->isWorking() && model->getOptimized())
        {
            int n = model->getObservationsDim();
            QVector<double> result(model->getOptimizedCoeffs());
            solutionTable->clear();
            solutionTable->setRowCount(n);
            solutionTable->setColumnCount(n);

            colorMap->data()->setSize(n,n);
            colorScale->setLabel("Optimierte Temperaturleitkoeffizienten\n[1e-6 m²/s]");

            for(int i = n-1; i >= 0; --i)
            {
                solutionTable->setColumnWidth(n-1-i,70);
                for(int j = 0; j < n; ++ j)
                {
                    QTableWidgetItem * tmpItemPtr = new QTableWidgetItem(QString::number(result[n*i+j]/valueShift));
                    tmpItemPtr->setFlags(Qt::ItemIsEnabled);
                    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    solutionTable->setItem(n-1-i,j,tmpItemPtr);
                    colorMap->data()->setCell(i,j,result[n*i+j]/valueShift);
                }
            }

            solutionTable->horizontalHeader()->setSectionsClickable(false);
            solutionTable->verticalHeader()->setSectionsClickable(false);

            colorMap->setVisible(true);
            colorMap->rescaleDataRange(true);
            plate->replot();
        }
    }
    updating = false;
}

void presentation::OptimizationWidget::nextMainStage(QString stage, int maximum)
{
    mainProgressBar->setMaximum(maximum);
    mainProgressBar->setMinimum(0);
    mainProgressBar->setValue(0);
    labelMainProgressBar->setText(stage);
}

void presentation::OptimizationWidget::nextSubStage(QString stage, int maximum)
{
    subProgressBar->setMaximum(maximum);
    subProgressBar->setMinimum(0);
    subProgressBar->setValue(0);
    labelSubProgressBar->setText(stage);
}

void presentation::OptimizationWidget::updateMainProgress(int step)
{
    mainProgressBar->setValue(step);
}

void presentation::OptimizationWidget::updateSubProgress(int step)
{
    subProgressBar->setValue(step);
}

void presentation::OptimizationWidget::transformTabIDSlot(int targetTab)
{
    activeSubTab = targetTab;
    emit subTabChange(UI::TabParameterFitting);
}

void presentation::OptimizationWidget::shiftInitialValueSlot(double newValue)
{
    emit newInitialValue(newValue*1e-6);
}
