#ifndef MODEL_H
#define MODEL_H
#include <QWidget>
#include <QThread>
#include "Simulationworker.h"

namespace presentation{
    class UI;
}

namespace model {

    class Model : public QWidget
    {
        Q_OBJECT

    // Funktionen:
    public:
        explicit Model();
        ~Model();

        void addNewArea(QVector<double> const & xKoords,
                        QVector<double> const & yKoords, double value,
                        SimulationSetup::AreaType type);
        void addNewArea(Area * area, SimulationSetup::AreaType type);
        void deleteArea(int const pos, SimulationSetup::AreaType type);

        bool getDataRead() const;
        double** const & getObservations() const;
        int getObservationsDim() const;
        double*** const & getResult() const;
        long getResultM() const;
        long getResultN() const;
        double getResultT() const;

        QList<QString> const getIntMethodsNames() const;
        QList<QString> const getSolverNames() const;

        bool getSimulated() const;
        bool isWorking() const;

        SimulationSetup * const & getSimulationSetup() const;

        void readObservations(QString const filename, long const obsCount);

        Area * removeLastArea(SimulationSetup::AreaType type);
        void reorderArea(int const pos, int const dir,
                         model::SimulationSetup::AreaType type);

        void selectIntMethod(QString const intMethod);
        void selectSolver(QString const newSolver);
        void setAreaBackground(double const newValue, SimulationSetup::AreaType type);
        void setIBV(double const newValue, SimulationSetup::IBV ibv);
        void setM(int const newM);
        void setN(int const newN);
        void setSolverMaxError(double const maxError);
        void setSolverMaxIt(double const maxIt);
        void setT(double const T);
        void setUI(presentation::UI * ui);

        void simulate();

        void updateAreaValue(int const pos, double const value, SimulationSetup::AreaType type);

    signals:
        void startReadingData(QString const filename, long const obsCount);
        void startSimulation(SimulationSetup * simSetup);

    private slots:
        void startedWorkSlot();
        void finishedOptimizationSlot();
        void finishedReadingDataSlot();
        void finishedSimulationSlot();

    //Attribute:
    private:
        bool blocking;
        bool dataRead;
        bool optimized;
        SimulationSetup * const simSetup;
        bool simulated;
        bool working;
        SimulationWorker * simWorker;
        presentation::UI * ui;
        QThread workerThread;
    };

}

#endif // MODEL_H
