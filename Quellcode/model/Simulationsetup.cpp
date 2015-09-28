#include "Simulationsetup.h"

double const model::SimulationSetup::AreaMaxValue[2] = {1000,200e-6};
double const model::SimulationSetup::AreaMinValue[2] = {0,1e-6};

double const model::SimulationSetup::MaxTemperature = 1000;
double const model::SimulationSetup::MinTemperature = 0;

model::SimulationSetup::SimulationSetup() : heatSourcesBackgroundValue(0),
    heatSourcesCount(0), m(1), n(3),
    selectedIntMethod("Impliziter Euler"),  selectedSolver("Jacobi"),
    solverMaxError(1e-10),solverMaxIt(1000), T(60.0),
    thermalDiffusivitiesBackgroundValue(1e-4), thermalDiffusivitiesCount(0)
{
    iBVs[SimulationSetup::BottomBoundary] = 300;
    iBVs[SimulationSetup::InitialValue] = 300;
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
    thermalDiffusivitiesCount(rhs.thermalDiffusivitiesCount)
{
    iBVs[SimulationSetup::BottomBoundary] = rhs.iBVs[SimulationSetup::BottomBoundary];
    iBVs[SimulationSetup::InitialValue] = rhs.iBVs[SimulationSetup::InitialValue];
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

void model::SimulationSetup::addNewArea(QVector<double> const & xKoords,
                                        QVector<double> const & yKoords,
                                        double value, SimulationSetup::AreaType type)
{
    if(type == SimulationSetup::AreaHeatSource)
    {
        heatSources.append(new Area(xKoords,yKoords,value,type));
        ++heatSourcesCount;
    }
    else
    {
        thermalDiffusivities.append(new Area(xKoords,yKoords,value,type));
        ++thermalDiffusivitiesCount;
    }
}

void model::SimulationSetup::deleteArea(const int pos, SimulationSetup::AreaType type)
{
    if(type == SimulationSetup::AreaHeatSource)
    {
        delete heatSources.takeAt(pos);
        --heatSourcesCount;
    }
    else
    {
        delete thermalDiffusivities.takeAt(pos);
        --thermalDiffusivitiesCount;
    }
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
                heatSourcesCount : thermalDiffusivitiesCount;
}

int model::SimulationSetup::getContainingAreaID(double const xKoord,
                                                double const yKoord,
                                                SimulationSetup::AreaType type)
{
    int & count = type == SimulationSetup::AreaHeatSource ?
                heatSourcesCount : thermalDiffusivitiesCount;
    int id = -1;
    if(count > 0)
    {
        QList<Area*> & areas = type == SimulationSetup::AreaHeatSource ?
                    heatSources : thermalDiffusivities;
        QList<Area*>::const_iterator it = areas.begin();
        for(; it != areas.end(); ++it)
            if((*it)->insidePoint(xKoord,yKoord))
                id = (*it)->getID();
    }
    return id;
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
        heatSources.removeLast();
        --heatSourcesCount;
    }
    else
    {
        thermalDiffusivities.removeLast();
        --thermalDiffusivitiesCount;
    }
}

void model::SimulationSetup::reorderAreas(int const pos, int const dir,
                                         SimulationSetup::AreaType type)
{
    QList<Area*> & areas = type == SimulationSetup::AreaHeatSource ?
                heatSources : thermalDiffusivities;
    if(dir == 2)
    {
        areas.push_front(areas.takeAt(pos));
        return;
    }
    if(dir == -2)
    {
        areas.push_back(areas.takeAt(pos));
    }
    else
        areas.swap(pos,pos-dir);
}

// Updatet die gewählte Integrationsmethode
void model::SimulationSetup::selectIntMethod(QString newIntMethod)
{
    selectedIntMethod = newIntMethod;
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

void model::SimulationSetup::setSolverMaxError(double const newMaxError)
{
    solverMaxError = newMaxError;
}

void model::SimulationSetup::setSolverMaxIt(int const newMaxIt)
{
    solverMaxIt = newMaxIt;
}

void model::SimulationSetup::setT(double const newT)
{
    T = newT;
}

void model::SimulationSetup::updateAreaValue(const int pos, const double newValue,
                                             SimulationSetup::AreaType type)
{
    if(type == SimulationSetup::AreaHeatSource)
    {
        heatSources.at(pos)->setValue(newValue);
    }
    else
    {
        thermalDiffusivities.at(pos)->setValue(newValue);
    }
}

QTextStream & model::operator<<(QTextStream & out, SimulationSetup & simSetup)
{
    out.setRealNumberNotation(QTextStream::FixedNotation);
    out.setRealNumberPrecision(3);
    out << "Simulation:\n\n"

        << "Einstellung fuer die Zeitdiskretisierung:"
        << "\nAusgewaehlte Integrationsmethode: " << simSetup.selectedIntMethod
        << "\nAnzahl Zeitschritte M: " << (int) simSetup.m
        << "\nEndzeitpunkt T: " << simSetup.T
        << "s\n\n"

        << "Einstellungen fuer den LGS Loeser:"
        << "\nAusgewaehlter Loeser: " << simSetup.selectedSolver;
    out.setRealNumberNotation(QTextStream::ScientificNotation);
    out.setRealNumberPrecision(0);
    out << "\nRelative Genauigkeit: " << simSetup.solverMaxError
        << "\nMaximale Iterationsanzahl: " << simSetup.solverMaxIt
        << "\n\n"

        << "Einstellungen fuer die Ortsdiskretisierung:"
        << "\nAnzahl Stuetzstellen N: " << (int) simSetup.n
        << "\n\n"

        << "Einstellungen fuer die IBVs:";
    out.setRealNumberNotation(QTextStream::FixedNotation);
    out.setRealNumberPrecision(2);
    out.setFieldWidth(8);
    out.setFieldAlignment(QTextStream::AlignRight);
    out << "\n     Anfangswert: "
        << simSetup.iBVs[SimulationSetup::InitialValue]
        << "K\nUnterer Randwert: "
        << simSetup.iBVs[SimulationSetup::BottomBoundary]
        << "K\n Linker Randwert: "
        << simSetup.iBVs[SimulationSetup::LeftBoundary]
        << "K\n Oberer Randwert: "
        << simSetup.iBVs[SimulationSetup::TopBoundary]
        << "K\nRechter Randwert: "
        << simSetup.iBVs[SimulationSetup::RightBoundary]
        << "K\n        \n";
    out.setFieldWidth(0);

    out << "Einstellungen fuer die Temperaturleitkoeffizienten:"
        << "\nAnzahl  Gebiete: " << simSetup.thermalDiffusivitiesCount;
    out.setRealNumberNotation(QTextStream::ScientificNotation);
    out.setRealNumberPrecision(2);
    out << "\nHintergrundwert: " << simSetup.thermalDiffusivitiesBackgroundValue << "m^2/s";
    if(simSetup.thermalDiffusivitiesCount > 0)
    {
        out << "\nAuflistung der Gebiete (Werte in m^2/s):"
            << "\n\n";
        int count = 1;
        QList<Area*>::const_iterator it = simSetup.thermalDiffusivities.begin();
        for(; it != simSetup.thermalDiffusivities.end(); ++it, ++count)
        {
            out.setRealNumberNotation(QTextStream::ScientificNotation);
            out.setRealNumberPrecision(2);
            out << count << ".Gebiet:\n"
                << *(*it);
        }
    }
    else
        out << "\n\n";

    out << "Einstellungen fuer die Waermequellen:"
        << "\nAnzahl  Gebiete: " << simSetup.heatSourcesCount;
    out.setRealNumberNotation(QTextStream::FixedNotation);
    out.setRealNumberPrecision(0);
    out << "\nHintergrundwert: " << simSetup.heatSourcesBackgroundValue << "K/s";
    if(simSetup.heatSourcesCount > 0)
    {
        out << "\nAuflistung der Gebiete (Werte in K/s):"
            << "\n\n";
       int count = 1;
       QList<Area*>::const_iterator it = simSetup.heatSources.begin();
       for(; it != simSetup.heatSources.end(); ++it, ++count)
       {
           out.setRealNumberPrecision(0);
           out << count << ".Gebiet:\n"
               << *(*it);
       }
    }
    else
        out << "\n\n";

    return out;
}

QTextStream & model::operator>>(QTextStream & in, SimulationSetup & simSetup)
{
    in.readLine();
    in.readLine();
    in.readLine();

    QString str;
    in >> str;
    in >> str;
    in >> simSetup.selectedIntMethod;
    in >> str;
    simSetup.selectedIntMethod += " " + str;

    in >> str;
    in >> str;
    in >> str;
    in >> simSetup.m;

    in >> str;
    in >> str;
    in >> simSetup.T;
//    in >> str;

    in.readLine();
    in.readLine();
    in.readLine();

    in >> str;
    in >> str;
    in >> simSetup.selectedSolver;
    if(simSetup.selectedSolver == "Gauss")
    {
        in >> str;
        simSetup.selectedSolver += " " + str;
    }

    in >> str;
    in >> str;
    in >> simSetup.solverMaxError;

    in >> str;
    in >> str;
    in >> simSetup.solverMaxIt;

    in.readLine();
    in.readLine();
    in.readLine();

    in >> str;
    in >> str;
    in >> str;
    in >> simSetup.n;

    in.readLine();
    in.readLine();
    in.readLine();

    in >> str;
    in >> simSetup.iBVs[SimulationSetup::InitialValue];
    in.readLine();

    in >> str;
    in >> str;
    in >> simSetup.iBVs[SimulationSetup::BottomBoundary];
    in.readLine();

    in >> str;
    in >> str;
    in >> simSetup.iBVs[SimulationSetup::LeftBoundary];
    in.readLine();

    in >> str;
    in >> str;
    in >> simSetup.iBVs[SimulationSetup::TopBoundary];
    in.readLine();

    in >> str;
    in >> str;
    in >> simSetup.iBVs[SimulationSetup::RightBoundary];
    in.readLine();

    in.readLine();
    in.readLine();

    in >> str;
    in >> str;
    in >> simSetup.thermalDiffusivitiesCount;

    in >> str;
    in >> simSetup.thermalDiffusivitiesBackgroundValue;
    in.readLine();

    if(simSetup.thermalDiffusivitiesCount > 0)
    {
        in.readLine();
        in.readLine();
        int type, id, count;
        double value, tmp;
        QVector<double> x,y;
        for(int i = 0; i < simSetup.thermalDiffusivitiesCount; ++i)
        {
            in.readLine();
            in >> str;
            in >> type;
            in >> str;
            in >> id;
            in >> str;
            in >> value;
            in >> str;
            in >> str;
            in >> count;
            x.resize(count);
            y.resize(count);
            for(int j = 0; j < count; ++j)
            {
                in >> tmp;
                x[j] = tmp;
                in.read(1);
                in >> tmp;
                y[j] = tmp;
            }
            in.readLine();
            in.readLine();
            simSetup.thermalDiffusivities.append(new Area(x,y,value,type));
        }
    }

    in.readLine();

    in >> str;
    in >> str;
    in >> simSetup.heatSourcesCount;

    in >> str;
    in >> simSetup.heatSourcesBackgroundValue;
    in.readLine();

    if(simSetup.heatSourcesCount > 0)
    {
        in.readLine();
        in.readLine();
        int type, id, count;
        double value, tmp;
        QVector<double> x,y;
        for(int i = 0; i < simSetup.heatSourcesCount; ++i)
        {
            in.readLine();
            in >> str;
            in >> type;
            in >> str;
            in >> id;
            in >> str;
            in >> value;
            in >> str;
            in >> str;
            in >> count;
            x.resize(count);
            y.resize(count);
            for(int j = 0; j < count; ++j)
            {
                in >> tmp;
                x[j] = tmp;
                in.read(1);
                in >> tmp;
                y[j] = tmp;
            }
            in.readLine();
            in.readLine();
            simSetup.heatSources.append(new Area(x,y,value,type));
        }
    }

    in.readLine();

    return in;
}
