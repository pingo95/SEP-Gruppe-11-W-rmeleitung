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
            AreaHeatSource,
            AreaThermalConductivity,
        };
    // Funktionen:
    public:
        explicit SimulationSetup();
        explicit SimulationSetup(SimulationSetup const & rhs);
        void addNewArea(QVector<double> const & xKoords,
                        QVector<double> const & yKoords, double value,
                        SimulationSetup::AreaType type);
        Area * const & getArea(int const id, SimulationSetup::AreaType type) const;
        double getAreaBackgroundValue(SimulationSetup::AreaType type) const;
        QList<Area*> const & getAreas(SimulationSetup::AreaType type) const;
        int getAreaCount(SimulationSetup::AreaType type) const;

        double getBoundaryBottom() const;
        double getBoundaryLeft() const;
        double getBoundaryRight() const;
        double getBoundaryTop() const;
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
        void setBoundaryBottom(double const newBottomBoundary);
        void setBoundaryTop(double const newTopBoundary);
        void setBoundaryLeft(double const newLeftBoundary);
        void setBoundaryRight(double const newRightBoundary);
        void setInitialValue(double const newInitialValue);
        void setM(int const newM);
        void setN(int const newN);
        void setSolverMaxError(double const maxError);
        void setSolverMaxIt(double const maxIt);
        void setT(double const T);

        void updateAreaValue(int const pos, double const value, SimulationSetup::AreaType type);

    // Attribute:
    private:
        QVector<double> boundaries;
        QList<Area*> heatSources;
        double heatSourcesBackgroundValue;
        int heatSourcesCount;
        double initialValue;
        long m;
        long n;
        QString selectedIntMethod;
        QString selectedSolver;
        double solverMaxError;
        int solverMaxIt;
        double T;
        QList<Area*> thermalConductivities;
        double thermalConductivitiesBackgroundValue;
        int thermalConductivitiesCount;
    };

}
Q_DECLARE_METATYPE(model::SimulationSetup)
#endif // SIMULATIONSETUP_H
