#ifndef SIMULATIONSETUP_H
#define SIMULATIONSETUP_H
#include <QList>
#include <QString>
#include "Area.h"
#include <QMetaType>

namespace model {

    class SimulationSetup
    {

    // Enum:
    public:
        enum AreaType
        {
            AreaHeatSource = 0,
            AreaThermalDiffusivity = 1,
        };
        enum IBV
        {
            InitialValue = 0,
            BottomBoundary = 1,
            LeftBoundary = 2,
            RightBoundary = 3,
            TopBoundary = 4
        };
        static double const AreaMaxValue[2];
        static double const AreaMinValue[2];

        static double const MaxTemperature;
        static double const MinTemperature;

    // Funktionen:
    public:
        explicit SimulationSetup();
        explicit SimulationSetup(SimulationSetup const & rhs);
        ~SimulationSetup();

        void addNewArea(QVector<double> const & xKoords,
                        QVector<double> const & yKoords, double value,
                        SimulationSetup::AreaType type);
        Area * const & getArea(int const id, SimulationSetup::AreaType type) const;
        double getAreaBackgroundValue(SimulationSetup::AreaType type) const;
        QList<Area*> const & getAreas(SimulationSetup::AreaType type) const;
        int getAreaCount(SimulationSetup::AreaType type) const;

        double getIBV(SimulationSetup::IBV ibv);
        double getInitialValue() const;
        long getM() const;
        long getN() const;
        QString getSelectedIntMethod() const;
        QString getSelectedSolver() const;
        double getSolverMaxError() const;
        int getSolverMaxIt() const;
        double getT() const;

        void removeLastArea(SimulationSetup::AreaType type);

        void selectIntMethod(QString const intMethod);
        void selectSolver(QString const newSolver);
        void setAreaBackground(double const newValue, SimulationSetup::AreaType type);
        void setIBV(double const newValue, SimulationSetup::IBV ibv);
        void setInitialValue(double const newInitialValue);
        void setM(int const newM);
        void setN(int const newN);
        void setSolverMaxError(double const maxError);
        void setSolverMaxIt(double const maxIt);
        void setT(double const T);

        void updateAreaValue(int const pos, double const value, SimulationSetup::AreaType type);

    // Attribute:
    private:
        double iBVs[5];
        QList<Area*> heatSources;
        double heatSourcesBackgroundValue;
        int heatSourcesCount;
        long m;
        long n;
        QString selectedIntMethod;
        QString selectedSolver;
        double solverMaxError;
        int solverMaxIt;
        double T;
        QList<Area*> thermalDiffusivities;
        double thermalDiffusivitiesBackgroundValue;
        int thermalDiffusiivitiesCount;
    };

}
Q_DECLARE_METATYPE(model::SimulationSetup *)
#endif // SIMULATIONSETUP_H
