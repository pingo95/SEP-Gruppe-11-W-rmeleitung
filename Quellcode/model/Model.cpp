#include "Model.h"
#include "../algorithms/Impeuler.h"
#include "../algorithms/Cranknicolson.h"
#include "../algorithms/Jacobi.h"
#include "../algorithms/Gaussseidel.h"
#include "../presentation/Ui.h"

model::Model::Model() : bottomBoundary(300), heatSourcesCount(0), initialValue(300),
    leftBoundary(300), m(1), n(1), result(NULL), resultM(0), resultN(0), resultT(0.),
    rightBoundary(300), selectedIntMethod(NULL), selectedIterativeSolver(""),
    simulated(false), simulating(false), T(1.), thermalConductivitesCount(0),
    topBoundary(300), ui(NULL)
{
    // Registrieren der Integrationsmethoden
    intMethods.insert("Impliziter Euler",new algorithms::ImpEuler());
    intMethods.insert("Crank Nicolson", new algorithms::CrankNicolson());

    // Registrieren der iterativen Löser
    iterativeSolvers.insert("Jacobi", new algorithms::Jacobi());
    iterativeSolvers.insert("Gauss-Seidel", new algorithms::GaussSeidel());

    // Setzen der intialen Integrationsmethode und Lösers
    selectedIntMethod = intMethods["Impliziter Euler"];
    selectedIntMethod->selectIterativeSolver(iterativeSolvers["Jacobi"]);
}

model::Model::~Model()
{
    QList<Area*>::iterator it = heatSources.begin();
    for(; it != heatSources.end(); ++it)
        delete (*it);
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
        for(int j = 0; j < m; ++j)
        {
            for(int i = 0; i < n; ++n)
                delete result[j][i];
            delete result[j];
        }
        delete result;
    }
}

// Vorbedingung: Das übergebene Gebiet wurde vorher mit Area::validateArea auf
// Gültigkeit überprüft
// Diese Funktion fügt dem Modell ein neues Wärmequellen-Gebiet hinzu
void model::Model::addHeatSource(Area *newHeatSource)
{
    heatSources.append(newHeatSource);
    ++heatSourcesCount;
    ui->updateNotification();
}

// Vorbedingung: Das übergebene Gebiet wurde vorher mit Area::validateArea auf
// Gültigkeit überprüft
// Diese Funktion fügt dem Modell ein neues Wärmeleitkoeffizienten-Gebiet hinzu
void model::Model::addThermalConductivity(Area *newThermalConductivity)
{
    thermalConductivites.append(newThermalConductivity);
    ++thermalConductivitesCount;
    ui->updateNotification();
}


double model::Model::getBottomBoundary() const
{
    return bottomBoundary;
}

// Vorbedingung: ID ist die gültige ID eines Gebietes im Modell
model::Area* const & model::Model::getHeatSource(int const id) const
{
    QList<Area*>::const_iterator it = heatSources.begin();
    for(; it != heatSources.end(); ++it)
        if((*it)->getID() == id)
            return (*it);
    return NULL;
}

QList<model::Area*> const & model::Model::getHeatSources() const
{
    return heatSources;
}

int model::Model::getHeatSourcesCount() const
{
    return heatSourcesCount;
}

double model::Model::getInitialValue() const
{
    return initialValue;
}

QList<QString> const model::Model::getIntMethodNames() const
{
    return intMethods.keys();
}

QList<QString> const model::Model::getIterativeSolverNames() const
{
    return iterativeSolvers.keys();
}

double model::Model::getLeftBoundary() const
{
    return leftBoundary;
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
    return result;
}

long model::Model::getResultM() const
{
    return resultM;
}

long model::Model::getResultN() const
{
    return resultN;
}

double model::Model::getResultT() const
{
    return resultT;
}

double model::Model::getRightBoundary() const
{
    return rightBoundary;
}

QString model::Model::getSelectedIntMethod() const
{
    return intMethods.key(selectedIntMethod);
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

double model::Model::getT() const
{
    return T;
}

model::Area * const & model::Model::getThermalConductivity(int const id) const
{
    QList<Area*>::const_iterator it = thermalConductivites.begin();
    for(; it != thermalConductivites.end(); ++it)
        if((*it)->getID() == id)
            return (*it);
    return NULL;
}

QList<model::Area *> const & model::Model::getThermalConductivities() const
{
    return thermalConductivites;
}

int model::Model::getThermalConductivitiesCount() const
{
    return thermalConductivitesCount;
}

double model::Model::getTopBoundary() const
{
    return topBoundary;
}

// Entfernt das zuletzt hinzugefügte Wärmequellen-Gebiet, falls
// Modell mindestens eins enthält
void model::Model::removeLastHeatSource()
{
    if(heatSourcesCount > 0)
    {
        delete heatSources.takeLast();
        --heatSourcesCount;
        ui->updateNotification();
    }
}

// Entfernt das zuletzt hinzugefügte Wärmeleitkoeffizienten-Gebiet, falls
// Modell mindestens eins enthält
void model::Model::removeLastThermalConductivity()
{
    if(thermalConductivitesCount > 0 )
    {
        delete thermalConductivites.takeLast();
        --thermalConductivitesCount;
        ui->updateNotification();
    }
}

// Updatet die gewählte Integrationsmethode
void model::Model::selectIntMethod(QString intMethod)
{
    selectedIntMethod = intMethods[intMethod];
    selectedIntMethod->selectIterativeSolver(iterativeSolvers[selectedIterativeSolver]);
    ui->updateNotification();
}

// Updatet den gewählten iterativen Löser
void model::Model::selectIterativeSolver(QString newIterativeSolver)
{
    selectedIterativeSolver = newIterativeSolver;
    selectedIntMethod->selectIterativeSolver(iterativeSolvers[selectedIterativeSolver]);
    ui->updateNotification();
}

void model::Model::setBottomBoundary(double const newBottomBoundary)
{
    bottomBoundary = newBottomBoundary;
    ui->updateNotification();
}

void model::Model::setInitialValue(double const newInitialValue)
{
    initialValue = newInitialValue;
    ui->updateNotification();
}

void model::Model::setLeftBoundary(double const newLeftBoundary)
{
    leftBoundary = newLeftBoundary;
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

void model::Model::setRightBoundary(double const newRightBoundary)
{
    rightBoundary = newRightBoundary;
    ui->updateNotification();
}

void model::Model::setT(double const newT)
{
    T = newT;
    ui->updateNotification();
}

void model::Model::setTopBoundary(double const newTopBoundary)
{
    topBoundary = newTopBoundary;
    ui->updateNotification();
}

void model::Model::setUI(presentation::UI *ui)
{
    this->ui = ui;
}

void model::Model::simulate()
{
    simulating = true;

    // Deltax
    double deltaX = (double) 1 / (double) n;
//    double deltaT = (double) 1 / (double) m;

    // konsekutives Anlegen
    double * consecutiveTempArray = new double [m*n*n];
    // umwandeln in die Ergebnismatrix
    result = new double**[m];
    for(long i = 0; i < m; ++i)
    {
        result[i] = new double *[n];

        // Erste Zeilen mit unterem Randwert
        result[i][0] = &(consecutiveTempArray[i*n*n]);
        for(long k = 0; k < n; ++k)
            result[i][0][k] = bottomBoundary;

        for(long j = 1; j < n-1; ++j)
        {
            result[i][j] = &(consecutiveTempArray[i*n*n + j*n]);

            // Erste und letzter Punkt mit linkem bzw rechtem Randwert
            result[i][j][0] = leftBoundary;
            result[i][j][n-1] = rightBoundary;
            // Inneren Punkt initialisiert mit  0
            for(long k = 1; k < n-1; ++k)
                result[i][j][k] = 0.;
        }

        // Letzte Zeilen mit oberem Randwert
        result[i][n-1] = &(consecutiveTempArray[i*n*n]);
        for(long k = 0; k < n; ++k)
            result[i][n-1][k] = topBoundary;
    }
    bool reusable;
    QVector<double> neededTimeSteps;

    selectedIntMethod->getNeedetHeatSources(neededTimeSteps, reusable); // timesteps in vielfachen von deltaT, "neuester" zuerst
    int neededTimeStepsCount = neededTimeSteps.size();

    // Berechnen welche Punkte von welcher Wärmequelle abgedeckt werden,
    // zwischenspeichern als Indizes
    QList<QList<double> *> heatSourceIndices;
    if(heatSourcesCount > 0)
    {
        QList<Area*>::const_iterator it = heatSources.begin();
        for(; it != heatSources.end(); ++it)
        {
            QList<double> * tmpListPtr = new QList<double>;
            Area* heatSource = *it;
            double xMin, xMax, yMin, yMax;
            heatSource->getTransitiveRectangle(xMin,xMax,yMin,yMax);
            long xLBound = ceil(xMin/deltaX),
                    xUBound = floor(xMax/deltaX),
                    yLBound = ceil(yMin/deltaX),
                    yUBound = floor(yMax/deltaX);
            for(long i = xLBound; i <= xUBound; ++i)
                for(long j = yLBound; j <= yUBound; ++j)
                    if(heatSource->insidePoint(i*deltaX,j*deltaX))
                        tmpListPtr->append(i+j*n);
            heatSourceIndices.append(tmpListPtr);
        }
    }

    // Anlegen der Vektoren für Wärmequellen und Wärmeleitkoeffizienten
    QVector<QVector<double> *> heatSourcesGrid(neededTimeStepsCount,NULL);
    QVector<double> thermalConductivitiesGrid(n*n,0);

    for(int i = 0; i < neededTimeStepsCount; ++i)
        heatSourcesGrid[i] = new QVector<double>(n*n,0);

    // Berechnen welche Punkte von welchem Wärmeleitkoeffizienten-Gebiet
    // abgedeckt werden, dabei überschreiben neure Gebiete ältere
    if(thermalConductivitesCount > 0)
    {
        QList<Area*>::const_iterator it = thermalConductivites.begin();
        for(; it != thermalConductivites.end(); ++it)
        {
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
        }
    }

    // Intialisieren der Int-Methode
    selectedIntMethod->setUp(m,n,T,thermalConductivitiesGrid);

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


    // Iterationsvektoren
    QVector<double> * step1 = new QVector<double>(n*n,0);
    QVector<double> * step2 = new QVector<double>(n*n,0);
    QVector<double> * swapTmp;
    for(long i = 0; i < n; ++i)
        for(long j = 0; j < n; ++j)
            (*step1)[i+j*n] = result[0][i][j];

    // Berechnen der Zeitschritte
    for(long i = 1; i < m; ++i)
    {
        selectedIntMethod->calcNextStep(*step1,*step2,heatSourcesGrid);
        for(long j = 0; j < n; ++j)
            for(long k = 0; k < n; ++k)
                result[i][j][k] = (*step2)[j+k*n];
        swapTmp = step1;
        step1 = step2;
        step2 = swapTmp;
        if(reusable)
        {
            // Wiederverwertbar -> ring swap rückwärts
            swapTmp = heatSourcesGrid[neededTimeStepsCount-1];
            for(int i = neededTimeStepsCount-2; i >= 0; --i)
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
    }

    resultM = m;
    resultN = n;
    resultT = T;

    QList<QList<double> *>::iterator it = heatSourceIndices.begin();
    for(; it != heatSourceIndices.end(); ++it)
        delete (*it);

    while(heatSourcesGrid.size() > 0)
        delete heatSourcesGrid.takeFirst();

    delete step1;
    delete step2;

    simulated = true;
    simulating = false;
    ui->updateNotification();
}

void model::Model::updateHeatSourceValue(int pos, double newValue)
{
    heatSources.at(pos)->setValue(newValue);
    ui->updateNotification();
}

void model::Model::updateThermalConductivityValue(int pos, double newValue)
{
    thermalConductivites.at(pos)->setValue(newValue);
    ui->updateNotification();
}
