#ifndef CRANKNICOLSON_H
#define CRANKNICOLSON_H

#include "Intmethod.h"

namespace algorithms {

    class CrankNicolson : public IntMethod {
    public:
        CrankNicolson();

        void calcNextStep(QVector<double> const &last, QVector<double>& next, QVector< QVector<double>* > const &heatSources);
        void getNeedetHeatSources(QVector<double> &neededTimeSteps, bool &reusable) const;
        void setUpSpecific(QVector<double> const &thermalDiffusivities);

    private:
        CRS rhsMatrix;
    };

}


#endif // CRANKNICOLSON_H
