#include "Model.h"
#include "../presentation/Ui.h"

model::Model::Model() : QWidget(NULL), blocking(false), dataRead(false), optimized(false),
    overrideDiffusivities(false), overrideInitialTDvalue(model::SimulationSetup::AreaMinValue[
                                                         model::SimulationSetup::AreaThermalDiffusivity]),
    simSetup(new SimulationSetup()),
    simulated(false), simWorker(new SimulationWorker(NULL)), ui(NULL), useHeatSources(false),
    workerThread(this), working(false)
{
    simWorker->moveToThread(&workerThread);
    simWorker->initializeMaps();

    connect(&workerThread, &QThread::finished, simWorker, &QObject::deleteLater);
    connect(this,SIGNAL(startOptimization(SimulationSetup*,bool,double,bool)),
            simWorker,SLOT(startOptimizationSlot(SimulationSetup*,bool,double,bool)));
    connect(this,SIGNAL(startReadingData(QString,long)),
            simWorker,SLOT(startReadingDataSlot(QString,long)));
    connect(this,SIGNAL(startSimulation(SimulationSetup*)),
            simWorker,SLOT(startSimulationSlot(SimulationSetup*)));
    connect(simWorker,SIGNAL(startedWork()),this,SLOT(startedWorkSlot()));
    connect(simWorker,SIGNAL(finishedOptimization(bool)),this,SLOT(finishedOptimizationSlot(bool)));
    connect(simWorker,SIGNAL(finishedReadingData()),this,SLOT(finishedReadingDataSlot()));
    connect(simWorker,SIGNAL(finishedSimulation(bool)),this,SLOT(finishedSimulationSlot(bool)));

    workerThread.start();
}

model::Model::~Model()
{
    delete simSetup;
    simWorker->abortWork();
    workerThread.quit();
    workerThread.wait();
}

void model::Model::abortWork()
{
    assert(!blocking);
    if(working)
        simWorker->abortWork();
}

void model::Model::addNewArea(const QVector<double> &xKoords,
                              const QVector<double> &yKoords, double value,
                              SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->addNewArea(xKoords,yKoords,value,type);
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

double** model::Model::getObservations() const
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

bool model::Model::getOptimized() const
{
    return optimized;
}

QVector<double> model::Model::getOptimizedCoeffs() const
{
    if(!working && optimized)
        return simWorker->getOptimizedCoeffs();
    return QVector<double>();
}

bool model::Model::getOverrideThermalDiffusivities() const
{
    return overrideDiffusivities;
}

double model::Model::getOverrideValue() const
{
    return overrideInitialTDvalue;
}

double ***model::Model::getResult() const
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

void model::Model::loadSetup(QString filename)
{
    assert(!blocking);
    delete simSetup;
    simSetup = new SimulationSetup();
    model::Area::resetIDs();
    overrideDiffusivities = false;
    overrideInitialTDvalue = SimulationSetup::AreaMinValue[
            SimulationSetup::AreaThermalDiffusivity];
    useHeatSources = false;

    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Truncate))
    {
        QTextStream in(&file);
        //TODO: korrektes überspringen der Ueberschrift
        in.readLine();

        in >> *simSetup;

        in.readLine();
        in.readLine();
        QString str;
        for(int i = 0; i < 4; ++i)
            in >> str;
        if(str == "Aktiviert")
        {
            overrideDiffusivities = true;
            in >> str;
            in >> str;
            in >> overrideInitialTDvalue;
            in >> str;
        }
        else
            in >> str;
        for(int i = 0; i < 4; ++i)
            in >> str;
        if(str == "Aktiviert")
            useHeatSources = true;
        else
            in >> str;
    }

    ui->updateNotification();
}


void model::Model::optimize()
{
    if(!dataRead)
        return;
    blocking = true;
    optimized = false;
    emit startOptimization(simSetup,overrideDiffusivities,overrideInitialTDvalue,useHeatSources);
    working = true;
    ui->updateNotification();
}

void model::Model::readObservations(QString const filename, long const obsCount)
{
    blocking = true;
    dataRead = false;
    emit startReadingData(filename,obsCount);
    working = true;
    ui->updateNotification();
}

void model::Model::removeLastArea(SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->removeLastArea(type);
    ui->updateNotification();
}

void model::Model::reorderArea(int const pos, int const dir,
                               model::SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->reorderAreas(pos,dir,type);
    ui->updateNotification();
}

void model::Model::resetSetup()
{
    assert(!blocking);
    delete simSetup;
    simSetup = new SimulationSetup();
    model::Area::resetIDs();
    overrideDiffusivities = false;
    overrideInitialTDvalue = SimulationSetup::AreaMinValue[
            SimulationSetup::AreaThermalDiffusivity];
    useHeatSources = false;
    ui->updateNotification();
}

void model::Model::saveSetup(QString filename)
{
    assert(!blocking);
    QFile file(filename);
    if(file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&file);
        out << "TODO: Grosse tolle Ueberschrift\n"
            << *simSetup
            << "Optimierungseinstellungen:\n\n"
            << "Ueberschreiben der Temperaturleitkoeffizienten: "
            << (overrideDiffusivities ? "Aktiviert" : "Nicht aktiviert")
            << "\n";
        if(overrideDiffusivities)
        {
            out.setRealNumberNotation(QTextStream::ScientificNotation);
            out.setRealNumberPrecision(2);
            out << "Manueller Anfangswert: "
                << overrideInitialTDvalue << "m^2/s" << "\n";
        }
        out << "Nutzen der Waermequellen: "
            << (useHeatSources ? "Aktiviert" : "Nicht aktiviert")
            << "\n";
    }
}

// Updatet die gewählte Integrationsmethode
void model::Model::selectIntMethod(QString newIntMethod)
{
    assert(!blocking);
    simSetup->selectIntMethod(newIntMethod);
    ui->updateNotification();
}

// Updatet den gewählten iterativen Löser
void model::Model::selectSolver(QString newSolver)
{
    assert(!blocking);
    simSetup->selectSolver(newSolver);
    ui->updateNotification();
}

void model::Model::setAreaBackgroundValue(const double newValue, SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->setAreaBackgroundValue(newValue,type);
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

void model::Model::setOverrideValue(double const newValue)
{
    assert(!blocking);
    overrideInitialTDvalue = newValue;
    ui->updateNotification();
}

void model::Model::setSolverMaxError(double const newMaxError)
{
    assert(!blocking);
    simSetup->setSolverMaxError(newMaxError);
    ui->updateNotification();
}

void model::Model::setSolverMaxIt(int const newMaxIt)
{
    assert(!blocking);
    simSetup->setSolverMaxIt(newMaxIt);
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
    connect(simWorker,SIGNAL(beginningOptimizationStage(QString,int)),ui,SLOT(nextOptimizationStageSlot(QString,int)));
    connect(simWorker,SIGNAL(finishedOptimizationStep(int)),ui,SLOT(updateOptimizationProgressSlot(int)));
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
    simulated = false;
    emit startSimulation(simSetup);
    working = true;
    ui->updateNotification();
}

void model::Model::updateAreaValue(int const pos, double const newValue,
                              SimulationSetup::AreaType type)
{
    assert(!blocking);
    simSetup->updateAreaValue(pos,newValue,type);
    ui->updateNotification();
}

void model::Model::startedWorkSlot()
{
    blocking = false;
    working = true;
}

void model::Model::finishedOptimizationSlot(bool success)
{
    optimized = success;
    working = false;
    ui->updateNotification();
}

void model::Model::finishedReadingDataSlot()
{
    dataRead = true;
    working = false;
    ui->updateNotification();
}

void model::Model::finishedSimulationSlot(bool success)
{
    simulated = success;
    working = false;
    ui->updateNotification();
}
