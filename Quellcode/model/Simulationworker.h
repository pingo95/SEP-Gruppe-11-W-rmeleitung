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
        double*** const & getResult() const;
        long getM() const;
        long getN() const;
        double getT() const;
        void initializeMaps();

    signals:
        void beginningSimulationStage(QString stage, int stepCount);
        void finishedSimulation();
        void finishedStep(int step);
        void startedSimulation();
        void simulationLogUpdate(QString message);

    public slots:
        void startSimulationSlot(SimulationSetup * simSetupTemplate);

//    private:
//        QString printVector(const QVector<double> &vec);
    // Attribute:
    private:
        bool busy;
        double * consecutiveTempArray;
        QMap<QString,algorithms::IntMethod*> intMethods;
        QMap<QString,algorithms::Solver*> solvers;
        long m;
        bool mapsInitialized;
        long n;
        double *** result;
        bool simulated;
        double T;
    };

}

#endif // SIMULATIONWORKER_H
