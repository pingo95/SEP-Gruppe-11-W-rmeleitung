#ifndef IMPEULER_H
#define IMPEULER_H

#include "Intmethod.h"

namespace algorithms {

    template <class T>
    class ImpEuler : public IntMethod<T> {
    public:
        ImpEuler();

        void calcNextStep(QVector<T> const & last, QVector<T> & next, QVector< QVector<T>* > const & heatSources) const;
        void getNeedetHeatSources(QVector<T> & neededTimeSteps, bool & reusable) const;
        void setUpSpecific(QVector<T> const & thermalDiffusivities);
    };

}

#endif // IMPEULER_H
