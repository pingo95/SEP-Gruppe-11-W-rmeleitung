#ifndef MODEL_H
#define MODEL_H
#include <QWidget>
#include <QString>
#include <QMap>
#include "Area.h"
#include "../algorithms/Intmethod.h"

namespace presentation{
    class UI;
}

namespace model {

    class Model : public QWidget
    {
        Q_OBJECT

    // Enum:
    public:
        enum AreaType
        {
            AreaHeatSource,
            AreaThermalConductivity,
        };

    //Funktionen:
    public:
        Model();
        ~Model();

        void addNewArea(QVector<double> const & xKoords,
                        QVector<double> const & yKoords, double value,
                        Model::AreaType type);
        Area * const & getArea(int const id, Model::AreaType type) const;
        QList<Area*> const & getAreas(Model::AreaType type) const;
        int getAreaCount(Model::AreaType type) const;

        double getBoundaryBottom() const;
        double getBoundaryLeft() const;
        double getBoundaryRight() const;
        double getBoundaryTop() const;
        double getInitialValue() const;
        QList<QString> const getIntMethodNames() const;
        QList<QString> const getIterativeSolverNames() const;
        long getM() const;
        long getN() const;
        double*** const & getResult() const;
        long getResultM() const;
        long getResultN() const;
        double getResultT() const;
        QString getSelectedIntMethod() const;
        QString getSelectedIterativeSolver() const;
        bool getSimulated() const;
        bool getSimulating() const;
        double getSolverMaxError() const;
        int getSolverMaxIt() const;
        double getT() const;

        void removeLastArea(Model::AreaType type);

        void selectIntMethod(QString const intMethod);
        void selectIterativeSolver(QString const newIterativeSolver);
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
        void setUI(presentation::UI * ui);

        void simulate();

        void updateAreaValue(int const pos, double const value, Model::AreaType type);

    signals:
        void beginningStage(QString stage, int stepCount);
        void finishedStep(int step);
        void simulationUpdate(QString message);

    private:
        QString printResult();
        QString printVector(QVector<double> const & vec);

    //Attribute:
    private:
        double boundaryBottom;
        double boundaryLeft;
        double boundaryRight;
        double boundaryTop;
        QList<Area*> heatSources;
        int heatSourcesCount;
        double initialValue;
        QMap<QString,algorithms::IntMethod*> intMethods;
        QMap<QString,algorithms::IterativeSolver*> iterativeSolvers;
        long m;
        long n;
        algorithms::IntMethod * selectedIntMethod;
        QString selectedIterativeSolver;
        bool simulated;
        double T;
        QList<Area*> thermalConductivites;
        int thermalConductivitesCount;
        presentation::UI * ui;

        double * consecutiveTempArray;
        double *** result;
        long resultM;
        long resultN;
        double resultT;

        bool simulating;


    };

}

#endif // MODEL_H
