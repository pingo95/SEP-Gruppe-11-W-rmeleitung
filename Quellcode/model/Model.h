#ifndef MODEL_H
#define MODEL_H
#include <QString>
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

        void addHeatSource(Area * newHeatSource);
        void addThermalConductivity(Area * newThermalConductivity);
        double getBottomBoundary;
        QList<Area*> getHeatSources();
        int getHeatSourcesCount();
        double getInitialValue();
        QList<QString> getIntMethodNames();
        QList<QString> getIterativeSolverNames();
        double getLeftBoundary();
        int getM();
        int getN();
        double*** getResult();
        int getResultM();
        int getResultN();
        double getResultT();
        double getRightBoundary();
        QString getSelectedIntMethod();
        QString getSelectedIterativeSolver();
        bool getSimulated();
        bool getSimulating();
        double getT;
        QList<Area*> getThermalConductivities();
        int getThermalConductivitiesCount();
        double getTopBoundary();
        void removeLastHeatSource();
        void removeLastThermalConductivity();
        void selectIntMethod(QString intMethod);
        void selectIterativeSolver(QString iterativeSolver);
        void setBottomBoundary(double bottomBoundary);
        void setInitialValue(double initialValue);
        void setLeftBoundary(double leftBoundary);
        void setM(int m);
        void setN(int n);
        void setRightBoundary(double rightBoundary);
        void setT(double T);
        void setTopBoundary(double topBoundary);
        void setUI(presentation::UI * ui);
        void simulate();
        void updateHeatSourceValue(int pos, double value);
        void updateThermalConductivityValue(int pos, double value);

    //Attribute:
    private:
        double bottomBoundary;
        QList<Area*> heatSources;
        int heatSourcesCount;
        double initialValue;
        double leftBoundary;
        int m;
        int n;
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
