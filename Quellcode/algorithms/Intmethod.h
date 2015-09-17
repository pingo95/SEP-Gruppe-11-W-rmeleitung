#ifndef INTMETHOD_H
#define INTMETHOD_H

#include "Solver.h"
#include <QVector>

namespace algorithms {

    class Solver;

    class IntMethod {
    //Funktionen:
    public:
        IntMethod();
        virtual ~IntMethod();

        virtual void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector< QVector<double>* > const &heatSources)=0;
        Solver * const & getSolver() const;
        virtual void getNeedetHeatSources(QVector<double> &neededTimeSteps, bool &reusable) const=0;
        void selectSolver(Solver* solver);
        void setUp(int const n, int const m, double const T, QVector<double> const &thermalConductivities);
        virtual void setUpSpecific(QVector<double> const &thermalConductivities)=0;

    //Attribute:
    protected:
        Solver* activeSolver;
        bool equilibrating;
        double deltaT;
        double deltaX;
        CRS itMatrix;
        int n;
    };

}

#endif // INTMETHOD_H
