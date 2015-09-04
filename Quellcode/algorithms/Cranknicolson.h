#ifndef CRANKNICOLSON_H
#define CRANKNICOLSON_H
#include "Intmethod.h"

namespace algorithms {

    class CrankNicolson : public IntMethod
    {
    public:
        CrankNicolson();

        void setUpItMatrix();
    };

}


#endif // CRANKNICOLSON_H
