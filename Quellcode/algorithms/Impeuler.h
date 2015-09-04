#ifndef IMPEULER_H
#define IMPEULER_H
#include "Intmethod.h"

namespace algorithms {

    class ImpEuler : public IntMethod
    {
    public:
        ImpEuler();

        void setUpItMatrix();
    };

}

#endif // IMPEULER_H
