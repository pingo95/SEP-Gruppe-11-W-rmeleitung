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

        void abortWork();

        void addNewArea(QVector<double> const & xKoords,
                        QVector<double> const & yKoords, double value,
                        SimulationSetup::AreaType type);
        void deleteArea(int const pos, SimulationSetup::AreaType type);

        bool getDataRead() const;
        QList<QString> const getIntMethodsNames() const;
        double** const & getObservations() const;
        int getObservationsDim() const;
        bool getOverrideThermalDiffusivities() const;
        double getOverrideValue() const;
        double*** const & getResult() const;
        long getResultM() const;
        long getResultN() const;
        double getResultT() const;

        bool getSimulated() const;
        SimulationSetup * const & getSimulationSetup() const;

        QList<QString> const getSolverNames() const;

        bool getUseHeatSources() const;

        bool isWorking() const;

        void loadSetup(QString filename);

        void optimize();

        void readObservations(QString const filename, long const obsCount);

        void removeLastArea(SimulationSetup::AreaType type);
        void reorderArea(int const pos, int const dir,
                         model::SimulationSetup::AreaType type);

        void resetSetup();

        void saveSetup(QString filename);

        void selectIntMethod(QString const newIntMethod);
        void selectSolver(QString const newSolver);
        void setAreaBackgroundValue(double const newValue, SimulationSetup::AreaType type);
        void setIBV(double const newValue, SimulationSetup::IBV ibv);
        void setM(int const newM);
        void setN(int const newN);
        void setOverrideThermalDiffusivities(bool const override);
        void setOverrideValue(double const newValue);
        void setSolverMaxError(double const newMaxError);
        void setSolverMaxIt(int const newMaxIt);
        void setT(double const newT);
        void setUI(presentation::UI * ui);
        void setUseHeatSources(bool const useHeatSources);

        void simulate();


        void updateAreaValue(int const pos, double const newValue,
                             SimulationSetup::AreaType type);

    signals:
        void startOptimization(SimulationSetup * simSetup, bool overrideTD,
                               double overrideValue, bool useHeatSources);
        void startReadingData(QString const filename, long const obsCount);
        void startSimulation(SimulationSetup * simSetup);

    private slots:
        void startedWorkSlot();
        void finishedOptimizationSlot(bool success);
        void finishedReadingDataSlot();
        void finishedSimulationSlot(bool success);

    //Attribute:
    private:
        bool blocking;
        bool dataRead;
        bool optimized;
        bool overrideDiffusivities;
        double overrideInitialTDvalue;
        SimulationSetup * simSetup;
        bool simulated;
        SimulationWorker * simWorker;
        presentation::UI * ui;
        bool useHeatSources;
        QThread workerThread;
        bool working;
    };

}

#endif // MODEL_H
