#include "Simulationwidget.h"

#include "Controller.h"

presentation::SimulationWidget::SimulationWidget(QWidget *parent)
    : QWidget(parent), controller(NULL), model(NULL)
{
        //Top-Label
    topLabel = new QLabel("",this);
    topLabel->setWordWrap(true);

        //IntMethod-Box
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
    inputT->setMaximum(1000);
    inputT->setSuffix("s");
    inputT->setKeyboardTracking(false);

    boxIntMethod = new QGroupBox(this);
    boxIntMethodLayout = new QGridLayout();
    boxIntMethodLayout->addWidget(labelIntMethod,0,0);
    boxIntMethodLayout->addWidget(inputIntMethod,0,1);
    boxIntMethodLayout->addWidget(labelM,1,0);
    boxIntMethodLayout->addWidget(inputM,1,1);
    boxIntMethodLayout->addWidget(labelT,2,0);
    boxIntMethodLayout->addWidget(inputT,2,1);
    boxIntMethod->setLayout(boxIntMethodLayout);

        //Solver-Box
    labelSolver = new QLabel("Löser auswählen",this);
    inputSolver = new QComboBox(this);

    labelMaxError = new QLabel("Epsilon eingeben",this);
    inputMaxError = new QDoubleSpinBox(this);
    inputMaxError->setDecimals(10);
    inputMaxError->setMinimum(1e-10);
    inputMaxError->setMaximum(1e-5);
    inputMaxError->setKeyboardTracking(false);
    inputMaxError->setSingleStep(1e-10);

    labelMaxIt = new QLabel("Maximale Iterationszahl eingeben",this);
    inputMaxIt = new QSpinBox(this);
    inputMaxIt->setMinimum(1);
    inputMaxIt->setMaximum(1000);
    inputMaxIt->setKeyboardTracking(false);

    boxSolver = new QGroupBox(this);
    boxSolverLayout = new QGridLayout();
    boxSolverLayout->addWidget(labelSolver,0,0);
    boxSolverLayout->addWidget(inputSolver,0,1);
    boxSolverLayout->addWidget(labelMaxError,1,0);
    boxSolverLayout->addWidget(inputMaxError,1,1);
    boxSolverLayout->addWidget(labelMaxIt,2,0);
    boxSolverLayout->addWidget(inputMaxIt,2,1);
    boxSolver->setLayout(boxSolverLayout);

        //Ortsdiskretisierung
    labelN = new QLabel("Stützstellen N eingeben",this);
    labelN->setMaximumWidth(175);
    inputN = new QSpinBox(this);
    inputN->setMaximum(0);
    inputN->setMaximum(800);
    inputN->setKeyboardTracking(false);
    inputN->setMaximumWidth(50);

        //Simulate-Box
    saveButton = new QPushButton(this);
    saveButton->setIcon(QIcon(":/Icons/save"));
    saveButton->setText("Speichern");
    saveButton->setIconSize(QSize(15,15));
    saveButton->setEnabled(false);
    loadButton = new QPushButton(this);
    loadButton->setIcon(QIcon(":/Icons/load"));
    loadButton->setText("Laden");
    loadButton->setIconSize(QSize(15,15));
    loadButton->setEnabled(false);
    simulateButton = new QPushButton("Simulieren",this);

    boxSimulate = new QGroupBox(this);
    boxSimulateLayout = new QVBoxLayout();
    boxSimulateLayout->addWidget(saveButton);
    boxSimulateLayout->addWidget(loadButton);
    boxSimulateLayout->addWidget(simulateButton);
    boxSimulate->setLayout(boxSimulateLayout);

        //SimulationLog
    simulationLog = new QTextEdit(this);
    simulationLog->setReadOnly(true);

        //ProgressBar
    labelProgressBar = new QLabel("Fortschrittsbalken",this);
    progressBar =  new QProgressBar(this);

        //Layout
    layout = new QGridLayout(this);

    layout->addWidget(topLabel,0,0,1,2);
    layout->addWidget(boxIntMethod,1,0,3,1);
    layout->addWidget(boxSolver,1,1,3,1);
    layout->addWidget(labelN,2,2);
    layout->addWidget(inputN,2,3);
    layout->addWidget(boxSimulate,1,4,3,1);
    layout->addWidget(simulationLog,4,0,1,6);
    layout->addWidget(labelProgressBar,5,0);
    layout->addWidget(progressBar,5,1,1,5);
}

void presentation::SimulationWidget::setController(Controller *controller)
{
    this->controller = controller;

    //TODO maxError und MAxIt & save load
    connect(inputIntMethod,SIGNAL(activated(QString)),controller,SLOT(selectIntMethodSlot(QString)));
    connect(inputM,SIGNAL(valueChanged(int)),controller,SLOT(newMSlot(int)));
    connect(inputT,SIGNAL(valueChanged(double)),controller,SLOT(newTSlot(double)));

    connect(inputSolver,SIGNAL(activated(QString)),controller,SLOT(selectSolverSlot(QString)));
    connect(inputMaxError,SIGNAL(valueChanged(double)),controller,SLOT(newMaxErrorSlot(double)));
    connect(inputMaxIt,SIGNAL(valueChanged(int)),controller,SLOT(newMaxItSlot(int)));

    connect(inputN,SIGNAL(valueChanged(int)),controller,SLOT(newNSlot(int)));

    connect(saveButton,SIGNAL(clicked(bool)),controller,SLOT(saveSetupSlot()));
    connect(loadButton,SIGNAL(clicked(bool)),controller,SLOT(loadSetupSlot()));
    connect(simulateButton,SIGNAL(clicked(bool)),controller,SLOT(simulateSlot()));
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
    inputMaxError->setValue(model->getSimulationSetup()->getSolverMaxError());
    inputMaxIt->setValue(model->getSimulationSetup()->getSolverMaxIt());

    inputN->setValue(model->getSimulationSetup()->getN());

    if(model->getSimulating())
    {
        topLabel->setText("Es wird zur Zeit simuliert.");
        simulateButton->setEnabled(false);
    }
    else
    {
       topLabel->setText("Dies ist der Tab zur Einstellung der Simulation.\n"
                         "Für weitere Informationen wechseln Sie in den Hilfe-Tab.");
       simulateButton->setEnabled(true);
       if(model->getSimulated())
           labelProgressBar->setText("Simulation beendet");

    }
}

void presentation::SimulationWidget::appendToSimulationLog(QString text)
{
    simulationLog->append(text);
}

void presentation::SimulationWidget::nextSimulationStage(QString stage, int maximum)
{
    progressBar->setMaximum(maximum);
    progressBar->setMinimum(0);
    progressBar->setValue(0);
    labelProgressBar->setText(stage);
}

void presentation::SimulationWidget::updateSimulationProgress(int step)
{
    progressBar->setValue(step);
}
