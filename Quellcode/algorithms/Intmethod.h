#ifndef INTMETHOD_H
#define INTMETHOD_H
#include <QVector>
#include "Crs.h"
namespace algorithms {


    class IterativeSolver;

    class IntMethod
    {
        IterativeSolver* activeIterativeSolver;
        double deltaX;
        double deltaT;
        CRS itMatrix;
        int n;

    public:
        IntMethod();

        void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector<double> const & heatSources);
        void selectIterativeSolver(IterativeSolver* iterativeSolver);
        void setUp(int const n, int const m, double const T, QVector<double> const &ThermalConductivities);
        virtual void setUpItMatrix()=0;
    };

}

#endif // INTMETHOD_H
