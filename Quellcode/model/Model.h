#ifndef MODEL_H
#define MODEL_H
#include <QString>
#include <QMap>
#include "Area.h"
#include "../algorithms/Intmethod.h"

namespace presentation{
    class UI;
}

namespace model {

    class Model
    {
    //Funktionen:
    public:
        Model();
        ~Model();

        void addHeatSource(Area * newHeatSource);
        void addThermalConductivity(Area * newThermalConductivity);
        double getBottomBoundary() const;
        Area * const & getHeatSource(int const id) const;
        QList<Area *> const& getHeatSources() const;
        int getHeatSourcesCount() const;
        double getInitialValue() const;
        QList<QString> const getIntMethodNames() const;
        QList<QString> const getIterativeSolverNames() const;
        double getLeftBoundary() const;
        int getM() const;
        int getN() const;
        double*** const & getResult() const;
        int getResultM() const;
        int getResultN() const;
        double getResultT() const;
        double getRightBoundary() const;
        QString getSelectedIntMethod() const;
        QString getSelectedIterativeSolver() const;
        bool getSimulated() const;
        bool getSimulating() const;
        double getT() const;
        QList<Area *> const & getThermalConductivities() const;
        int getThermalConductivitiesCount() const;
        Area * const & getThermalConductivity(int const id) const;
        double getTopBoundary() const;
        void removeLastHeatSource();
        void removeLastThermalConductivity();
        void selectIntMethod(QString const intMethod);
        void selectIterativeSolver(QString const newIterativeSolver);
        void setBottomBoundary(double const newBottomBoundary);
        void setInitialValue(double const newInitialValue);
        void setLeftBoundary(double const newLeftBoundary);
        void setM(int const newM);
        void setN(int const newN);
        void setRightBoundary(double const newRightBoundary);
        void setT(double const T);
        void setTopBoundary(double const topBoundary);
        void setUI(presentation::UI * ui);
        void simulate();
        void updateHeatSourceValue(int const pos, double const value);
        void updateThermalConductivityValue(int const pos, double const newValue);


    //Attribute:
    private:
        double bottomBoundary;
        QList<Area*> heatSources;
        int heatSourcesCount;
        double initialValue;
        QMap<QString,algorithms::IntMethod*> intMethods;
        QMap<QString,algorithms::IterativeSolver*> iterativeSolvers;
        double leftBoundary;
        long m;
        long n;
        double *** result;
        int resultM;
        int resultN;
        double resultT;
        double rightBoundary;
        algorithms::IntMethod * selectedIntMethod;
        QString selectedIterativeSolver;
        bool simulated;
        bool simulating;
        double T;
        QList<Area*> thermalConductivites;
        int thermalConductivitesCount;
        double topBoundary;
        presentation::UI * ui;

    };

}

#endif // MODEL_H
