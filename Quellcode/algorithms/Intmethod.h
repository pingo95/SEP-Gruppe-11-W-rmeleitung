#ifndef INTMETHOD_H
#define INTMETHOD_H

#include <QVector>

#include "Solver.h"

namespace algorithms {

    template <class T>
    class IntMethod {
    //Funktionen:
    public:
        IntMethod();
        virtual ~IntMethod();

        virtual void calcNextStep(QVector<T> const & last, QVector<T> & next, QVector< QVector<T>* > const & heatSources) const=0;
        virtual void getNeedetHeatSources(QVector<T> & neededTimeSteps, bool &reusable) const=0;
        Solver<T>* const & getSolver() const;
        void selectSolver(Solver<T>* solver);
        void setUp(int const n, int const m, T const Time, QVector<T> const & thermalDiffusivities);
        virtual void setUpSpecific(QVector<T> const & thermalDiffusivities)=0;

    //Attribute:
    protected:
        Solver<T>* activeSolver;
        T deltaT;
        T deltaX;
        CRS<T> itMatrix;
        int n;
    };

}

#endif // INTMETHOD_H
