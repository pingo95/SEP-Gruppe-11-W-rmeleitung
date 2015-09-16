#include "Simulationworker.h"
#include "../algorithms/Impeuler.h"
#include "../algorithms/Cranknicolson.h"
#include "../algorithms/Jacobi.h"
#include "../algorithms/Gaussseidel.h"

model::SimulationWorker::SimulationWorker(QObject * parent): QObject(parent),
    busy(false), consecutiveTempArray(NULL), m(1), mapsInitialized(false), n(3),
    result(NULL), simulated(false), T(1.)
{

}

QList<QString> const model::SimulationWorker::getIntMethodNames() const
{
    assert(mapsInitialized);
    return intMethods.keys();
}

QList<QString> const model::SimulationWorker::getIterativeSolverNames() const
{
    assert(mapsInitialized);
    return iterativeSolvers.keys();
}


double *** const & model::SimulationWorker::getResult() const
{
    if(!busy && simulated) return result;
    return NULL;
}

long model::SimulationWorker::getM() const
{
    if(!busy && simulated) return m;
    return 0;
}

long model::SimulationWorker::getN() const
{
    if(!busy && simulated) return n;
    return 0;
}

double model::SimulationWorker::getT() const
{
    if(!busy && simulated) return T;
    return 0.;
}

model::SimulationWorker::~SimulationWorker()
{
    QList<algorithms::IntMethod*> tmpList1 = intMethods.values();
    QList<algorithms::IntMethod*>::iterator it2 = tmpList1.begin();
    for(; it2 != tmpList1.end(); ++it2)
        delete (*it2);
    QList<algorithms::IterativeSolver*> tmpList2 = iterativeSolvers.values();
    QList<algorithms::IterativeSolver*>::iterator it3 = tmpList2.begin();
    for(; it3 != tmpList2.end(); ++it3)
        delete (*it3);
    if(simulated)
    {
        delete consecutiveTempArray;
        for(long i = 0; i < m+1; ++i)
            delete result[i];
        delete result;
    }
}

void model::SimulationWorker::initializeMaps()
{
    if(mapsInitialized) return;
    // Registrieren der Integrationsmethoden
    intMethods.insert("Impliziter Euler",new algorithms::ImpEuler());
    intMethods.insert("Crank Nicolson", new algorithms::CrankNicolson());

    // Registrieren der iterativen Löser
    iterativeSolvers.insert("Jacobi", new algorithms::Jacobi());
    iterativeSolvers.insert("Gauss-Seidel", new algorithms::GaussSeidel());
    mapsInitialized = true;
}

void model::SimulationWorker::startSimulationSlot(const double boundaryBottom,
                                                  const double boundaryLeft,
                                                  const double boundaryRight,
                                                  const double boundaryTop,
                                                  const QList<Area *> &heatSourcesTemplate,
                                                  const double heatSourceBackgroundValue,
                                                  const int heatSourcesCount,
                                                  const double initialValue,
                                                  const QString intMethod,
                                                  const QString iterativeSolver,
                                                  const long m, long const n,
                                                  double const solverMaxError,
                                                  int const solverMaxIt,
                                                  const double T,
                                                  const QList<Area *> &thermalConductivitiesTemplate,
                                                  const double thermalConductivitiesBackgroundValue,
                                                  const int thermalConductivitiesCount)
{
    if(!mapsInitialized || busy) return;
    busy = true;

    // Areas kopieren:
    QList<Area*> heatSources,thermalConductivities;
    QList<Area*>::const_iterator itCopy = heatSourcesTemplate.begin();
    for(; itCopy != heatSourcesTemplate.end(); ++itCopy)
        heatSources.append(new Area(*(*itCopy)));
    itCopy = thermalConductivitiesTemplate.begin();
    for(; itCopy != thermalConductivitiesTemplate.end(); ++itCopy)
        thermalConductivities.append(new Area(*(*itCopy)));

    emit startedSimulation();

    //altes ergebniss löschen
    if(simulated)
    {
        delete consecutiveTempArray;
        for(long i = 0; i < this->m+1; ++i)
            delete result[i];
        delete result;
    }


    // Deltas
    double deltaX = (double) 1 / (double) (n-1);
    double deltaT = T / (double) (m);

    QString message = "Beginne neue Simulation\n\nKonfiguration:\nZeitdiskretisierung\n\tm = "
            + QString::number(m) + "\n\tdeltaT = "+ QString::number(deltaT) + "\n\tT = " + QString::number(T)
            + "\nOrtsdiskretisierung\n\tn = " + QString::number(n) + "\n\tdeltaX = " + QString::number(deltaX)
            + "\n\nAllozieren des benötigten Speichers\n";
    emit simulationLogUpdate(message);

    // konsekutives Anlegen
    consecutiveTempArray = new double [(m+1)*n*n];
    // umwandeln in die Ergebnismatrix
    result = new double**[m];
    for(long i = 0; i < m+1; ++i)
    {
        result[i] = new double *[n];

        // Erste Zeilen mit unterem Randwert
        result[i][0] = &(consecutiveTempArray[i*n*n]);
        for(long k = 0; k < n; ++k)
            result[i][0][k] = boundaryBottom;

        for(long j = 1; j < n-1; ++j)
        {
            result[i][j] = &(consecutiveTempArray[i*n*n + j*n]);

            // Erste und letzter Punkt mit linkem bzw rechtem Randwert
            result[i][j][0] = boundaryLeft;
            result[i][j][n-1] = boundaryRight;
            // Inneren Punkt initialisiert mit  initialValue
            for(long k = 1; k < n-1; ++k)
                result[i][j][k] = initialValue;
        }

        // Letzte Zeilen mit oberem Randwert
        result[i][n-1] = &(consecutiveTempArray[i*n*n + (n-1)*n]);
        for(long k = 0; k < n; ++k)
            result[i][n-1][k] = boundaryTop;
    }
    message = "Speicher alloziert.\n\nBerechne Wärmeleitkoeffizienten\nAnzahl Gebiete : "
            + QString::number(thermalConductivitiesCount);
    emit simulationLogUpdate(message);

    // Anlegen der Vektoren für Wärmeleitkoeffizienten
    QVector<double> thermalConductivitiesGrid(n*n,thermalConductivitiesBackgroundValue);

    // Berechnen welche Punkte von welchem Wärmeleitkoeffizienten-Gebiet
    // abgedeckt werden, dabei überschreiben neure Gebiete ältere
    emit beginningSimulationStage("Wärmeleitkoeffizienten:",thermalConductivitiesCount);
    if(thermalConductivitiesCount > 0)
    {
        QList<Area*>::const_iterator it = thermalConductivities.begin();
        for(; it != thermalConductivities.end(); ++it)
        {
            int count = 0;
            Area* thermalConductivity = *it;
            double conductivity = thermalConductivity->getValue();
            double xMin, xMax, yMin, yMax;
            thermalConductivity->getTransitiveRectangle(xMin,xMax,yMin,yMax);
            long xLBound = ceil(xMin/deltaX),
                    xUBound = floor(xMax/deltaX),
                    yLBound = ceil(yMin/deltaX),
                    yUBound = floor(yMax/deltaX);
            for(long i = xLBound; i <= xUBound; ++i)
                for(long j = yLBound; j <= yUBound; ++j)
                    if(thermalConductivity->insidePoint(i*deltaX,j*deltaX))
                        thermalConductivitiesGrid[i+j*n] = conductivity;    //thermalConductivity.getValue(i*deltaX,j*deltaX);
            emit simulationLogUpdate(QString::number(++count) + ". Gebiet abgeschlossen");
            emit finishedStep(count);
        }
    }
    message = "Wärmeleitkoeffizienten abgeschlossen\n\n Berechne Wärmequellen\nAnzahl Gebiete: "
            + QString::number(heatSourcesCount) + "\n";
    emit simulationLogUpdate(message);

    bool reusable;
    QVector<double> neededTimeSteps;

    algorithms::IntMethod * selectedIntMethod = intMethods[intMethod];
    selectedIntMethod->selectIterativeSolver(iterativeSolvers[iterativeSolver]);
    selectedIntMethod->getIterativeSolver()->setEps(solverMaxError);
    selectedIntMethod->getIterativeSolver()->setMaxIt(solverMaxIt);

    selectedIntMethod->getNeedetHeatSources(neededTimeSteps, reusable); // timesteps in vielfachen von deltaT, "neuester" zuerst
    int neededTimeStepsCount = neededTimeSteps.size();

    QVector<QVector<double> *> heatSourcesGrid(neededTimeStepsCount,NULL);

    // Berechnen welche Punkte von welcher Wärmequelle abgedeckt werden,
    // zwischenspeichern als Indizes
    emit beginningSimulationStage("Wärmequellen", heatSourcesCount);
    QList<QList<double> *> heatSourceIndices;
    if(heatSourcesCount > 0)
    {
        int count = 0;
        QList<Area*>::const_iterator it = heatSources.begin();
        for(; it != heatSources.end(); ++it)
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
        heatSourcesGrid[i] = new QVector<double>(n*n,heatSourceBackgroundValue);

    emit simulationLogUpdate("Initiales Auswerten der Wärmequellen\n");
    // Initiales Auswerten der Wärmequellenvektoren
    if(heatSourcesCount > 0)
    {
        for(int i = 0; i < neededTimeStepsCount; ++i)
        {
    //        currentT = deltaT * neededTimeSteps[i];
            QList<Area*>::const_iterator it = heatSources.begin();
            QList<QList<double> *>::const_iterator it2 = heatSourceIndices.begin();
            for(; it != heatSources.end(); ++it,++it2)
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

    // Intialisieren der Int-Methode
    selectedIntMethod->setUp(n,m,T,thermalConductivitiesGrid);

    // Iterationsvektoren
    QVector<double> * step1 = new QVector<double>(n*n,0);
    QVector<double> * swapTmp;
    for(long i = 0; i < n; ++i)
        for(long j = 0; j < n; ++j)
            (*step1)[i*n + j] = result[0][i][j];
    QVector<double> * step2 = new QVector<double>(*step1);

    // Berechnen der Zeitschritte
    emit beginningSimulationStage("Zeitschritte berechnen:",m);
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
            if(heatSourcesCount > 0)
            {
        //        currentT = deltaT * neededTimeSteps[0] + i * deltaT;
                QList<Area*>::const_iterator it = heatSources.begin();
                QList<QList<double> *>::const_iterator it2 = heatSourceIndices.begin();
                for(; it != heatSources.end(); ++it,++it2)
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
            if(heatSourcesCount > 0)
            {
                for(int k = 0; k < neededTimeStepsCount; ++k)
                {
            //        currentT = deltaT * neededTimeSteps[k] + i * deltaT;
                    QList<Area*>::const_iterator it = heatSources.begin();
                    QList<QList<double> *>::const_iterator it2 = heatSourceIndices.begin();
                    for(; it != heatSources.end(); ++it,++it2)
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
                + QString::number(selectedIntMethod->getIterativeSolver()->getItCount()) + "\n";
        emit simulationLogUpdate(message);
    }

    this->m = m;
    this->n = n;
    this->T = T;

    message = "\nBerechnungen beendet\n\nAufräumen des Hauptspeichers\n\n"; //\nErgebnis:" + printResult()
    emit simulationLogUpdate(message);

    QList<Area*>::iterator it = heatSources.begin();
    for(; it != heatSources.end(); ++it)
        delete (*it);
    it = thermalConductivities.begin();
    for(; it != thermalConductivities.end(); ++it)
        delete (*it);

    QList<QList<double> *>::iterator it2 = heatSourceIndices.begin();
    for(; it2 != heatSourceIndices.end(); ++it2)
        delete (*it2);

    while(heatSourcesGrid.size() > 0)
        delete heatSourcesGrid.takeFirst();

    delete step1;
    delete step2;

    emit simulationLogUpdate("Simulation abgeschlossen\n\n");
    simulated = true;

    busy = false;
    emit finishedSimulation();
}

//QString model::SimulationWorker::printResult()
//{
//    QString output;
//    output += "\nAusgabe der Ergebnis-Matrix:\n\n";
//    for(long i = 0; i < m+1; ++i)
//    {
//        output += "m = " + QString::number(i) + "\n";
//        for(long k = 0; k < n; ++k)
//            output += "-------------";
//        output += "\nj\\k\t| ";
//        for(long k = 0; k < n-1; ++k)
//            output += QString::number(k) + "\t| ";
//        output += QString::number(n-1) + "\n";
//        for(long k = 0; k < n; ++k)
//            output += "-------------";
//        output += "\n";
//        for(long j = n-1; j >= 0; --j)
//        {
//            output += QString::number(j) + "\t| ";
//            for(long k = 0; k < n-1; ++k)
//            {
//                output += QString::number(result[i][j][k]) + "\t| ";
//            }
//            output += QString::number(result[i][j][n-1]) + "\n";
//            for(long k = 0; k < n; ++k)
//                output += "-------------";
//            output += "\n";
//        }
//        output += "\n";
//    }
//    output += "\n";
//    return output;
//}

//QString model::SimulationWorker::printVector(const QVector<double> &vec)
//{
//    QString output;
//    output += "\n";
//    for(long k = 0; k <= n; ++k)
//        output += "------";
//    output += "\nj\\k| ";
//    for(long k = 0; k < n-1; ++k)
//        output += QString::number(k) + "| ";
//    output += QString::number(n-1) + "\n";
//    for(long k = 0; k <= n; ++k)
//        output += "------";
//    output += "\n";
//    for(long j = n-1; j >= 0; --j)
//    {
//        output += QString::number(j) + "| ";
//        for(long k = 0; k < n-1; ++k)
//        {
//            output += QString::number(vec[k + j*n]) + "| ";
//        }
//        output += QString::number(vec[n-1 + j*n]) + "\n";
//        for(long k = 0; k <= n; ++k)
//            output += "------";
//        output += "\n";
//    }
//    output += "\n";
//    return output;
//}
