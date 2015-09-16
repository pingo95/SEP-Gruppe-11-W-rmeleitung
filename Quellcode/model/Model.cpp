#include "Model.h"
#include "../presentation/Ui.h"

model::Model::Model() : QWidget(NULL), blocking(false), simSetup(new SimulationSetup()), simulated(false),
    simulating(false),  simWorker(new SimulationWorker()), ui(NULL), workerThread(this)
{
    simWorker->moveToThread(&workerThread);
    simWorker->initializeMaps();
    connect(&workerThread, &QThread::finished, simWorker, &QObject::deleteLater);
    connect(this,SIGNAL(startSimulation(SimulationSetup*)),simWorker,SLOT(startSimulationSlot(SimulationSetup*)));
    connect(simWorker,SIGNAL(startedSimulation()),this,SLOT(simulationStartedSlot()));
    connect(simWorker,SIGNAL(finishedSimulation()),this,SLOT(simulationFinishedSlot()));
    workerThread.start();
}

model::Model::~Model()
{
    delete simSetup;
    workerThread.quit();
    workerThread.wait();
}

// Vorbedingung: Das übergebene Gebiet wurde vorher mit Area::validateArea auf
// Gültigkeit überprüft
// Diese Funktion fügt dem Modell ein neues Wärmequellen oder Wärmeleitkoeffizienten
// -Gebiet hinzu
void model::Model::addNewArea(const QVector<double> &xKoords,
                              const QVector<double> &yKoords, double value,
                              SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->addNewArea(xKoords,yKoords,value,type);
    ui->updateNotification();
}

double*** const & model::Model::getResult() const
{
    return simWorker->getResult();
}

long model::Model::getResultM() const
{
    return simWorker->getM();
}

long model::Model::getResultN() const
{
    return simWorker->getN();
}

double model::Model::getResultT() const
{
    return simWorker->getT();
}

QList<QString> const model::Model::getIntMethodsNames() const
{
    return simWorker->getIntMethodNames();
}

QList<QString> const model::Model::getSolverNames() const
{
    return simWorker->getSolverNames();
}

bool model::Model::getSimulated() const
{
    return simulated;
}

bool model::Model::getSimulating() const
{
    return simulating;
}

model::SimulationSetup * const & model::Model::getSimulationSetup() const
{
    return simSetup;
}

void model::Model::removeLastArea(SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->removeLastArea(type);
    ui->updateNotification();
}


// Updatet die gewählte Integrationsmethode
void model::Model::selectIntMethod(QString intMethod)
{
    assert(!blocking);
    simSetup->selectIntMethod(intMethod);
    ui->updateNotification();
}

// Updatet den gewählten iterativen Löser
void model::Model::selectSolver(QString newSolver)
{
    assert(!blocking);
    simSetup->selectSolver(newSolver);
    ui->updateNotification();
}

void model::Model::setAreaBackground(const double newValue, SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->setAreaBackground(newValue,type);
    ui->updateNotification();
}

void model::Model::setBoundaryBottom(double const newBottomBoundary)
{
    assert(!blocking);
    simSetup->setBoundaryBottom(newBottomBoundary);
    ui->updateNotification();
}

void model::Model::setBoundaryLeft(double const newLeftBoundary)
{
    assert(!blocking);
    simSetup->setBoundaryLeft(newLeftBoundary);
    ui->updateNotification();
}

void model::Model::setBoundaryRight(double const newRightBoundary)
{
    assert(!blocking);
    simSetup->setBoundaryRight(newRightBoundary);
    ui->updateNotification();
}

void model::Model::setBoundaryTop(double const newTopBoundary)
{
    assert(!blocking);
    simSetup->setBoundaryTop(newTopBoundary);
    ui->updateNotification();
}

void model::Model::setInitialValue(double const newInitialValue)
{
    assert(!blocking);
    simSetup->setInitialValue(newInitialValue);
    ui->updateNotification();
}

void model::Model::setM(int const newM)
{
    assert(!blocking);
    simSetup->setM(newM);
    ui->updateNotification();
}

void model::Model::setN(int const newN)
{
    assert(!blocking);
    simSetup->setN(newN);
    ui->updateNotification();
}

void model::Model::setSolverMaxError(double const maxError)
{
    assert(!blocking);
    simSetup->setSolverMaxError(maxError);
    ui->updateNotification();
}

void model::Model::setSolverMaxIt(double const maxIt)
{
    assert(!blocking);
    simSetup->setSolverMaxIt(maxIt);
    ui->updateNotification();
}

void model::Model::setT(double const newT)
{
    assert(!blocking);
    simSetup->setT(newT);
    ui->updateNotification();
}

void model::Model::setUI(presentation::UI *ui)
{
    this->ui = ui;

    connect(simWorker,SIGNAL(beginningSimulationStage(QString,int)),ui,SLOT(nextProgresseStageSlot(QString,int)));
    connect(simWorker,SIGNAL(finishedStep(int)),ui,SLOT(updateSimulationProgressSlot(int)));
    connect(simWorker,SIGNAL(simulationLogUpdate(QString)),ui,SLOT(appendToSimulationLogSlot(QString)));
}


void model::Model::simulate()
{
    blocking = true;
    emit startSimulation(simSetup);
    simulating = true;
    ui->updateNotification();
}

void model::Model::updateAreaValue(int const pos, double const value,
                              SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->updateAreaValue(pos,value,type);
    ui->updateNotification();
}

void model::Model::simulationStartedSlot()
{
    blocking = false;
}

void model::Model::simulationFinishedSlot()
{
    simulated = true;
    simulating = false;
    ui->updateNotification();
}
