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
            BottomBoundary = 0,
            InitialValue = 1,
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
                        QVector<double> const & yKoords,
                        double value,
                        SimulationSetup::AreaType type);
        void deleteArea(int const pos, SimulationSetup::AreaType type);
        double getAreaBackgroundValue(SimulationSetup::AreaType type) const;
        QList<Area*> const & getAreas(SimulationSetup::AreaType type) const;
        int getAreaCount(SimulationSetup::AreaType type) const;
        int getContainingAreaID(double const xKoord, double const yKoord,
                                SimulationSetup::AreaType type);

        double getIBV(SimulationSetup::IBV ibv);
        long getM() const;
        long getN() const;
        QString getSelectedIntMethod() const;
        QString getSelectedSolver() const;
        double getSolverMaxError() const;
        int getSolverMaxIt() const;
        double getT() const;

        void removeLastArea(SimulationSetup::AreaType type);
        void reorderAreas(int const pos, int const dir, SimulationSetup::AreaType type);

        void selectIntMethod(QString const newIntMethod);
        void selectSolver(QString const newSolver);
        void setAreaBackground(double const newValue, SimulationSetup::AreaType type);
        void setIBV(double const newValue, SimulationSetup::IBV ibv);
        void setM(int const newM);
        void setN(int const newN);
        void setSolverMaxError(double const newMaxError);
        void setSolverMaxIt(double const newMaxIt);
        void setT(double const newT);


        void updateAreaValue(int const pos, double const newValue,
                             SimulationSetup::AreaType type);

    // Attribute:
    private:
        QList<Area*> heatSources;
        double heatSourcesBackgroundValue;
        int heatSourcesCount;
        double iBVs[5];
        long m;
        long n;
        QString selectedIntMethod;
        QString selectedSolver;
        double solverMaxError;
        int solverMaxIt;
        double T;
        QList<Area*> thermalDiffusivities;
        double thermalDiffusivitiesBackgroundValue;
        int thermalDiffusivitiesCount;

        friend QTextStream & operator<<(QTextStream & out, SimulationSetup & simSetup);
        friend QTextStream & operator>>(QTextStream & in, SimulationSetup & simSetup);
    };

    QTextStream & operator<<(QTextStream & out, SimulationSetup & simSetup);
    QTextStream & operator>>(QTextStream & in, SimulationSetup & simSetup);
}
Q_DECLARE_METATYPE(model::SimulationSetup *)
#endif // SIMULATIONSETUP_H
