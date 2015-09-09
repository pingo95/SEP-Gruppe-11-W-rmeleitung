#include "testercrs.h"

algorithms::TesterCRS::TesterCRS() {
    text = new QLabel();
}

QLabel* algorithms::TesterCRS::print() {
    int testSize=3;
    QString testString;

    QVector<double> testVec(testSize*testSize);
    for(int i=0;i<testSize*testSize;++i) testVec[i]=(i+1)*(i+1);
    QVector<double> res(testSize*testSize);

    testMatrix.A1(testSize);
    res = 2 * testVec;

    int kl, ku, il, iu;
    for(int i=0; i<testMatrix.size; ++i) {
        kl = testMatrix.getRowsMinCol(i);
        ku = testMatrix.getRowsMinCol(i+1);
        if(kl==ku) {
            for(int j=0; j<testMatrix.size; ++j) testString.append(QString().number(0)+"\t");
        }
        else {
            il = testMatrix.index[kl];
            iu = testMatrix.index[ku-1];
            for(int j=0; j<testMatrix.size; ++j) {
                if(j >= il && j <= iu) {
                    if(j==testMatrix.index[kl]) {
                        testString.append(QString().number(testMatrix.value[kl])+"\t");
                        ++kl;
                    }
                    else testString.append(QString().number(0)+"\t");
                }
                else testString.append(QString().number(0)+"\t");
            }
        }
        testString.append("\n");
    }
    text->setText(testString);
    return text;
}
