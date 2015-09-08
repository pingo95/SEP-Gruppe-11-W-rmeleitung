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


        virtual void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector<double> const &heatSources) = 0;
        void selectIterativeSolver(IterativeSolver* iterativeSolver);
        virtual void setUp(int const n, int const m, double const T, QVector<double> const &thermalConductivities) = 0;

    //Attribute:
    protected:
        IterativeSolver* activeIterativeSolver;
        double deltaX;
        double deltaT;
        CRS itMatrix;
        int n;
    };

    QVector<double> operator*(double const &scalar, QVector<double> const & rhs);
}

#endif // INTMETHOD_H
