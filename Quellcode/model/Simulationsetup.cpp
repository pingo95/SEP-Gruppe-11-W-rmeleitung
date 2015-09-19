#include "Simulationsetup.h"

double const model::SimulationSetup::AreaMaxValue[2] = {1000,200e-6};
double const model::SimulationSetup::AreaMinValue[2] = {0,1e-6};

double const model::SimulationSetup::MaxTemperature = 1000;
double const model::SimulationSetup::MinTemperature = 0;

model::SimulationSetup::SimulationSetup() : heatSourcesBackgroundValue(0),
    heatSourcesCount(0), m(1), n(3),
    selectedIntMethod("Impliziter Euler"),  selectedSolver("Jacobi"),
    solverMaxError(1e-10),solverMaxIt(1000), T(1.0),
    thermalDiffusivitiesBackgroundValue(1e-4), thermalDiffusiivitiesCount(0)
{
    iBVs[SimulationSetup::InitialValue] = 300;
    iBVs[SimulationSetup::BottomBoundary] = 300;
    iBVs[SimulationSetup::LeftBoundary] = 300;
    iBVs[SimulationSetup::RightBoundary] = 300;
    iBVs[SimulationSetup::TopBoundary] = 300;
}

model::SimulationSetup::SimulationSetup(const SimulationSetup &rhs) :
    heatSourcesBackgroundValue(rhs.heatSourcesBackgroundValue),
    heatSourcesCount(rhs.heatSourcesCount),
    m(rhs.m), n(rhs.n), selectedIntMethod(rhs.selectedIntMethod),
    selectedSolver(rhs.selectedSolver), solverMaxError(rhs.solverMaxError),
    solverMaxIt(rhs.solverMaxIt), T(rhs.T),
    thermalDiffusivitiesBackgroundValue(rhs.thermalDiffusivitiesBackgroundValue),
    thermalDiffusiivitiesCount(rhs.thermalDiffusiivitiesCount)
{
    iBVs[SimulationSetup::InitialValue] = rhs.iBVs[SimulationSetup::InitialValue];
    iBVs[SimulationSetup::BottomBoundary] = rhs.iBVs[SimulationSetup::BottomBoundary];
    iBVs[SimulationSetup::LeftBoundary] = rhs.iBVs[SimulationSetup::LeftBoundary];
    iBVs[SimulationSetup::RightBoundary] = rhs.iBVs[SimulationSetup::RightBoundary];
    iBVs[SimulationSetup::TopBoundary] = rhs.iBVs[SimulationSetup::TopBoundary];
    QList<Area* >::const_iterator it = rhs.heatSources.begin();
    for(; it != rhs.heatSources.end(); ++it)
        heatSources.append(new Area(*(*it)));
    it = rhs.thermalDiffusivities.begin();
    for(; it != rhs.thermalDiffusivities.end(); ++it)
        thermalDiffusivities.append(new Area(*(*it)));
}

model::SimulationSetup::~SimulationSetup()
{
    QList<Area*>::iterator it = heatSources.begin();
    for(; it != heatSources.end(); ++it)
        delete (*it);
    it = thermalDiffusivities.begin();
    for(; it != thermalDiffusivities.end(); ++it)
        delete (*it);
}

void model::SimulationSetup::addNewArea(const QVector<double> &xKoords,
                              const QVector<double> &yKoords, double value,
                              SimulationSetup::AreaType type)
{
    if(type == SimulationSetup::AreaHeatSource)
    {
        heatSources.append(new Area(xKoords,yKoords,value,type));
        ++heatSourcesCount;
    }
    else
    {
        thermalDiffusivities.append(new Area(xKoords,yKoords,value,type));
        ++thermalDiffusiivitiesCount;
    }
}

// Vorbedingung: ID ist die gültige ID eines Gebietes im Modell
model::Area * const & model::SimulationSetup::getArea(const int id,
                                                      SimulationSetup::AreaType type) const
{
    QList<Area*> const & currentList = type == SimulationSetup::AreaHeatSource ?
                heatSources : thermalDiffusivities;
    QList<Area*>::const_iterator it = currentList.begin();
    for(; it != currentList.end(); ++it)
        if((*it)->getID() == id)
            return (*it);
    return NULL;
}

double model::SimulationSetup::getAreaBackgroundValue(SimulationSetup::AreaType type) const
{
  return type == SimulationSetup::AreaHeatSource ? heatSourcesBackgroundValue
                                       : thermalDiffusivitiesBackgroundValue;
}

QList<model::Area*> const & model::SimulationSetup::getAreas(SimulationSetup::AreaType type) const
{
    return type == SimulationSetup::AreaHeatSource ? heatSources: thermalDiffusivities;
}

int model::SimulationSetup::getAreaCount(SimulationSetup::AreaType type) const
{
    return type == SimulationSetup::AreaHeatSource ?
                heatSourcesCount : thermalDiffusiivitiesCount;
}

double model::SimulationSetup::getIBV(SimulationSetup::IBV ibv)
{
    return iBVs[ibv];
}

long model::SimulationSetup::getM() const
{
    return m;
}

long model::SimulationSetup::getN() const
{
    return n;
}

QString model::SimulationSetup::getSelectedIntMethod() const
{
    return selectedIntMethod;
}

QString model::SimulationSetup::getSelectedSolver() const
{
    return selectedSolver;
}

double model::SimulationSetup::getSolverMaxError() const
{
    return solverMaxError;
}

int model::SimulationSetup::getSolverMaxIt() const
{
    return solverMaxIt;
}

double model::SimulationSetup::getT() const
{
    return T;
}


void model::SimulationSetup::removeLastArea(SimulationSetup::AreaType type)
{
    if(type == SimulationSetup::AreaHeatSource)
    {
        delete heatSources.takeLast();
        --heatSourcesCount;
    }
    else
    {
        delete thermalDiffusivities.takeLast();
        --thermalDiffusiivitiesCount;
    }
}


// Updatet die gewählte Integrationsmethode
void model::SimulationSetup::selectIntMethod(QString intMethod)
{
    selectedIntMethod = intMethod;
}

// Updatet den gewählten iterativen Löser
void model::SimulationSetup::selectSolver(QString newSolver)
{
    selectedSolver = newSolver;
}

void model::SimulationSetup::setAreaBackground(const double newValue, SimulationSetup::AreaType type)
{
    if(type == SimulationSetup::AreaHeatSource)
    {
        heatSourcesBackgroundValue = newValue;
    }
    else
    {
        thermalDiffusivitiesBackgroundValue = newValue;
    }
}

void model::SimulationSetup::setIBV(double const newValue, SimulationSetup::IBV ibv)
{
    iBVs[ibv] = newValue;
}

void model::SimulationSetup::setM(int const newM)
{
    m = newM;
}

void model::SimulationSetup::setN(int const newN)
{
    n = newN;
}

void model::SimulationSetup::setSolverMaxError(double const maxError)
{
    solverMaxError = maxError;
}

void model::SimulationSetup::setSolverMaxIt(double const maxIt)
{
    solverMaxIt = maxIt;
}

void model::SimulationSetup::setT(double const newT)
{
    T = newT;
}

void model::SimulationSetup::updateAreaValue(const int pos, const double value, SimulationSetup::AreaType type)
{
    if(type == SimulationSetup::AreaHeatSource)
    {
        heatSources.at(pos)->setValue(value);
    }
    else
    {
        thermalDiffusivities.at(pos)->setValue(value);
    }
}
