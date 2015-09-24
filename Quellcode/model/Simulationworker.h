#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QObject>
#include <QMap>
#include "../algorithms/Intmethod.h"
#include "Simulationsetup.h"

namespace model {

    class SimulationWorker : public QObject
    {
        Q_OBJECT


    // Funktionen:
    public:
        explicit SimulationWorker(QObject *parent = 0);
        ~SimulationWorker();

        QList<QString> const getIntMethodNames() const;
        QList<QString> const getSolverNames() const;
        long getM() const;
        long getN() const;
        double ** const &getObservations() const;
        int getObservationsDim() const;
        double*** const & getResult() const;
        double getT() const;
        void initializeMaps();

    signals:
        void beginningStage(QString stage, int stepCount, bool simulation = true);
        void finishedOptimization();
        void finishedReadingData();
        void finishedSimulation();
        void finishedStep(int step, bool simulation = true);
        void startedWork();
        void simulationLogUpdate(QString message);

    public slots:
        void startOptimization(SimulationSetup * simSetupTemplate, bool overrideTD,
                               double overrideValue, bool useHeatSources);
        void startReadingData(QString const filename, long const obsCount);
        void startSimulationSlot(SimulationSetup * simSetupTemplate);

    private:
        QVector<double> * & simpleSimulation(SimulationSetup & simSetup, QVector<double> * & step1,
                              QVector<double> * & step2, QVector<double> &currentCs,
                              QList<QList<long>*> heatSourceIndices);

    // Attribute:
    private:
        bool busy;
        double * consecutiveArrayObservations;
        double * consecutiveArraySimulation;
        bool dataRead;
        QMap<QString,algorithms::IntMethod<double>*> intMethods;
        long m;
        bool mapsInitialized;
        long n;
        long obsSize;
        double ** observations;
        double *** result;
        bool simulated;
        QMap<QString,algorithms::Solver<double>*> solvers;
        double T;

        bool optimized;
        long optimizationN;
        QVector<double> optimizedCs;
    };

}

#endif // SIMULATIONWORKER_H
