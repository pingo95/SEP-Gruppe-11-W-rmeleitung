#ifndef IMPEULER_H
#define IMPEULER_H
#include "Intmethod.h"

namespace algorithms {

    class ImpEuler : public IntMethod
    {
    public:
        ImpEuler();

        void calcNextStep(const QVector<double> &last, QVector<double> &next, const QVector<double> &heatSources);
        void setUpItMatrix();
    };

}

#endif // IMPEULER_H
