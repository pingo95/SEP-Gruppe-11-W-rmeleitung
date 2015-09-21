#ifndef INTMETHOD_H
#define INTMETHOD_H

#include <QVector>

#include "Solver.h"

namespace algorithms {

    class Solver;

    class IntMethod {
    //Funktionen:
    public:
        IntMethod();
        virtual ~IntMethod();

        virtual void calcNextStep(QVector<double> const & last, QVector<double> & next, QVector< QVector<double>* > const & heatSources) const=0;
        virtual void getNeedetHeatSources(QVector<double> & neededTimeSteps, bool &reusable) const=0;
        Solver* const & getSolver() const;
        void selectSolver(Solver* solver);
        void setUp(int const n, int const m, double const T, QVector<double> const & thermalDiffusivities);
        virtual void setUpSpecific(QVector<double> const & thermalDiffusivities)=0;

    //Attribute:
    protected:
        Solver* activeSolver;
        double deltaT;
        double deltaX;
        CRS itMatrix;
        int n;
    };

}

#endif // INTMETHOD_H
