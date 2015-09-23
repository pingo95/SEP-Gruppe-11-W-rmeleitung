#ifndef CRANKNICOLSON_H
#define CRANKNICOLSON_H

#include "Intmethod.h"

namespace algorithms {

    template <class T>
    class CrankNicolson : public IntMethod<T> {
    public:
        CrankNicolson();

        void calcNextStep(QVector<T> const & last, QVector<T> & next, QVector<QVector<T>* > const & heatSources) const;
        void getNeedetHeatSources(QVector<T> & neededTimeSteps, bool & reusable) const;
        void setUpSpecific(QVector<T> const & thermalDiffusivities);

    private:
        CRS<T> rhsMatrix;
    };

}


#endif // CRANKNICOLSON_H
