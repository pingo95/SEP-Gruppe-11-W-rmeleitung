#ifndef IMPEULER_H
#define IMPEULER_H
#include "Intmethod.h"

namespace algorithms {

    class ImpEuler : public IntMethod
    {
    public:
        ImpEuler();

        void calcNextStep(const QVector<double> &last, QVector<double> &next, const QVector<double> &heatSources);
        void setUp(const int n, const int m, const double T, const QVector<double> &thermalConductivities);
        void setUpItMatrix();
    };

}

#endif // IMPEULER_H
