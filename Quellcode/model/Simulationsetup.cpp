#include "Simulationsetup.h"

model::SimulationSetup::SimulationSetup() : boundaries(4,300),
    heatSourcesBackgroundValue(0), heatSourcesCount(0), initialValue(300),
    m(1), n(3), selectedIntMethod("Impliziter Euler"),  selectedSolver("Jacobi"),
    solverMaxError(1e-10),solverMaxIt(1000), T(1.0),
    thermalConductivitiesBackgroundValue(0.01), thermalConductivitiesCount(0)
{

}

model::SimulationSetup::SimulationSetup(const SimulationSetup &rhs) :
    boundaries(rhs.boundaries), heatSourcesBackgroundValue(rhs.heatSourcesBackgroundValue),
    heatSourcesCount(rhs.heatSourcesCount), initialValue(rhs.initialValue),
    m(rhs.m), n(rhs.n), selectedIntMethod(rhs.selectedIntMethod),
    selectedSolver(rhs.selectedSolver), solverMaxError(rhs.solverMaxError),
    solverMaxIt(rhs.solverMaxIt), T(rhs.T),
    thermalConductivitiesBackgroundValue(rhs.thermalConductivitiesBackgroundValue),
    thermalConductivitiesCount(rhs.thermalConductivitiesCount)
{
    QList<Area* >::const_iterator it = rhs.heatSources.begin();
    for(; it != rhs.heatSources.end(); ++it)
        heatSources.append(new Area(*(*it)));
    it = rhs.thermalConductivities.end();
    for(; it != rhs.thermalConductivities.end(); ++it)
        thermalConductivities.append(new Area(*(*it)));
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
        thermalConductivities.append(new Area(xKoords,yKoords,value,type));
        ++thermalConductivitiesCount;
    }
}

// Vorbedingung: ID ist die gültige ID eines Gebietes im Modell
model::Area * const & model::SimulationSetup::getArea(const int id,
                                                      SimulationSetup::AreaType type) const
{
    QList<Area*> const & currentList = type == SimulationSetup::AreaHeatSource ?
                heatSources : thermalConductivities;
    QList<Area*>::const_iterator it = currentList.begin();
    for(; it != currentList.end(); ++it)
        if((*it)->getID() == id)
            return (*it);
    return NULL;
}

double model::SimulationSetup::getAreaBackgroundValue(SimulationSetup::AreaType type) const
{
  return type == SimulationSetup::AreaHeatSource ? heatSourcesBackgroundValue
                                       : thermalConductivitiesBackgroundValue;
}

QList<model::Area*> const & model::SimulationSetup::getAreas(SimulationSetup::AreaType type) const
{
    return type == SimulationSetup::AreaHeatSource ? heatSources: thermalConductivities;
}

int model::SimulationSetup::getAreaCount(SimulationSetup::AreaType type) const
{
    return type == SimulationSetup::AreaHeatSource ?
                heatSourcesCount : thermalConductivitiesCount;
}


double model::SimulationSetup::getBoundaryBottom() const
{
    return boundaries[0];
}

double model::SimulationSetup::getBoundaryLeft() const
{
    return boundaries[1];
}

double model::SimulationSetup::getBoundaryRight() const
{
    return boundaries[2];
}

double model::SimulationSetup::getBoundaryTop() const
{
    return boundaries[3];
}

double model::SimulationSetup::getInitialValue() const
{
    return initialValue;
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
        delete thermalConductivities.takeLast();
        --thermalConductivitiesCount;
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
        thermalConductivitiesBackgroundValue = newValue;
    }
}

void model::SimulationSetup::setBoundaryBottom(double const newBottomBoundary)
{
    boundaries[0] = newBottomBoundary;
}

void model::SimulationSetup::setBoundaryRight(double const newRightBoundary)
{
    boundaries[1] = newRightBoundary;
}

void model::SimulationSetup::setBoundaryLeft(double const newLeftBoundary)
{
    boundaries[2] = newLeftBoundary;
}

void model::SimulationSetup::setBoundaryTop(double const newTopBoundary)
{
    boundaries[3] = newTopBoundary;
}

void model::SimulationSetup::setInitialValue(double const newInitialValue)
{
    initialValue = newInitialValue;
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
