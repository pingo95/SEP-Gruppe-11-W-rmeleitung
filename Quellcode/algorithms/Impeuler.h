#ifndef IMPEULER_H
#define IMPEULER_H
#include "Intmethod.h"

namespace algorithms {

    class ImpEuler : public IntMethod
    {
    public:
        ImpEuler();

        void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector< QVector<double>* > const &heatSources);
        void getNeedetHeatSources(QVector<double> &neededTimeSteps, bool &reusable);
        void setUp(QVector<double> const &thermalConductivities);
    };

}

#endif // IMPEULER_H
