#include "Optimizationwidget.h"

#include "Controller.h"


presentation::OptimizationWidget::OptimizationWidget(QWidget *parent)
    : QTabWidget(parent), activeSubTab(OptimizationWidget::TabConfiguration),
      controller(NULL), model(NULL), valueShift(1e-6)
{
        //Tab Konfig
    configurationTab = new QWidget(this);
    this->addTab(configurationTab,"Konfiguration");

    topLabelConfiguration = new QLabel("Dies ist der Tab zur Optimierung der "
                                       "Temperaturleitkoeffizienten.\nFür weitere "
                                       "Informationen wechseln Sie bitte in den Hi"
                                       "lfe-Tab.",configurationTab);
    topLabelConfiguration->setWordWrap(true);

    loadDataButton = new QPushButton("Laden",configurationTab);
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
    inputInitialValue->setSuffix(" [1e-6 m²/s]");
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

    labelMaxError = new QLabel("Epsilon:",configurationTab);
    displayMaxError = new QDoubleSpinBox(configurationTab);
    displayMaxError->setDecimals(10);
    displayMaxError->setMinimum(1e-10);
    displayMaxError->setMaximum(1e-5);
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
    labelData = new QLabel("Temperaturverteilung (in Kelvin):",configurationTab);
    dataTable = new QTableWidget(configurationTab);

        //Fortschritts-Balken
    labelProgressBar = new QLabel("Fortschrittsbalken:",configurationTab);
    progressBar = new QProgressBar(configurationTab);

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
    configurationTabLayout->addWidget(plate,4,0,2,2);
    configurationTabLayout->addWidget(labelProgressBar,5,2);
    configurationTabLayout->addWidget(progressBar,5,3);
    configurationTabLayout->addItem(spacerItem,6,0);

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
    configurationTabLayout->setRowStretch(6,1);

//    configurationTabLayout->setColumnMinimumWidth(0,200);
//    configurationTabLayout->setColumnMinimumWidth(1,200);

    //Tab Ergebnis
    solutionTab = new QWidget(this);
    this->addTab(solutionTab,"Ergebnis");

    topLabelSolution = new QLabel("Es kann erst ein Ergebnis angezeigt werden, "
                                  "nachdem eine Optimierung durchgeführt wurde.",solutionTab);

    solutionTable = new QTableWidget(solutionTab);

    solutionTabLayout = new QVBoxLayout(solutionTab);

    solutionTabLayout->addWidget(topLabelSolution,0);
    solutionTabLayout->addWidget(solutionTable,1);

    // interne Signal & Slots verbinden
    connect(this,SIGNAL(currentChanged(int)),this,SLOT(transformTabIDSlot(int)));

    connect(inputInitialValue,SIGNAL(valueChanged(double)),this,SLOT(shiftInitialValue(double)));
}

void presentation::OptimizationWidget::setController(Controller *controller)
{
    this->controller = controller;

    connect(this,SIGNAL(subTabChange(int)),controller,SLOT(tabChangedSlot(int)));

    connect(loadDataButton,SIGNAL(clicked(bool)),controller,SLOT(loadObservationsSlot()));
    connect(startOptimizationButton,SIGNAL(clicked(bool)),controller,SLOT(optimizationSlot()));

    connect(overrideHeatSources,SIGNAL(clicked(bool)),controller,SLOT(useHeatSourcesSlot(bool)));
    connect(overrideThermalDiffusivities,SIGNAL(clicked(bool)),controller,SLOT(overrideThermalDiffusivities(bool)));
    connect(this,SIGNAL(newInitialValue(double)),controller,SLOT(newOverrideValue(double)));
}

void presentation::OptimizationWidget::setModel(model::Model *model)
{
    this->model = model;
}

void presentation::OptimizationWidget::update()
{
    if(activeSubTab == OptimizationWidget::TabConfiguration)
    {
        //Config Tab updaten
        //überprüfen ob gerade optimiert wird: i.e. model->getOptimizing()
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

        //TODO: Override inputs werte aktualisieren temporär:
        inputInitialValue->setValue(model->getOverrideValue()/valueShift);
        if(model->getOverrideThermalDiffusivities())
        {
            labelInitialValue->setEnabled(true);
            inputInitialValue->setEnabled(true);
        }
        else
        {
            labelInitialValue->setEnabled(false);
            inputInitialValue->setEnabled(false);
        }

        //Übernommene Werte aktualisieren
        displayIntMethod->setText(model->getSimulationSetup()->getSelectedIntMethod());
        displayM->setValue(model->getSimulationSetup()->getM());
        displayT->setValue(model->getSimulationSetup()->getT());
        displaySolver->setText(model->getSimulationSetup()->getSelectedSolver());
        displayMaxError->setValue(model->getSimulationSetup()->getSolverMaxError());
        displayMaxIt->setValue(model->getSimulationSetup()->getSolverMaxIt());

        //TODO: Endergebnis oder Anfangswert auf platte darstellen valueShift!!!
        colorMap->data()->setSize(1,1);
        colorMap->data()->setCell(0,0,model->getOverrideValue()/valueShift);
        colorScale->setLabel("(Initiale) Temperaturleitkoeffizienten\n[1e-6 m²/s]");
        plate->replot();

        //TODO: Falls geladen, Messungen in Tabelle darstellen
        if(model->getDataRead())
        {
            int count = model->getObservationsDim();
            double ** observations = model->getObservations();

            dataTable->clear();
            dataTable->setColumnCount(count);
            dataTable->setRowCount(count);
            QStringList header;
            for(int i = 0; i < count; ++i)
            {
                header << QString::number((double) i * 1./(double)(count-1));
                for(int j = 0; j < count; ++ j)
                {
                    QTableWidgetItem * tmpItemPtr = new QTableWidgetItem(QString::number(observations[i][j]));
                    tmpItemPtr->setFlags(Qt::ItemIsEnabled);
                    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    dataTable->setItem(i,j,tmpItemPtr);
                }
            }
            dataTable->setHorizontalHeaderLabels(header);
            dataTable->horizontalHeader()->setSectionsClickable(false);
            dataTable->setVerticalHeaderLabels(header);
            dataTable->verticalHeader()->setSectionsClickable(false);
        }


    }
    else
    {
        // Ergebnis Tab updaten
        // 1.Schritt Test ob schon optimiert wurde, i.e. if(model->getOptimized())
        // 2.Schritt Ergegnis n holen, i.e. int n = model->getOptimizationN();
        // 3.Schritt Ergebnis Referenz holen, i.e. double ** & result = model->getOptimizationResult();
        // 4.Schritt Ergebnis in Tabelle einfügen:
//        solutionTable->clear();
//        solutionTable->setRowCount(n);
//        solutionTable->setColumnCount(n);
//        QStringList header;
//        for(int i = 0; i < n; ++i)
//        {
//            header << QString::number((double) i * 1./(double)(n-1));
//            for(int j = 0; j < n; ++ j)
//            {
//                QTableWidgetItem * tmpItemPtr = new QTableWidgetItem(QString::number(result[i][j]/valueShift));
//                tmpItemPtr->setFlags(Qt::ItemIsEnabled);
//                tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//                solutionTable->setItem(i,j,tmpItemPtr);
//            }
//        }
//        solutionTable->setHorizontalHeaderLabels(header);
//        solutionTable->horizontalHeader()->setSectionsClickable(false);
//        solutionTable->setVerticalHeaderLabels(header);
//        solutionTable->verticalHeader()->setSectionsClickable(false);
    }
}

void presentation::OptimizationWidget::nextStage(QString stage, int maximum)
{
    progressBar->setMaximum(maximum);
    progressBar->setMinimum(0);
    progressBar->setValue(0);
    labelProgressBar->setText(stage);
}

void presentation::OptimizationWidget::updateProgress(int step)
{
    progressBar->setValue(step);
}

void presentation::OptimizationWidget::transformTabIDSlot(int targetTab)
{
    activeSubTab = targetTab;
    emit subTabChange(UI::TabParameterFitting);
}

void presentation::OptimizationWidget::shiftInitialValue(double newValue)
{
    emit newInitialValue(newValue*1e-6);
}
