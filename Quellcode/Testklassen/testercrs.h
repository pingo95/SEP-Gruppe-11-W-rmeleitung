#ifndef TESTERCRS_H
#define TESTERCRS_H

#include <QLabel>

#include "Testwindow.h"

namespace algorithms {

    class TesterCRS
    {
        public:
            TesterCRS();
            ~TesterCRS();

            QLabel* testA1();
            QLabel* testDiag();
            QLabel* testDiffCRS();
            QLabel* testFull();
            QLabel* testEye();
            QLabel* testMultCRSCRS();
            QLabel* testMultCRSQVector();
            QLabel* testMultRowQVector();
            QLabel* testMultRowQVectorAbs();
            QLabel* testScalarCRS();
            QLabel* testScalarQVector();
            QLabel* testSumCRS();
            QLabel* testSumQVector();

        private:

            CRS<double> testMatrix;
    };
}

#endif // TESTERCRS_H
