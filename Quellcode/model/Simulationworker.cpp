#include "Simulationworker.h"
#include "../algorithms/Impeuler.h"
#include "../algorithms/Cranknicolson.h"
#include "../algorithms/Jacobi.h"
#include "../algorithms/Gaussseidel.h"
#include "../algorithms/LU.h"
#include <QFile>
#include <QTextStream>
#include <QTime>

model::SimulationWorker::SimulationWorker(QObject * parent): QObject(parent),
    busy(false), consecutiveArrayObservations(NULL),
    consecutiveArraySimulation(NULL), dataRead(false), m(1),
    mapsInitialized(false), n(3), obsDim(0), result(NULL), simulated(false), T(1.)
{

}

model::SimulationWorker::~SimulationWorker()
{
    QList<algorithms::IntMethod*> tmpList1 = intMethods.values();
    QList<algorithms::IntMethod*>::iterator it2 = tmpList1.begin();
    for(; it2 != tmpList1.end(); ++it2)
        delete (*it2);
    QList<algorithms::Solver*> tmpList2 = solvers.values();
    QList<algorithms::Solver*>::iterator it3 = tmpList2.begin();
    for(; it3 != tmpList2.end(); ++it3)
        delete (*it3);
    if(simulated)
    {
        delete consecutiveArraySimulation;
        for(long i = 0; i < m+1; ++i)
            delete result[i];
        delete result;
    }
    if(dataRead)
    {
        delete consecutiveArrayObservations;
        delete observations;
    }
}

QList<QString> const model::SimulationWorker::getIntMethodNames() const
{
    assert(mapsInitialized);
    return intMethods.keys();
}

QList<QString> const model::SimulationWorker::getSolverNames() const
{
    assert(mapsInitialized);
    return solvers.keys();
}

long model::SimulationWorker::getM() const
{
    assert(!busy && simulated);
    return m;
}

long model::SimulationWorker::getN() const
{
    assert(!busy && simulated);
    return n;
}

double** const & model::SimulationWorker::getObservations() const
{
    assert(!busy && dataRead) ;
    return observations;
}

int model::SimulationWorker::getObservationsDim() const
{
    assert(!busy && dataRead);
    return obsDim;
}

double *** const & model::SimulationWorker::getResult() const
{
    assert(!busy && simulated);
    return result;
}

double model::SimulationWorker::getT() const
{
    assert(!busy && simulated);
    return T;
}

void model::SimulationWorker::initializeMaps()
{
    if(mapsInitialized) return;
    // Registrieren der Integrationsmethoden
    intMethods.insert("Impliziter Euler",new algorithms::ImpEuler());
    intMethods.insert("Crank Nicolson", new algorithms::CrankNicolson());

    // Registrieren der iterativen Löser
    solvers.insert("Jacobi", new algorithms::Jacobi());
    solvers.insert("Gauss-Seidel", new algorithms::GaussSeidel());
    solvers.insert("Gauss (LU)",new algorithms::LU());
    mapsInitialized = true;
}

void model::SimulationWorker::startReadingData(QString const filename, long const obsCount)
{
    if(busy) return;
    busy = true;

    emit startedWork();
    emit beginningStage("Messungen einlesen:",obsCount,false);
    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Truncate)) {
        QTextStream in(&file);
        obsDim = sqrt(obsCount);
        if(obsDim*obsDim != obsCount)
            obsDim += 1;
        long count = 0;

        //alte Werte löschen:
        if(dataRead)
        {
            delete consecutiveArrayObservations;
            delete observations;
        }
        consecutiveArrayObservations = new double[obsDim*obsDim];
        observations = new double*[obsDim];
        for(long i = 0; i < obsDim; ++i)
        {
            observations[i] = &(consecutiveArrayObservations[i*obsDim]);
            for(long j= 0; j < obsDim; ++j)
                observations[i][j] = 0.0;
        }
        for(long i = 0; i < obsDim; ++i)
        {
            for(long j = 0; j < obsDim; ++j)
            {
                if(count == obsCount)
                    break;
                in >> observations[i][j];
                ++count;
                emit finishedStep(i*obsDim+j+1,false);
            }
            if(count == obsCount)
                break;
        }
    }

    dataRead = true;
    busy = false;
    emit finishedReadingData();
}


void model::SimulationWorker::startSimulationSlot(SimulationSetup * simSetupTemplate)
{
    if(!mapsInitialized || busy) return;
    busy = true;
    QTime timer;
    timer.start();
    SimulationSetup  simSetup(*simSetupTemplate);

    emit startedWork();

    //altes ergebniss löschen
    if(simulated)
    {
        delete consecutiveArraySimulation;
        for(long i = 0; i < m+1; ++i)
            delete result[i];
        delete result;
    }

    this->m = simSetup.getM();
    this->n = simSetup.getN();
    this->T = simSetup.getT();

    // Deltas
    double deltaX = (double) 1 / (double) (n-1);
    double deltaT = T / (double) (m);

    QString message = "Beginne neue Simulation\n\nKonfiguration:\nZeitdiskretisierung\n\tm = "
            + QString::number(m) + "\n\tdeltaT = "+ QString::number(deltaT) + "\n\tT = " + QString::number(T)
            + "\nOrtsdiskretisierung\n\tn = " + QString::number(n) + "\n\tdeltaX = " + QString::number(deltaX)
            + "\n\nAllozieren des benötigten Speichers\n";
    emit simulationLogUpdate(message);

    // konsekutives Anlegen
    consecutiveArraySimulation = new double [(m+1)*n*n];
    // umwandeln in die Ergebnismatrix
    result = new double**[m+1];
    for(long i = 0; i < m+1; ++i)
    {
        result[i] = new double *[n];

        // Erste Zeilen mit unterem Randwert
        result[i][0] = &(consecutiveArraySimulation[i*n*n]);
        for(long k = 0; k < n; ++k)
            result[i][0][k] = simSetup.getIBV(SimulationSetup::BottomBoundary);

        for(long j = 1; j < n-1; ++j)
        {
            result[i][j] = &(consecutiveArraySimulation[i*n*n + j*n]);

            // Erste und letzter Punkt mit linkem bzw rechtem Randwert
            result[i][j][0] = simSetup.getIBV(SimulationSetup::LeftBoundary);
            result[i][j][n-1] = simSetup.getIBV(SimulationSetup::RightBoundary);
            // Inneren Punkt initialisiert mit  initialValue
            for(long k = 1; k < n-1; ++k)
                result[i][j][k] = simSetup.getIBV(SimulationSetup::InitialValue);
        }

        // Letzte Zeilen mit oberem Randwert
        result[i][n-1] = &(consecutiveArraySimulation[i*n*n + (n-1)*n]);
        for(long k = 0; k < n; ++k)
            result[i][n-1][k] = simSetup.getIBV(SimulationSetup::TopBoundary);
    }
    message = "Speicher alloziert.\n\nBerechne Wärmeleitkoeffizienten\nAnzahl Gebiete : "
            + QString::number(simSetup.getAreaCount(SimulationSetup::AreaThermalDiffusivity)) + "\n";
    emit simulationLogUpdate(message);

    // Anlegen der Vektoren für Wärmeleitkoeffizienten
    QVector<double> thermalDiffusivitiesGrid(n*n,simSetup.getAreaBackgroundValue(SimulationSetup::AreaThermalDiffusivity));

    // Berechnen welche Punkte von welchem Wärmeleitkoeffizienten-Gebiet
    // abgedeckt werden, dabei überschreiben neure Gebiete ältere
    emit beginningStage("Wärmeleitkoeffizienten:",simSetup.getAreaCount(SimulationSetup::AreaThermalDiffusivity));
    if(simSetup.getAreaCount(SimulationSetup::AreaThermalDiffusivity) > 0)
    {
        QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaThermalDiffusivity).begin();
        for(; it != simSetup.getAreas(SimulationSetup::AreaThermalDiffusivity).end(); ++it)
        {
            int count = 0;
            Area* thermalDiffusivity = *it;
            double diffusivity = thermalDiffusivity->getValue();
            double xMin, xMax, yMin, yMax;
            thermalDiffusivity->getTransitiveRectangle(xMin,xMax,yMin,yMax);
            long xLBound = ceil(xMin/deltaX),
                    xUBound = floor(xMax/deltaX),
                    yLBound = ceil(yMin/deltaX),
                    yUBound = floor(yMax/deltaX);
            for(long i = xLBound; i <= xUBound; ++i)
                for(long j = yLBound; j <= yUBound; ++j)
                    if(thermalDiffusivity->insidePoint(i*deltaX,j*deltaX))
                        thermalDiffusivitiesGrid[i+j*n] = diffusivity;    //thermalDiffusivity.getValue(i*deltaX,j*deltaX);
            emit simulationLogUpdate(QString::number(++count) + ". Gebiet abgeschlossen\n");
            emit finishedStep(count);
        }
    }
    message = "Wärmeleitkoeffizienten abgeschlossen\n\nBerechne Wärmequellen\nAnzahl Gebiete: "
            + QString::number(simSetup.getAreaCount(SimulationSetup::AreaHeatSource)) + "\n";
    emit simulationLogUpdate(message);

    bool reusable;
    QVector<double> neededTimeSteps;

    algorithms::IntMethod * selectedIntMethod = intMethods[simSetup.getSelectedIntMethod()];
    selectedIntMethod->selectSolver(solvers[simSetup.getSelectedSolver()]);
    selectedIntMethod->getSolver()->setEps(simSetup.getSolverMaxError());
    selectedIntMethod->getSolver()->setMaxIt(simSetup.getSolverMaxIt());

    selectedIntMethod->getNeedetHeatSources(neededTimeSteps, reusable); // timesteps in vielfachen von deltaT, "neuester" zuerst
    int neededTimeStepsCount = neededTimeSteps.size();

    QVector<QVector<double> *> heatSourcesGrid(neededTimeStepsCount,NULL);

    // Berechnen welche Punkte von welcher Wärmequelle abgedeckt werden,
    // zwischenspeichern als Indizes
    emit beginningStage("Wärmequellen", simSetup.getAreaCount(SimulationSetup::AreaHeatSource));
    QList<QList<double> *> heatSourceIndices;
    if(simSetup.getAreaCount(SimulationSetup::AreaHeatSource) > 0)
    {
        int count = 0;
        QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaHeatSource).begin();
        for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it)
        {
            QList<double> * tmpListPtr = new QList<double>;
            Area* heatSource = *it;
            double xMin, xMax, yMin, yMax;
            heatSource->getTransitiveRectangle(xMin,xMax,yMin,yMax);
            long xLBound = xMin > 0 ? ceil(xMin/deltaX) : 1,
                    xUBound = xMax < 1 ? floor(xMax/deltaX) : n-2,
                    yLBound = yMin > 0 ? ceil(yMin/deltaX) : 1,
                    yUBound = yMax < 1 ? floor(yMax/deltaX) : n-2;
            for(long i = xLBound; i <= xUBound; ++i)
                for(long j = yLBound; j <= yUBound; ++j)
                    if(heatSource->insidePoint(i*deltaX,j*deltaX))
                        tmpListPtr->append(i+j*n);
            heatSourceIndices.append(tmpListPtr);
            emit simulationLogUpdate(QString::number(++count) + ". Gebiet abgeschlossen\n");
            emit finishedStep(count);
        }
    }

    for(int i = 0; i < neededTimeStepsCount; ++i)
        heatSourcesGrid[i] = new QVector<double>(n*n,simSetup.getAreaBackgroundValue(SimulationSetup::AreaHeatSource));

    emit simulationLogUpdate("Initiales Auswerten der Wärmequellen\n");
    // Initiales Auswerten der Wärmequellenvektoren
    if(simSetup.getAreaCount(SimulationSetup::AreaHeatSource) > 0)
    {
        for(int i = 0; i < neededTimeStepsCount; ++i)
        {
    //        currentT = deltaT * neededTimeSteps[i];
            QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaHeatSource).begin();
            QList<QList<double> *>::const_iterator it2 = heatSourceIndices.begin();
            for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it,++it2)
            {
                QList<double>::const_iterator it3 = (*it2)->begin();
                for(; it3 != (*it2)->end(); ++it3)
                {
                    long pos = (*it3);
                    (*(heatSourcesGrid[i]))[pos] = (*it)->getValue(); //(*it)->getValue(currentT,(pos % n) * deltaX,((pos - (pos % n)) / n) + deltaX);
                }
            }
        }
    }

    message = "Wärmequellen abgeschlossen\n\n Initialisieren der Integrationsmethode und des Lösers\n\n";
    emit simulationLogUpdate(message);

    emit beginningStage("Zeitschritte berechnen:",m);
    // Intialisieren der Int-Methode
    selectedIntMethod->setUp(n,m,T,thermalDiffusivitiesGrid);

    // Iterationsvektoren
    QVector<double> * step1 = new QVector<double>(n*n,0);
    QVector<double> * swapTmp;
    for(long i = 0; i < n; ++i)
        for(long j = 0; j < n; ++j)
            (*step1)[i*n + j] = result[0][i][j];
    QVector<double> * step2 = new QVector<double>(*step1);

    // Berechnen der Zeitschritte
    emit simulationLogUpdate("Zeitschritte berechnen\n");
    for(long i = 1; i < m+1; ++i)
    {
        selectedIntMethod->calcNextStep(*step1,*step2,heatSourcesGrid);
        for(long j = 1; j < n-1; ++j)       //<-- Achtung fall Ränder nicht mehr const
            for(long k = 1; k < n-1; ++k)
                result[i][j][k] = (*step2)[k+j*n];
        swapTmp = step1;
        step1 = step2;
        step2 = swapTmp;
        if(reusable)
        {
            // Wiederverwertbar -> ring swap rückwärts
            swapTmp = heatSourcesGrid[neededTimeStepsCount-1];
            for(int i = neededTimeStepsCount-1; i > 0; --i)
                heatSourcesGrid[i] = heatSourcesGrid[i-1];
            heatSourcesGrid[0] = swapTmp;

            // neusten aktualisieren
            if(simSetup.getAreaCount(SimulationSetup::AreaHeatSource) > 0)
            {
        //        currentT = deltaT * neededTimeSteps[0] + i * deltaT;
                QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaHeatSource).begin();
                QList<QList<double> *>::const_iterator it2 = heatSourceIndices.begin();
                for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it,++it2)
                {
                    QList<double>::const_iterator it3 = (*it2)->begin();
                    for(; it3 != (*it2)->end(); ++it3)
                    {
                        long pos = (*it3);
                        (*(heatSourcesGrid[0]))[pos] = (*it)->getValue(); //(*it)->getValue(currentT,(pos % n) * deltaX,((pos - (pos % n)) / n) + deltaX);
                    }
                }

            }
        }
        else
        {
            // nicht wiederverwertbar -> alle neu berechnen
            if(simSetup.getAreaCount(SimulationSetup::AreaHeatSource) > 0)
            {
                for(int k = 0; k < neededTimeStepsCount; ++k)
                {
            //        currentT = deltaT * neededTimeSteps[k] + i * deltaT;
                    QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaHeatSource).begin();
                    QList<QList<double> *>::const_iterator it2 = heatSourceIndices.begin();
                    for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it,++it2)
                    {
                        QList<double>::const_iterator it3 = (*it2)->begin();
                        for(; it3 != (*it2)->end(); ++it3)
                        {
                            long pos = (*it3);
                            (*(heatSourcesGrid[k]))[pos] = (*it)->getValue(); //(*it)->getValue(currentT,(pos % n) * deltaX,((pos - (pos % n)) / n) + deltaX);
                        }
                    }
                }
            }
        }
        emit finishedStep(i);
        message = QString::number(i) + ". Zeitschritt beendet\nBenötigte Iterationen des Lösers: "
                + QString::number(selectedIntMethod->getSolver()->getItCount()) + "\n";
        emit simulationLogUpdate(message);
    }

    message = "\nBerechnungen beendet\n\nAufräumen des Hauptspeichers\n\n"; //\nErgebnis:" + printResult()
    emit simulationLogUpdate(message);

    QList<QList<double> *>::iterator it2 = heatSourceIndices.begin();
    for(; it2 != heatSourceIndices.end(); ++it2)
        delete (*it2);

    while(heatSourcesGrid.size() > 0)
        delete heatSourcesGrid.takeFirst();

    delete step1;
    delete step2;
    int time = timer.elapsed();
    emit simulationLogUpdate("Simulation abgeschlossen\nBenötigte Zeit: "
                             + QString::number(round(time/(60*1000)))       + " min "
                             + QString::number(round(time%(60*1000)/1000))  + " s "
                             + QString::number(time%(60*1000)%1000)         + " ms\n\n");
    simulated = true;

    busy = false;
//    emit beginningStage("Simulation beendet",0);
    emit finishedSimulation();
}
