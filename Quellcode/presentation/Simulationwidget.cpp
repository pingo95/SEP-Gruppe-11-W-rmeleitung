#include "Simulationwidget.h"

#include "Controller.h"

presentation::SimulationWidget::SimulationWidget(QWidget *parent)
    : QWidget(parent), controller(NULL), model(NULL)
{
        //Top-Label
    topLabel = new QLabel("",this);
    topLabel->setWordWrap(true);

        //IntMethod-Box
    labelBoxIntMethod = new QLabel("Zeitdiskretisierung",this);

    labelIntMethod = new QLabel("Integrationsmethode auswählen",this);
    inputIntMethod = new QComboBox(this);

    labelM = new QLabel("Zeitschritte M eingeben",this);
    inputM = new QSpinBox(this);
    inputM->setMinimum(0);
    inputM->setMaximum(800);
    inputM->setKeyboardTracking(false);

    labelT = new QLabel("Endzeitpunkt T eingeben",this);
    inputT = new QDoubleSpinBox(this);
    inputT->setDecimals(3);
    inputT->setMinimum(0);
    inputT->setMaximum(3600);
    inputT->setSuffix("s");
    inputT->setKeyboardTracking(false);

    boxIntMethod = new QGroupBox(this);
    boxIntMethodLayout = new QGridLayout();
    boxIntMethodLayout->addWidget(labelBoxIntMethod,0,0,1,2);
    boxIntMethodLayout->addWidget(labelIntMethod,1,0);
    boxIntMethodLayout->addWidget(inputIntMethod,1,1);
    boxIntMethodLayout->addWidget(labelM,2,0);
    boxIntMethodLayout->addWidget(inputM,2,1);
    boxIntMethodLayout->addWidget(labelT,3,0);
    boxIntMethodLayout->addWidget(inputT,3,1);
    boxIntMethod->setLayout(boxIntMethodLayout);

        //Solver-Box
    labelBoxSolver = new QLabel("LGS Löser",this);

    labelSolver = new QLabel("Löser auswählen",this);
    inputSolver = new QComboBox(this);

    labelMaxError = new QLabel("Relative Genauigkeit:\t1e-",this);
    inputMaxError = new QSpinBox(this);
    inputMaxError->setMinimum(2);
    inputMaxError->setMaximum(10);
    inputMaxError->setKeyboardTracking(false);

    labelMaxIt = new QLabel("Maximale Iterationszahl eingeben",this);
    inputMaxIt = new QSpinBox(this);
    inputMaxIt->setMinimum(1);
    inputMaxIt->setMaximum(1000);
    inputMaxIt->setKeyboardTracking(false);

    boxSolver = new QGroupBox(this);
    boxSolverLayout = new QGridLayout();
    boxSolverLayout->addWidget(labelBoxSolver,0,0,1,2);
    boxSolverLayout->addWidget(labelSolver,1,0);
    boxSolverLayout->addWidget(inputSolver,1,1);
    boxSolverLayout->addWidget(labelMaxError,2,0);
    boxSolverLayout->addWidget(inputMaxError,2,1);
    boxSolverLayout->addWidget(labelMaxIt,3,0);
    boxSolverLayout->addWidget(inputMaxIt,3,1);
    boxSolver->setLayout(boxSolverLayout);

        //Simulate-Box
    labelBoxSimulate = new QLabel("Ortsdiskretisierung",this);

    labelN = new QLabel("Stützstellen N eingeben",this);
    labelN->setMaximumWidth(175);
    inputN = new QSpinBox(this);
    inputN->setMaximum(0);
    inputN->setMaximum(800);
    inputN->setKeyboardTracking(false);
    inputN->setMaximumWidth(50);
    simulateButton = new QPushButton("Simulieren",this);
    abortButton = new QPushButton("Simulation abbrechen",this);
    abortButton->setEnabled(false);

    boxSimulate = new QGroupBox(this);
    boxSimulateLayout = new QGridLayout();
    boxSimulateLayout->addWidget(labelBoxSimulate,0,0,1,2);
    boxSimulateLayout->addWidget(labelN,1,0);
    boxSimulateLayout->addWidget(inputN,1,1);
    boxSimulateLayout->addWidget(simulateButton,2,0,1,2);
    boxSimulateLayout->addWidget(abortButton,3,0,1,2);
    boxSimulate->setLayout(boxSimulateLayout);

        //Save-Load-Box
    labelBoxSaveLoad = new QLabel("Simulationseinstellungen", this);

    saveButton = new QPushButton("Speichern",this);
    saveButton->setEnabled(false);
    loadButton = new QPushButton("Laden",this);
    loadButton->setEnabled(false);
    resetButton = new QPushButton("Zurücksetzen",this);

    boxSaveLoad = new QGroupBox(this);
    boxSaveLoadLayout = new QGridLayout();
    boxSaveLoadLayout->addWidget(labelBoxSaveLoad,0,0);
    boxSaveLoadLayout->addWidget(saveButton,1,0);
    boxSaveLoadLayout->addWidget(loadButton,2,0);
    boxSaveLoadLayout->addWidget(resetButton,3,0);
    boxSaveLoad->setLayout(boxSaveLoadLayout);

        //SimulationLog
    simulationLog = new QTextEdit(this);
    simulationLog->setReadOnly(true);

        //ProgressBar
    labelProgressBar = new QLabel("Fortschrittsbalken",this);
    progressBar =  new QProgressBar(this);

        //Layout
    layout = new QGridLayout(this);

    layout->addWidget(topLabel,0,0,1,2);
    layout->addWidget(boxIntMethod,1,0,4,1);
    layout->addWidget(boxSolver,1,1,4,1);
    layout->addWidget(boxSimulate,1,2,4,1);
    layout->addWidget(boxSaveLoad,1,3,4,1);
    layout->addWidget(simulationLog,5,0,1,5);
    layout->addWidget(labelProgressBar,6,0);
    layout->addWidget(progressBar,6,1,1,4);

    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,0);
    layout->setColumnStretch(2,0);
    layout->setColumnStretch(3,0);
    layout->setColumnStretch(4,1);
}

void presentation::SimulationWidget::setController(Controller *controller)
{
    this->controller = controller;

    //TODO maxError und MAxIt & save load
    connect(inputIntMethod,SIGNAL(activated(QString)),controller,SLOT(selectIntMethodSlot(QString)));
    connect(inputM,SIGNAL(valueChanged(int)),controller,SLOT(newMSlot(int)));
    connect(inputT,SIGNAL(valueChanged(double)),controller,SLOT(newTSlot(double)));

    connect(inputSolver,SIGNAL(activated(QString)),controller,SLOT(selectSolverSlot(QString)));
    connect(this,SIGNAL(newMaxErrorValue(double)),controller,SLOT(newMaxErrorSlot(double)));
    connect(inputMaxIt,SIGNAL(valueChanged(int)),controller,SLOT(newMaxItSlot(int)));

    connect(inputN,SIGNAL(valueChanged(int)),controller,SLOT(newNSlot(int)));
    connect(simulateButton,SIGNAL(clicked(bool)),controller,SLOT(simulateSlot()));
    connect(resetButton,SIGNAL(clicked(bool)),controller,SLOT(resetSetupSlot()));
}

void presentation::SimulationWidget::setModel(model::Model *model)
{
    this->model = model;
    QList<QString> tmpList = model->getIntMethodsNames();
    QList<QString>::const_iterator it = tmpList.begin();
    for(; it < tmpList.end(); ++it)
        inputIntMethod->addItem((*it));
    tmpList = model->getSolverNames();
    it = tmpList.begin();
    for(; it < tmpList.end(); ++it)
        inputSolver->addItem((*it));
}

void presentation::SimulationWidget::update()
{
    inputIntMethod->setCurrentText(model->getSimulationSetup()->getSelectedIntMethod());
    inputM->setValue(model->getSimulationSetup()->getM());
    inputT->setValue(model->getSimulationSetup()->getT());

    inputSolver->setCurrentText(model->getSimulationSetup()->getSelectedSolver());
    inputMaxError->setValue(-1*log(model->getSimulationSetup()->getSolverMaxError()));
    inputMaxIt->setValue(model->getSimulationSetup()->getSolverMaxIt());

    inputN->setValue(model->getSimulationSetup()->getN());

    if(model->isWorking())
    {
        topLabel->setText("Es wird zur Zeit simuliert.");
        simulateButton->setEnabled(false);
        abortButton->setEnabled(true);
    }
    else
    {
       topLabel->setText("Dies ist der Tab zur Einstellung der Simulation.\n"
                         "Für weitere Informationen wechseln Sie in den Hilfe-Tab.");
       simulateButton->setEnabled(true);
       abortButton->setEnabled(false);
       if(model->getSimulated())
           labelProgressBar->setText("Simulation beendet");

    }
}

void presentation::SimulationWidget::appendToSimulationLog(QString text)
{
    simulationLog->append(text);
}

void presentation::SimulationWidget::nextStage(QString stage, int maximum)
{
    progressBar->setMaximum(maximum);
    progressBar->setMinimum(0);
    progressBar->setValue(0);
    labelProgressBar->setText(stage);
}

void presentation::SimulationWidget::updateProgress(int step)
{
    progressBar->setValue(step);
}

void presentation::SimulationWidget::transformMaxError(int value)
{
    emit newMaxErrorValue(pow(10,-1*value));
}
