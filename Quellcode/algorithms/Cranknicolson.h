#ifndef CRANKNICOLSON_H
#define CRANKNICOLSON_H
#include "Intmethod.h"

namespace algorithms {

    class CrankNicolson : public IntMethod
    {
    public:
        CrankNicolson();

        void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector<double> const &heatSources);
        void setUpItMatrix();
    };

}


#endif // CRANKNICOLSON_H
