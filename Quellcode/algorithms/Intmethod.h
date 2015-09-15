#ifndef INTMETHOD_H
#define INTMETHOD_H

#include "Iterativesolver.h"
#include <QVector>

namespace algorithms {

    class IterativeSolver;

    class IntMethod
    {
    //Funktionen:
    public:
        IntMethod();
        virtual ~IntMethod();

        virtual void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector< QVector<double>* > const &heatSources) const=0;
        IterativeSolver * const & getIterativeSolver() const;
        virtual void getNeedetHeatSources(QVector<double> &neededTimeSteps, bool &reusable) const=0;
        void selectIterativeSolver(IterativeSolver* iterativeSolver);
        void setUp(int const n, int const m, double const T, QVector<double> const &thermalConductivities);
        virtual void setUpSpecific(QVector<double> const &thermalConductivities)=0;

    //Attribute:
    protected:
        IterativeSolver* activeIterativeSolver;
        double deltaX;
        double deltaT;
        CRS itMatrix;
        int n;
    };
}

#endif // INTMETHOD_H
