#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QObject>
#include <QMap>
#include <QString>
#include "Area.h"
#include "../algorithms/Intmethod.h"

namespace model {

    class SimulationWorker : public QObject
    {
        Q_OBJECT


    // Funktionen:
    public:
        explicit SimulationWorker(QObject *parent = 0);
        ~SimulationWorker();

        QList<QString> const getIntMethodNames() const;
        QList<QString> const getIterativeSolverNames() const;
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
        void startSimulationSlot(double const boundaryBottom,
                                 double const boundaryLeft,
                                 double const boundaryRight,
                                 double const boundaryTop,
                                 QList<model::Area*> const & heatSourcesTemplate,
                                 double const heatSourceBackgroundValue,
                                 int const heatSourcesCount,
                                 double const initialValue,
                                 QString const intMethod,
                                 QString const iterativeSolver,
                                 long const m, long const n,
                                 double const solverMaxError,
                                 int const solverMaxIt,
                                 double const T,
                                 QList<model::Area*> const & thermalConductivitiesTemplate,
                                 double const thermalConductivitiesBackgroundValue,
                                 int const thermalConductivitiesCount);

//    private:
//        QString printResult();
//        QString printVector(QVector<double> const & vec);

    // Attribute:
    private:
        bool busy;
        double * consecutiveTempArray;
        QMap<QString,algorithms::IntMethod*> intMethods;
        QMap<QString,algorithms::IterativeSolver*> iterativeSolvers;
        long m;
        bool mapsInitialized;
        long n;
        double *** result;
        bool simulated;
        long T;
    };

}

#endif // SIMULATIONWORKER_H
