#ifndef INTMETHOD_H
#define INTMETHOD_H
#include "Iterativesolver.h"

namespace algorithms {

    class IterativeSolver;

    class IntMethod
    {
    //Funktionen:
    public:
        IntMethod();

        void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector<double> const & heatSources);
        void selectIterativeSolver(IterativeSolver* iterativeSolver);
        void setUp(int const n, int const m, double const T, QVector<double> const &ThermalConductivities);
        virtual void setUpItMatrix()=0;

    //Attribute:
    private:
        IterativeSolver* activeIterativeSolver;
        double deltaX;
        double deltaT;
        CRS itMatrix;
        int n;
    };

}

#endif // INTMETHOD_H
