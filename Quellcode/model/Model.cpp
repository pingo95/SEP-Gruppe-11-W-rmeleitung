#include "Model.h"
#include "../algorithms/Impeuler.h"
#include "../algorithms/Cranknicolson.h"
#include "../algorithms/Jacobi.h"
#include "../algorithms/Gaussseidel.h"
#include "../presentation/Ui.h"

model::Model::Model() : QWidget(NULL), boundaryBottom(300), boundaryLeft(300),
    boundaryRight(300), boundaryTop(300), heatSourcesBackgroundValue(0),
    heatSourcesCount(0), initialValue(300), m(1), n(3), selectedIntMethod("Impliziter Euler"),
    selectedIterativeSolver("Jacobi"), simulated(false), T(1.),
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
    return selectedIterativeSolver;
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
    selectedIterativeSolver = newIterativeSolver;
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
                         selectedIntMethod,selectedIterativeSolver,m,n,solverMaxError,solverMaxIt,
                         T,thermalConductivities,thermalConductivitiesBackgroundValue,
                         thermalConductivitiesCount);
//    simulating = true;
//    ui->updateNotification();
//    //altes ergebniss löschen
//    if(simulated)
//    {
//        delete consecutiveTempArray;
//        for(long i = 0; i < resultM+1; ++i)
//            delete result[i];
//        delete result;
//    }


//    // Deltas
//    double deltaX = (double) 1 / (double) (n-1);
//    double deltaT = T / (double) (m);

//    QString message = "Beginne neue Simulation\n\nKonfiguration:\nZeitdiskretisierung\n\tm = "
//            + QString::number(m) + "\n\tdeltaT = "+ QString::number(deltaT) + "\n\tT = " + QString::number(T)
//            + "\nOrtsdiskretisierung\n\tn = " + QString::number(n) + "\n\tdeltaX = " + QString::number(deltaX)
//            + "\n\nAllozieren des benötigten Speichers\n";
//    emit simulationLogUpdate(message);

//    // konsekutives Anlegen
//    consecutiveTempArray = new double [(m+1)*n*n];
//    // umwandeln in die Ergebnismatrix
//    result = new double**[m];
//    for(long i = 0; i < m+1; ++i)
//    {
//        result[i] = new double *[n];

//        // Erste Zeilen mit unterem Randwert
//        result[i][0] = &(consecutiveTempArray[i*n*n]);
//        for(long k = 0; k < n; ++k)
//            result[i][0][k] = boundaryBottom;

//        for(long j = 1; j < n-1; ++j)
//        {
//            result[i][j] = &(consecutiveTempArray[i*n*n + j*n]);

//            // Erste und letzter Punkt mit linkem bzw rechtem Randwert
//            result[i][j][0] = boundaryLeft;
//            result[i][j][n-1] = boundaryRight;
//            // Inneren Punkt initialisiert mit  initialValue
//            for(long k = 1; k < n-1; ++k)
//                result[i][j][k] = initialValue;
//        }

//        // Letzte Zeilen mit oberem Randwert
//        result[i][n-1] = &(consecutiveTempArray[i*n*n + (n-1)*n]);
//        for(long k = 0; k < n; ++k)
//            result[i][n-1][k] = boundaryTop;
//    }
//    message = "Speicher alloziert.\n\nBerechne Wärmeleitkoeffizienten\nAnzahl Gebiete : "
//            + QString::number(thermalConductivitiesCount);
//    emit simulationLogUpdate(message);

//    // Anlegen der Vektoren für Wärmeleitkoeffizienten
//    QVector<double> thermalConductivitiesGrid(n*n,thermalConductivitiesBackgroundValue);

//    // Berechnen welche Punkte von welchem Wärmeleitkoeffizienten-Gebiet
//    // abgedeckt werden, dabei überschreiben neure Gebiete ältere
////    emit beginningStage("Wärmeleitkoeffizienten:",thermalConductivitesCount);
//    if(thermalConductivitiesCount > 0)
//    {
//        QList<Area*>::const_iterator it = thermalConductivities.begin();
//        for(; it != thermalConductivities.end(); ++it)
//        {
//            int count = 0;
//            Area* thermalConductivity = *it;
//            double conductivity = thermalConductivity->getValue();
//            double xMin, xMax, yMin, yMax;
//            thermalConductivity->getTransitiveRectangle(xMin,xMax,yMin,yMax);
//            long xLBound = ceil(xMin/deltaX),
//                    xUBound = floor(xMax/deltaX),
//                    yLBound = ceil(yMin/deltaX),
//                    yUBound = floor(yMax/deltaX);
//            for(long i = xLBound; i <= xUBound; ++i)
//                for(long j = yLBound; j <= yUBound; ++j)
//                    if(thermalConductivity->insidePoint(i*deltaX,j*deltaX))
//                        thermalConductivitiesGrid[i+j*n] = conductivity;    //thermalConductivity.getValue(i*deltaX,j*deltaX);
////            emit finishedStep(++count);
//            emit simulationLogUpdate(QString::number(++count) + ". Gebiet abgeschlossen");
//        }
//    }
//    message = "Wärmeleitkoeffizienten abgeschlossen:\n" + printVector(thermalConductivitiesGrid)
//            + "\n\n Berechne Wärmequellen\nAnzahl Gebiete: " + QString::number(heatSourcesCount)
//            + "\n";
//    emit simulationLogUpdate(message);

//    bool reusable;
//    QVector<double> neededTimeSteps;

//    selectedIntMethod->getNeedetHeatSources(neededTimeSteps, reusable); // timesteps in vielfachen von deltaT, "neuester" zuerst
//    int neededTimeStepsCount = neededTimeSteps.size();

//    QVector<QVector<double> *> heatSourcesGrid(neededTimeStepsCount,NULL);

//    // Berechnen welche Punkte von welcher Wärmequelle abgedeckt werden,
//    // zwischenspeichern als Indizes
////    emit beginningStage("Wärmequellen", heatSourcesCount);
//    QList<QList<double> *> heatSourceIndices;
//    if(heatSourcesCount > 0)
//    {
//        int count = 0;
//        QList<Area*>::const_iterator it = heatSources.begin();
//        for(; it != heatSources.end(); ++it)
//        {
//            QList<double> * tmpListPtr = new QList<double>;
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
////            emit finishedStep(++count);
//            emit simulationLogUpdate(QString::number(++count) + ". Gebiet abgeschlossen");
//        }
//    }

//    for(int i = 0; i < neededTimeStepsCount; ++i)
//        heatSourcesGrid[i] = new QVector<double>(n*n,heatSourcesBackgroundValue);

//    emit simulationLogUpdate("Initiales Auswerten der Wärmequellen");
//    // Initiales Auswerten der Wärmequellenvektoren
//    if(heatSourcesCount > 0)
//    {
//        for(int i = 0; i < neededTimeStepsCount; ++i)
//        {
//    //        currentT = deltaT * neededTimeSteps[i];
//            QList<Area*>::const_iterator it = heatSources.begin();
//            QList<QList<double> *>::const_iterator it2 = heatSourceIndices.begin();
//            for(; it != heatSources.end(); ++it,++it2)
//            {
//                QList<double>::const_iterator it3 = (*it2)->begin();
//                for(; it3 != (*it2)->end(); ++it3)
//                {
//                    long pos = (*it3);
//                    (*(heatSourcesGrid[i]))[pos] = (*it)->getValue(); //(*it)->getValue(currentT,(pos % n) * deltaX,((pos - (pos % n)) / n) + deltaX);
//                }
//            }
//        }
//    }

//    message = "Wärmequellen abgeschlossen\nInitiale Auswertung:\n";
//    for(int i = 0; i < neededTimeStepsCount; ++i)
//        message += QString::number(i+1) + ". Wärmequelle\n" + printVector(*(heatSourcesGrid[i])) + "\n";
//    message += "\n Initialisieren der Integrationsmethode und des Lösers\n\n";
//    emit simulationLogUpdate(message);

//    // Intialisieren der Int-Methode
//    selectedIntMethod->setUp(n,m,T,thermalConductivitiesGrid);

//    // Iterationsvektoren
//    QVector<double> * step1 = new QVector<double>(n*n,0);
//    QVector<double> * swapTmp;
//    for(long i = 0; i < n; ++i)
//        for(long j = 0; j < n; ++j)
//            (*step1)[i*n + j] = result[0][i][j];
//    QVector<double> * step2 = new QVector<double>(*step1);

//    // Berechnen der Zeitschritte
//    emit beginningSimulationStage("Zeitschritte berechnen:",m);
//    emit simulationLogUpdate("Zeitschritte berechnen\n1. Zeitschritt beendet\n");
//    for(long i = 1; i < m+1; ++i)
//    {
//        selectedIntMethod->calcNextStep(*step1,*step2,heatSourcesGrid);
//        for(long j = 1; j < n-1; ++j)       //<-- Achtung fall Ränder nicht mehr const
//            for(long k = 1; k < n-1; ++k)
//                result[i][j][k] = (*step2)[k+j*n];
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
//            if(heatSourcesCount > 0)
//            {
//        //        currentT = deltaT * neededTimeSteps[0] + i * deltaT;
//                QList<Area*>::const_iterator it = heatSources.begin();
//                QList<QList<double> *>::const_iterator it2 = heatSourceIndices.begin();
//                for(; it != heatSources.end(); ++it,++it2)
//                {
//                    QList<double>::const_iterator it3 = (*it2)->begin();
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
//            if(heatSourcesCount > 0)
//            {
//                for(int k = 0; k < neededTimeStepsCount; ++k)
//                {
//            //        currentT = deltaT * neededTimeSteps[k] + i * deltaT;
//                    QList<Area*>::const_iterator it = heatSources.begin();
//                    QList<QList<double> *>::const_iterator it2 = heatSourceIndices.begin();
//                    for(; it != heatSources.end(); ++it,++it2)
//                    {
//                        QList<double>::const_iterator it3 = (*it2)->begin();
//                        for(; it3 != (*it2)->end(); ++it3)
//                        {
//                            long pos = (*it3);
//                            (*(heatSourcesGrid[k]))[pos] = (*it)->getValue(); //(*it)->getValue(currentT,(pos % n) * deltaX,((pos - (pos % n)) / n) + deltaX);
//                        }
//                    }
//                }
//            }
//        }
//        emit finishedStep(i);
//        message = QString::number(i) + ". Zeitschritt beendet\nBenötigte Iterationen des Lösers: "
//                + QString::number(selectedIntMethod->getIterativeSolver()->getItCount()) + "\n";
//        emit simulationLogUpdate(message);
//    }

//    resultM = m;
//    resultN = n;
//    resultT = T;

//    message = "\nBerechnungen beendet\n\nAufräumen des Hauptspeichers\n\n"; //\nErgebnis:" + printResult()
//    emit simulationLogUpdate(message);

//    QList<QList<double> *>::iterator it = heatSourceIndices.begin();
//    for(; it != heatSourceIndices.end(); ++it)
//        delete (*it);

//    while(heatSourcesGrid.size() > 0)
//        delete heatSourcesGrid.takeFirst();

//    delete step1;
//    delete step2;

//    emit simulationLogUpdate("Simulation abgeschlossen\n\n");
//    simulated = true;
//    simulating = false;
//    ui->updateNotification();
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

//QString model::Model::printResult()
//{
//    QString output;
//    output += "\nAusgabe der Ergebnis-Matrix:\nIntMethode: "
//            + intMethods.key(selectedIntMethod) + "\nLöser: "
//            + selectedIterativeSolver + "\n\n";
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

//QString model::Model::printVector(const QVector<double> &vec)
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
