#ifndef TESTERCRS_H
#define TESTERCRS_H

#include <QLabel>

#include "../algorithms/Crs.h"
#include "testwindow.h"

namespace algorithms {

    class TesterCRS
    {
        public:
            TesterCRS();

            QLabel* testA1();
            QLabel* testDiag();
            QLabel* testDiffCRS();
            QLabel* testEye();
            QLabel* testMultCRSCRS();
            QLabel* testMultCRSQVector();
            QLabel* testScalarCRS();
            QLabel* testScalarQVector();
            QLabel* testSumCRS();

        private:

            CRS testMatrix;
            QLabel* text;
    };
}

#endif // TESTERCRS_H
