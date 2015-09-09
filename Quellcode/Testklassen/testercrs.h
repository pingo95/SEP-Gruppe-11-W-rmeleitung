#ifndef TESTERCRS_H
#define TESTERCRS_H

#include <QLabel>

#include "../algorithms/Crs.h"

namespace algorithms {

    class TesterCRS
    {
        public:
            TesterCRS();

            QLabel* print();

        private:
            CRS testMatrix;
            QLabel* text;
    };
}

#endif // TESTERCRS_H
