#include "Model.h"
#include "../presentation/Ui.h"

model::Model::Model() : QWidget(NULL), blocking(false), dataRead(false), optimized(false),
    overrideDiffusivities(false), overrideInitialTDvalue(model::SimulationSetup::AreaMinValue[
                                                         model::SimulationSetup::AreaThermalDiffusivity]),
    simSetup(new SimulationSetup()),
    simulated(false), simWorker(new SimulationWorker()), ui(NULL), useHeatSources(false),
    workerThread(this), working(false)
{
    simWorker->moveToThread(&workerThread);
    simWorker->initializeMaps();

    connect(&workerThread, &QThread::finished, simWorker, &QObject::deleteLater);
    connect(this,SIGNAL(startOptimization(SimulationSetup*,bool,double,bool)),
            simWorker,SLOT(startOptimization(SimulationSetup*,bool,double,bool)));
    connect(this,SIGNAL(startReadingData(QString,long)),
            simWorker,SLOT(startReadingData(QString,long)));
    connect(this,SIGNAL(startSimulation(SimulationSetup*)),
            simWorker,SLOT(startSimulationSlot(SimulationSetup*)));
    connect(simWorker,SIGNAL(startedWork()),this,SLOT(startedWorkSlot()));
    connect(simWorker,SIGNAL(finishedOptimization()),this,SLOT(finishedOptimizationSlot()));
    connect(simWorker,SIGNAL(finishedReadingData()),this,SLOT(finishedReadingDataSlot()));
    connect(simWorker,SIGNAL(finishedSimulation()),this,SLOT(finishedSimulationSlot()));

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

void model::Model::addNewArea(Area *area, SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->addNewArea(area,type);
    ui->updateNotification();
}

void model::Model::deleteArea(const int pos, SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->deleteArea(pos,type);
    ui->updateNotification();
}

bool model::Model::getDataRead() const
{
    return dataRead;
}

QList<QString> const model::Model::getIntMethodsNames() const
{
    return simWorker->getIntMethodNames();
}

double** const & model::Model::getObservations() const
{
    if(!working && dataRead)
        return simWorker->getObservations();
    return NULL;
}

int model::Model::getObservationsDim() const
{
   if(!working && dataRead)
       return simWorker->getObservationsDim();
   return 1;
}

bool model::Model::getOverrideThermalDiffusivities() const
{
    return overrideDiffusivities;
}

double model::Model::getOverrideValue() const
{
    return overrideInitialTDvalue;
}

double*** const & model::Model::getResult() const
{
    if(!working && simulated)
        return simWorker->getResult();
    return NULL;
}

long model::Model::getResultM() const
{
    if(!working && simulated)
        return simWorker->getM();
    return 1;
}

long model::Model::getResultN() const
{
    if(!working && simulated)
        return simWorker->getN();
    return 3;
}

double model::Model::getResultT() const
{
    if(!working && simulated)
        return simWorker->getT();
    return 60.0;
}

bool model::Model::getSimulated() const
{
    return simulated;
}

model::SimulationSetup * const & model::Model::getSimulationSetup() const
{
    return simSetup;
}

QList<QString> const model::Model::getSolverNames() const
{
    return simWorker->getSolverNames();
}

bool model::Model::getUseHeatSources() const
{
    return useHeatSources;
}

bool model::Model::isWorking() const
{
    return working;
}

void model::Model::optimize()
{
    blocking = true;
    emit startOptimization(simSetup,overrideDiffusivities,overrideInitialTDvalue,useHeatSources);
    working = true;
    ui->updateNotification();
}

void model::Model::readObservations(QString const filename, long const obsCount)
{
    blocking = true;
    emit startReadingData(filename,obsCount);
    working = true;
    ui->updateNotification();
}

void model::Model::reorderArea(int const pos, int const dir,
                               model::SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->reorderArea(pos,dir,type);
    ui->updateNotification();
}

void model::Model::resetSetup()
{
    assert(!blocking);
    delete simSetup;
    simSetup = new SimulationSetup;
    overrideDiffusivities = false;
    overrideInitialTDvalue = SimulationSetup::AreaMinValue[
            SimulationSetup::AreaThermalDiffusivity];
    useHeatSources = false;
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

void model::Model::setIBV(const double newValue, SimulationSetup::IBV ibv)
{
    assert(!blocking);
    simSetup->setIBV(newValue,ibv);
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

void model::Model::setOverrideThermalDiffusivities(bool const override)
{
    assert(!blocking);
    overrideDiffusivities = override;
    ui->updateNotification();
}

void model::Model::setOverrideValue(double const value)
{
    assert(!blocking);
    overrideInitialTDvalue = value;
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

    connect(simWorker,SIGNAL(beginningStage(QString,int,bool)),ui,SLOT(nextStageSlot(QString,int,bool)));
    connect(simWorker,SIGNAL(finishedStep(int,bool)),ui,SLOT(updateProgressSlot(int,bool)));
    connect(simWorker,SIGNAL(simulationLogUpdate(QString)),ui,SLOT(appendToSimulationLogSlot(QString)));
}

void model::Model::setUseHeatSources(bool const useHeatSources)
{
    assert(!blocking);
    this->useHeatSources = useHeatSources;
    ui->updateNotification();
}

void model::Model::simulate()
{
    blocking = true;
    emit startSimulation(simSetup);
    working = true;
    ui->updateNotification();
}

model::Area * model::Model::takeLastArea(SimulationSetup::AreaType type)
{
    assert(!blocking);
    Area * ptr = simSetup->takeLastArea(type);
    ui->updateNotification();
    return ptr;
}

void model::Model::updateAreaValue(int const pos, double const value,
                              SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->updateAreaValue(pos,value,type);
    ui->updateNotification();
}

void model::Model::startedWorkSlot()
{
    blocking = false;
    working = true;
}

void model::Model::finishedOptimizationSlot()
{
    optimized = true;
    working = false;
    ui->updateNotification();
}

void model::Model::finishedReadingDataSlot()
{
    dataRead = true;
    working = false;
    ui->updateNotification();
}

void model::Model::finishedSimulationSlot()
{
    simulated = true;
    working = false;
    ui->updateNotification();
}
