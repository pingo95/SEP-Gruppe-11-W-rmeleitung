#include "Model.h"
#include "../presentation/Ui.h"

model::Model::Model() : QWidget(NULL), boundaryBottom(300), boundaryLeft(300),
    boundaryRight(300), boundaryTop(300), heatSourcesBackgroundValue(0),
    heatSourcesCount(0), initialValue(300), m(1), n(3), selectedIntMethod("Impliziter Euler"),
    selectedSolver("Jacobi"), simulated(false), T(1.),
    thermalConductivitiesBackgroundValue(0.01), thermalConductivitiesCount(0), ui(NULL),
    simulating(false), blocking(false), simWorker(new SimulationWorker()),
    workerThread(this), solverMaxError(1e-10), solverMaxIt(1000)
{
    simWorker->moveToThread(&workerThread);
    simWorker->initializeMaps();
    connect(&workerThread, &QThread::finished, simWorker, &QObject::deleteLater);
    connect(this,SIGNAL(startSimulation(double,double,double,double,QList<model::Area*>,double,int,double,QString,QString,long,long,double,int,double,QList<model::Area*>,double,int)),
            simWorker,SLOT(startSimulationSlot(double,double,double,double,QList<model::Area*>,double,int,double,QString,QString,long,long,double,int,double,QList<model::Area*>,double,int)));
    connect(simWorker,SIGNAL(startedSimulation()),this,SLOT(simulationStartedSlot()));
    connect(simWorker,SIGNAL(finishedSimulation()),this,SLOT(simulationFinishedSlot()));
    workerThread.start();
}

model::Model::~Model()
{
    QList<Area*>::iterator it = heatSources.begin();
    for(; it != heatSources.end(); ++it)
        delete (*it);
    it = thermalConductivities.begin();
    for(; it != thermalConductivities.end(); ++it)
        delete (*it);

    workerThread.quit();
    workerThread.wait();
}

// Vorbedingung: Das übergebene Gebiet wurde vorher mit Area::validateArea auf
// Gültigkeit überprüft
// Diese Funktion fügt dem Modell ein neues Wärmequellen oder Wärmeleitkoeffizienten
// -Gebiet hinzu
void model::Model::addNewArea(const QVector<double> &xKoords,
                              const QVector<double> &yKoords, double value,
                              Model::AreaType type)
{
    assert(!blocking);
    if(type == Model::AreaHeatSource)
    {
        heatSources.append(new Area(xKoords,yKoords,value,type));
        ++heatSourcesCount;
    }
    else
    {
        thermalConductivities.append(new Area(xKoords,yKoords,value,type));
        ++thermalConductivitiesCount;
    }
    ui->updateNotification();
}

// Vorbedingung: ID ist die gültige ID eines Gebietes im Modell
model::Area * const & model::Model::getArea(const int id, Model::AreaType type) const
{
    assert(!blocking);
    QList<Area*> const & currentList = type == Model::AreaHeatSource ?
                heatSources : thermalConductivities;
    QList<Area*>::const_iterator it = currentList.begin();
    for(; it != currentList.end(); ++it)
        if((*it)->getID() == id)
            return (*it);
    return NULL;
}

double model::Model::getAreaBackgroundValue(Model::AreaType type) const
{
  return type == Model::AreaHeatSource ? heatSourcesBackgroundValue
                                       : thermalConductivitiesBackgroundValue;
}

QList<model::Area*> const & model::Model::getAreas(Model::AreaType type) const
{
    assert(!blocking);
    return type == Model::AreaHeatSource ? heatSources: thermalConductivities;
}

int model::Model::getAreaCount(Model::AreaType type) const
{
    return type == Model::AreaHeatSource ?
                heatSourcesCount : thermalConductivitiesCount;
}


double model::Model::getBoundaryBottom() const
{
    return boundaryBottom;
}

double model::Model::getBoundaryLeft() const
{
    return boundaryLeft;
}

double model::Model::getBoundaryRight() const
{
    return boundaryRight;
}

double model::Model::getBoundaryTop() const
{
    return boundaryTop;
}

double model::Model::getInitialValue() const
{
    return initialValue;
}

QList<QString> const model::Model::getIntMethodNames() const
{
    return simWorker->getIntMethodNames();
}

QList<QString> const model::Model::getIterativeSolverNames() const
{
    return simWorker->getIterativeSolverNames();
}

long model::Model::getM() const
{
    return m;
}

long model::Model::getN() const
{
    return n;
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

QString model::Model::getSelectedIntMethod() const
{
    return selectedIntMethod;
}

QString model::Model::getSelectedIterativeSolver() const
{
    return selectedSolver;
}

bool model::Model::getSimulated() const
{
    return simulated;
}

bool model::Model::getSimulating() const
{
    return simulating;
}

double model::Model::getSolverMaxError() const
{
    return solverMaxError;
}

int model::Model::getSolverMaxIt() const
{
    return solverMaxIt;
}

double model::Model::getT() const
{
    return T;
}


void model::Model::removeLastArea(Model::AreaType type)
{
    assert(!blocking);
    if(type == Model::AreaHeatSource)
    {
        delete heatSources.takeLast();
        --heatSourcesCount;
    }
    else
    {
        delete thermalConductivities.takeLast();
        --thermalConductivitiesCount;
    }
    ui->updateNotification();
}


// Updatet die gewählte Integrationsmethode
void model::Model::selectIntMethod(QString intMethod)
{
    selectedIntMethod = intMethod;
    ui->updateNotification();
}

// Updatet den gewählten iterativen Löser
void model::Model::selectIterativeSolver(QString newIterativeSolver)
{
    selectedSolver = newIterativeSolver;
    ui->updateNotification();
}

void model::Model::setAreaBackground(const double newValue, Model::AreaType type)
{
    if(type == Model::AreaHeatSource)
    {
        heatSourcesBackgroundValue = newValue;
    }
    else
    {
        thermalConductivitiesBackgroundValue = newValue;
    }
    ui->updateNotification();
}

void model::Model::setBoundaryBottom(double const newBottomBoundary)
{
    boundaryBottom = newBottomBoundary;
    ui->updateNotification();
}

void model::Model::setBoundaryRight(double const newRightBoundary)
{
    boundaryRight = newRightBoundary;
    ui->updateNotification();
}

void model::Model::setBoundaryLeft(double const newLeftBoundary)
{
    boundaryLeft = newLeftBoundary;
    ui->updateNotification();
}

void model::Model::setBoundaryTop(double const newTopBoundary)
{
    boundaryTop = newTopBoundary;
    ui->updateNotification();
}

void model::Model::setInitialValue(double const newInitialValue)
{
    initialValue = newInitialValue;
    ui->updateNotification();
}

void model::Model::setM(int const newM)
{
    m = newM;
    ui->updateNotification();
}

void model::Model::setN(int const newN)
{
    n = newN;
    ui->updateNotification();
}

void model::Model::setSolverMaxError(double const maxError)
{
    solverMaxError = maxError;
}

void model::Model::setSolverMaxIt(double const maxIt)
{
    solverMaxIt = maxIt;
}

void model::Model::setT(double const newT)
{
    T = newT;
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
    emit startSimulation(boundaryBottom,boundaryLeft,boundaryRight,boundaryTop,
                         heatSources,heatSourcesBackgroundValue,heatSourcesCount,initialValue,
                         selectedIntMethod,selectedSolver,m,n,solverMaxError,solverMaxIt,
                         T,thermalConductivities,thermalConductivitiesBackgroundValue,
                         thermalConductivitiesCount);
}

void model::Model::updateAreaValue(int const pos, double const value,
                              Model::AreaType type)
{
    QList<Area*> const & currentList = type == Model::AreaHeatSource ?
                heatSources : thermalConductivities;
    currentList.at(pos)->setValue(value);
    ui->updateNotification();
}

void model::Model::simulationStartedSlot()
{
    blocking = false;
    simulating = true;

    ui->updateNotification();
}

void model::Model::simulationFinishedSlot()
{
    simulated = true;
    simulating = false;

    ui->updateNotification();
}
