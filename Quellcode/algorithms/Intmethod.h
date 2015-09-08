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


        virtual void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector< QVector<double>* > const &heatSources)=0;
        virtual void getNeedetHeatSources(QVector<double> &neededTimeSteps, bool &reusable)=0;
        void selectIterativeSolver(IterativeSolver* iterativeSolver);
        void setUp(int const n, int const m, double const T, QVector<double> const &thermalConductivities);
        virtual void setUp(QVector<double> const &thermalConductivities)=0;

    //Attribute:
    protected:
        IterativeSolver* activeIterativeSolver;
        double deltaX;
        double deltaT;
        CRS itMatrix;
        int n;
    };

    QVector<double> operator *(double const &scalar, QVector<double> const &rhs);
}

#endif // INTMETHOD_H
