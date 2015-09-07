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

        void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector<double> const &heatSources);
        void selectIterativeSolver(IterativeSolver* iterativeSolver);
        void setThermalConductivities(QVector<double> const &thermalConductivities);
        void setUp(int const n, int const m, double const T, QVector<double> const &thermalConductivities);
        virtual void setUpItMatrix()=0;

    //Attribute:
    protected:
        IterativeSolver* activeIterativeSolver;
        double deltaX;
        double deltaT;
        CRS itMatrix;
        int n;
        QVector<double> thermalConductivites;
    };

    QVector<double> operator*(double const &scalar, QVector<double> const & rhs);
}

#endif // INTMETHOD_H
