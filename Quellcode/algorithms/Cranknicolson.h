#ifndef CRANKNICOLSON_H
#define CRANKNICOLSON_H
#include "Intmethod.h"

namespace algorithms {

    class CrankNicolson : public IntMethod
    {
    public:
        CrankNicolson();

        void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector<double> const &heatSources);
        void setUp(int const n, int const m, double const T, QVector<double> const &thermalConductivities);
        void setUpItMatrix();
    };

}


#endif // CRANKNICOLSON_H
