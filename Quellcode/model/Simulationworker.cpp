#include "Simulationworker.h"
//#include "../algorithms/dco.hpp"
#include "../algorithms/Crs.cpp"
#include "../algorithms/Intmethod.cpp"
#include "../algorithms/Solver.cpp"
#include "../algorithms/Iterativesolver.cpp"
#include "../algorithms/Impeuler.cpp"
#include "../algorithms/Cranknicolson.cpp"
#include "../algorithms/Jacobi.cpp"
#include "../algorithms/Gaussseidel.cpp"
#include "../algorithms/LU.cpp"
#include <QFile>
#include <QTextStream>
#include <QTime>

typedef double AD_TYPE;
//typedef dco::ga1s<double>::type AD_TYPE;
//typedef dco::ga1s<double> AD_MODE;

model::SimulationWorker::SimulationWorker(QObject * parent): QObject(parent),
    busy(false), consecutiveArrayObservations(NULL),
    consecutiveArraySimulation(NULL), dataRead(false), m(1),
    mapsInitialized(false), n(3), obsSize(0), result(NULL), simulated(false), T(1.),
    optimized(false), optimizationN(1)
{

}

model::SimulationWorker::~SimulationWorker()
{
    QList<algorithms::IntMethod<double>*> tmpList1 = intMethods.values();
    QList<algorithms::IntMethod<double>*>::iterator it2 = tmpList1.begin();
    for(; it2 != tmpList1.end(); ++it2)
        delete (*it2);
    QList<algorithms::Solver<double>*> tmpList2 = solvers.values();
    QList<algorithms::Solver<double>*>::iterator it3 = tmpList2.begin();
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
    return obsSize;
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
    intMethods.insert("Impliziter Euler",new algorithms::ImpEuler<double>());
    intMethods.insert("Crank Nicolson", new algorithms::CrankNicolson<double>());

    // Registrieren der iterativen Löser
    solvers.insert("Jacobi", new algorithms::Jacobi<double>());
    solvers.insert("Gauss-Seidel", new algorithms::GaussSeidel<double>());
    solvers.insert("Gauss (LU)",new algorithms::LU<double>());
    mapsInitialized = true;
}

void model::SimulationWorker::startOptimization(SimulationSetup *simSetupTemplate, bool overrideTD,
                                                double overrideValue, bool useHeatSources)
{
//    if(busy) return;
//    busy = true;

//    SimulationSetup  simSetup(*simSetupTemplate);

//    emit startedWork();

//    optimizationN = obsSize*obsSize;
//    simSetup.setN(obsSize);
//    // DeltaX
//    double deltaX = (double) 1 / (double) (simSetup.getN()-1);

//    // Anlegen des Vektors für zu optimierende Wärmeleitkoeffizienten
//    QVector<AD_TYPE> optimizedCsAD;
//    if(overrideTD)
//        optimizedCsAD.fill(optimizationN,overrideValue);
//    else
//    {
//        optimizedCsAD.fill(optimizationN,simSetup.getAreaBackgroundValue(SimulationSetup::AreaThermalDiffusivity));
//        // Berechnen welche Punkte von welchem Wärmeleitkoeffizienten-Gebiet
//        // abgedeckt werden, dabei überschreiben neure Gebiete ältere
//        emit beginningStage("Initiale Wärmeleitkoeffizienten:",simSetup.getAreaCount(SimulationSetup::AreaThermalDiffusivity),false);
//        if(simSetup.getAreaCount(SimulationSetup::AreaThermalDiffusivity) > 0)
//        {
//            QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaThermalDiffusivity).begin();
//            for(; it != simSetup.getAreas(SimulationSetup::AreaThermalDiffusivity).end(); ++it)
//            {
//                int count = 0;
//                Area* thermalDiffusivity = *it;
//                double diffusivity = thermalDiffusivity->getValue();
//                double xMin, xMax, yMin, yMax;
//                thermalDiffusivity->getTransitiveRectangle(xMin,xMax,yMin,yMax);
//                long xLBound = ceil(xMin/deltaX),
//                        xUBound = floor(xMax/deltaX),
//                        yLBound = ceil(yMin/deltaX),
//                        yUBound = floor(yMax/deltaX);
//                for(long i = xLBound; i <= xUBound; ++i)
//                    for(long j = yLBound; j <= yUBound; ++j)
//                        if(thermalDiffusivity->insidePoint(i*deltaX,j*deltaX))
//                            optimizedCsAD[i+j*n] = diffusivity;    //thermalDiffusivity.getValue(i*deltaX,j*deltaX);
//                emit finishedStep(++count,false);
//            }
//        }
//    }

//    // Berechnen welche Punkte von welcher Wärmequelle abgedeckt werden,
//    // zwischenspeichern als Indizes
//    QList<QList<long> *> heatSourceIndices;
//    if(useHeatSources && simSetup.getAreaCount(SimulationSetup::AreaHeatSource) > 0)
//    {
//        emit beginningStage("Wärmequellen", simSetup.getAreaCount(SimulationSetup::AreaHeatSource),false);
//        int count = 0;
//        QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaHeatSource).begin();
//        for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it)
//        {
//            QList<long> * tmpListPtr = new QList<long>;
//            Area* heatSource = *it;
//            double xMin, xMax, yMin, yMax;
//            heatSource->getTransitiveRectangle(xMin,xMax,yMin,yMax);
//            long xLBound = xMin > 0 ? ceil(xMin/deltaX) : 1,
//                    xUBound = xMax < 1 ? floor(xMax/deltaX) : n-2,
//                    yLBound = yMin > 0 ? ceil(yMin/deltaX) : 1,
//                    yUBound = yMax < 1 ? floor(yMax/deltaX) : n-2;
//            for(long i = xLBound; i <= xUBound; ++i)
//                for(long j = yLBound; j <= yUBound; ++j)
//                    if(heatSource->insidePoint(i*deltaX,j*deltaX))
//                        tmpListPtr->append(i+j*n);
//            heatSourceIndices.append(tmpListPtr);
//            emit finishedStep(++count,false);
//        }
//    }

//    QVector<AD_TYPE> * step1 = new QVector<AD_TYPE>(optimizationN,simSetup.getIBV(SimulationSetup::InitialValue));
//    QVector<AD_TYPE> * step2 = new QVector<AD_TYPE>(optimizationN,simSetup.getIBV(SimulationSetup::InitialValue));


//    //TODO: dco steepest decent algorithm:
//    AD_MODE::global_tape = AD_MODE::tape_t::create();

////    QVector<AD_TYPE> grad;
////    do
////    {
////        AD_MODE::global_tape->register_variable(optimizedCsAD,optimizationN);

////        QVector<AD_TYPE> * result = simpleSimulation(simSetup,step1,step2,optimizedCsAD,heatSourceIndices);

////        AD_TYPE J = 0;
////        for(int i = 0; i < obsSize; ++i)
////            for(int j = 0; j < obsSize; ++j)
////                J += ((*result)[i*obsSize + j] * observations[i][j])
////                        *((*result)[i*obsSize + j] * observations[i][j]);
////        dco::derivative(J) = 1;
////        AD_MODE::global_tape->interpret_adjoint();

////        grad = optimizedCsAD;
////        for(int i = 0; i < optimizationN; ++i)
////            grad[i] = dco::derivative(optimizedCsAD[i]);
////        AD_TYPE s = 0.0001;
////        for(int i = 0; i < optimizationN; ++i)
////            optimizedCsAD[i]  -= s * grad[i];
////    }
////    while(algorithms::norm2(grad)-eps > 0);



//    //Aufräumen des Speichers
//    QList<QList<long> *>::iterator it = heatSourceIndices.begin();
//    for(; it != heatSourceIndices.end(); ++it)
//        delete (*it);
//    delete step1;
//    delete step2;

//    optimized = true;
//    busy = false;
//    emit finishedOptimization();
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
        obsSize = sqrt(obsCount);
        if(obsSize*obsSize != obsCount)
            obsSize += 1;
        long count = 0;

        //alte Werte löschen:
        if(dataRead)
        {
            delete consecutiveArrayObservations;
            delete observations;
        }
        consecutiveArrayObservations = new double[obsSize*obsSize];
        observations = new double*[obsSize];
        for(long i = 0; i < obsSize; ++i)
        {
            observations[i] = &(consecutiveArrayObservations[i*obsSize]);
            for(long j= 0; j < obsSize; ++j)
                observations[i][j] = 0.0;
        }
        for(long i = 0; i < obsSize; ++i)
        {
            for(long j = 0; j < obsSize; ++j)
            {
                if(count == obsCount)
                    break;
                in >> observations[i][j];
                ++count;
                emit finishedStep(i*obsSize+j+1,false);
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

    algorithms::IntMethod<double> * selectedIntMethod = intMethods[simSetup.getSelectedIntMethod()];
    selectedIntMethod->selectSolver(solvers[simSetup.getSelectedSolver()]);
    selectedIntMethod->getSolver()->setEps(simSetup.getSolverMaxError());
    selectedIntMethod->getSolver()->setMaxIt(simSetup.getSolverMaxIt());

    selectedIntMethod->getNeedetHeatSources(neededTimeSteps, reusable); // timesteps in vielfachen von deltaT, "neuester" zuerst
    int neededTimeStepsCount = neededTimeSteps.size();

    QVector<QVector<double> *> heatSourcesGrid(neededTimeStepsCount,NULL);

    // Berechnen welche Punkte von welcher Wärmequelle abgedeckt werden,
    // zwischenspeichern als Indizes
    emit beginningStage("Wärmequellen", simSetup.getAreaCount(SimulationSetup::AreaHeatSource));
    QList<QList<long> *> heatSourceIndices;
    if(simSetup.getAreaCount(SimulationSetup::AreaHeatSource) > 0)
    {
        int count = 0;
        QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaHeatSource).begin();
        for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it)
        {
            QList<long> * tmpListPtr = new QList<long>;
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
            QList<QList<long> *>::const_iterator it2 = heatSourceIndices.begin();
            for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it,++it2)
            {
                QList<long>::const_iterator it3 = (*it2)->begin();
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
                QList<QList<long> *>::const_iterator it2 = heatSourceIndices.begin();
                for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it,++it2)
                {
                    QList<long>::const_iterator it3 = (*it2)->begin();
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
                    QList<QList<long> *>::const_iterator it2 = heatSourceIndices.begin();
                    for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it,++it2)
                    {
                        QList<long>::const_iterator it3 = (*it2)->begin();
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

    QList<QList<long> *>::iterator it2 = heatSourceIndices.begin();
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

QVector<double> * & model::SimulationWorker::simpleSimulation(SimulationSetup &simSetup, QVector<double> *&step1,
                                                              QVector<double> *&step2, QVector<double> &currentCs,
                                                              QList<QList<long> *> heatSourceIndices)
{
//    // Deltas
////    double deltaX = (double) 1 / (double) (simSetup.getN()-1);
////    double deltaT = simSetup.getT() / (double) (simSetup.getM());

//    step1->fill(simSetup.getIBV(SimulationSetup::InitialValue));

//    // Anfangswerte
//    for(long k = 0; k < simSetup.getN(); ++k)
//    {
//        (*step1)[k] = simSetup.getIBV(SimulationSetup::BottomBoundary);
//        (*step1)[(simSetup.getN()-1)*k] = simSetup.getIBV(SimulationSetup::LeftBoundary);
//        (*step1)[(simSetup.getN()-1)*k + n-1] = simSetup.getIBV(SimulationSetup::RightBoundary);
//        (*step1)[(simSetup.getN()-1)*simSetup.getN() + k] = simSetup.getIBV(SimulationSetup::TopBoundary);

//        (*step2)[k] = simSetup.getIBV(SimulationSetup::BottomBoundary);
//        (*step2)[(simSetup.getN()-1)*k] = simSetup.getIBV(SimulationSetup::LeftBoundary);
//        (*step2)[(simSetup.getN()-1)*k + n-1] = simSetup.getIBV(SimulationSetup::RightBoundary);
//        (*step2)[(simSetup.getN()-1)*simSetup.getN() + k] = simSetup.getIBV(SimulationSetup::TopBoundary);
//    }

//    bool reusable;
//    QVector<double> neededTimeSteps;

//    algorithms::IntMethod<AD_TYPE> * selectedIntMethod = intMethods[simSetup.getSelectedIntMethod()];
//    selectedIntMethod->selectSolver(solvers[simSetup.getSelectedSolver()]);
//    selectedIntMethod->getSolver()->setEps(simSetup.getSolverMaxError());
//    selectedIntMethod->getSolver()->setMaxIt(simSetup.getSolverMaxIt());

//    selectedIntMethod->getNeedetHeatSources(neededTimeSteps, reusable); // timesteps in vielfachen von deltaT, "neuester" zuerst
//    int neededTimeStepsCount = neededTimeSteps.size();

//    QVector<QVector<double> *> heatSourcesGrid(neededTimeStepsCount,NULL);

//    for(int i = 0; i < neededTimeStepsCount; ++i)
//        heatSourcesGrid[i] = new QVector<double>(n*n,simSetup.getAreaBackgroundValue(SimulationSetup::AreaHeatSource));

//    // Initiales Auswerten der Wärmequellenvektoren
//    if(simSetup.getAreaCount(SimulationSetup::AreaHeatSource) > 0)
//    {
//        for(int i = 0; i < neededTimeStepsCount; ++i)
//        {
//    //        currentT = deltaT * neededTimeSteps[i];
//            QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaHeatSource).begin();
//            QList<QList<long> *>::const_iterator it2 = heatSourceIndices.begin();
//            for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it,++it2)
//            {
//                QList<long>::const_iterator it3 = (*it2)->begin();
//                for(; it3 != (*it2)->end(); ++it3)
//                {
//                    long pos = (*it3);
//                    (*(heatSourcesGrid[i]))[pos] = (*it)->getValue(); //(*it)->getValue(currentT,(pos % n) * deltaX,((pos - (pos % n)) / n) + deltaX);
//                }
//            }
//        }
//    }
//    emit beginningStage("Zeitschritte berechnen:",simSetup.getM(),false);
//    selectedIntMethod->setUp(simSetup.getN(),simSetup.getM(),simSetup.getT(),currentCs);
//    QVector<double> * swapTmp;

//    for(long i = 1; i < simSetup.getM()+1; ++i)
//    {
//        selectedIntMethod->calcNextStep(*step1,*step2,heatSourcesGrid);

//        swapTmp = step1;
//        step1 = step2;
//        step2 = swapTmp;
//        if(reusable)
//        {
//            // Wiederverwertbar -> ring swap rückwärts
//            swapTmp = heatSourcesGrid[neededTimeStepsCount-1];
//            for(int i = neededTimeStepsCount-1; i > 0; --i)
//                heatSourcesGrid[i] = heatSourcesGrid[i-1];
//            heatSourcesGrid[0] = swapTmp;

//            // neusten aktualisieren
//            if(simSetup.getAreaCount(SimulationSetup::AreaHeatSource) > 0)
//            {
//        //        currentT = deltaT * neededTimeSteps[0] + i * deltaT;
//                QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaHeatSource).begin();
//                QList<QList<long> *>::const_iterator it2 = heatSourceIndices.begin();
//                for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it,++it2)
//                {
//                    QList<long>::const_iterator it3 = (*it2)->begin();
//                    for(; it3 != (*it2)->end(); ++it3)
//                    {
//                        long pos = (*it3);
//                        (*(heatSourcesGrid[0]))[pos] = (*it)->getValue(); //(*it)->getValue(currentT,(pos % n) * deltaX,((pos - (pos % n)) / n) + deltaX);
//                    }
//                }

//            }
//        }
//        else
//        {
//            // nicht wiederverwertbar -> alle neu berechnen
//            if(simSetup.getAreaCount(SimulationSetup::AreaHeatSource) > 0)
//            {
//                for(int k = 0; k < neededTimeStepsCount; ++k)
//                {
//            //        currentT = deltaT * neededTimeSteps[k] + i * deltaT;
//                    QList<Area*>::const_iterator it = simSetup.getAreas(SimulationSetup::AreaHeatSource).begin();
//                    QList<QList<long> *>::const_iterator it2 = heatSourceIndices.begin();
//                    for(; it != simSetup.getAreas(SimulationSetup::AreaHeatSource).end(); ++it,++it2)
//                    {
//                        QList<long>::const_iterator it3 = (*it2)->begin();
//                        for(; it3 != (*it2)->end(); ++it3)
//                        {
//                            long pos = (*it3);
//                            (*(heatSourcesGrid[k]))[pos] = (*it)->getValue(); //(*it)->getValue(currentT,(pos % n) * deltaX,((pos - (pos % n)) / n) + deltaX);
//                        }
//                    }
//                }
//            }
//        }
//        emit finishedStep(i,false);
//    }
//    return simSetup.getM() % 2 == 0 ? step1 : step2;
}
