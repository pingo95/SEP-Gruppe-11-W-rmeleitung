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
        void startReadingData(QString const filename, long const obsCount);
        void startSimulationSlot(SimulationSetup * simSetupTemplate);

//    private:
//        QString printVector(const QVector<double> &vec);
    // Attribute:
    private:
        bool busy;
        double * consecutiveArrayObservations;
        double * consecutiveArraySimulation;
        bool dataRead;
        QMap<QString,algorithms::IntMethod*> intMethods;
        long m;
        bool mapsInitialized;
        long n;
        long obsDim;
        double ** observations;
        double *** result;
        bool simulated;
        QMap<QString,algorithms::Solver*> solvers;
        double T;

    };

}

#endif // SIMULATIONWORKER_H
