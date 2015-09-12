#include "Testercrs.h"

algorithms::TesterCRS::TesterCRS() {
    text = new QLabel();
}
algorithms::TesterCRS::~TesterCRS()
{
//    delete text;
}

QLabel* algorithms::TesterCRS::testA1() {
    QString testString;
    int testSize=4;

    testMatrix.A1(testSize);

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testDiag() {
    QString testString;
    int testSize=4;

    QVector<double> testVec(testSize*testSize);
    for(int i=0;i<testSize*testSize;++i) testVec[i]=(i+1)*(i+1);

    testMatrix.diag(testVec);

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testDiffCRS() {
    QString testString;
    int testSize=3;

    algorithms::CRS mat1, mat2;
    mat1.A1(testSize); mat2.eye(testSize);

    testMatrix = mat1 - mat2;

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testEye() {
    QString testString;
    int testSize=3;

    testMatrix.eye(testSize);

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testMultCRSCRS() {
    QString testString;
    int testSize=3;

    QVector<double> testVec(testSize*testSize);
    for(int i=0;i<testSize*testSize;++i) testVec[i]=(i+1)*(i+1);
    algorithms::CRS mat1, mat2;
    mat1.diag(testVec); mat2.A1(testSize);

    testMatrix = mat1.multCRSCRS(mat2);

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testMultCRSQVector() {
    QString testString;
    int testSize=3;

    QVector<double> testVec(testSize*testSize);
    for(int i=0;i<testSize*testSize;++i) testVec[i]=(i+1)*(i+1);

    testMatrix.A1(testSize);
    testVec = testMatrix * testVec;

    testString = printQVector(testVec);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testScalarCRS() {
    QString testString;
    int testSize=3;

    testMatrix.eye(testSize);
    testMatrix = 3 * testMatrix;
    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testScalarQVector() {
    QString testString;
    int testSize=3;

    QVector<double> testVec(testSize*testSize);
    for(int i=0;i<testSize*testSize;++i) testVec[i]=(i+1)*(i+1);

    testVec = 3 * testVec;

    testString = printQVector(testVec);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testSumCRS() {
    QString testString;
    int testSize=3;

    algorithms::CRS mat1, mat2;
    mat1.A1(testSize); mat2.eye(testSize);

    testMatrix = mat1 + mat2;

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testSumQVector() {
    QString testString;
    int testSize=3;

    QVector<double> vec1(testSize), vec2(testSize);
    for(int i=0; i<vec1.size(); ++i) {
        vec1[i] = i;
        vec2[i] = 5;
    }
    vec1 = algorithms::addQVectors(vec1,vec2);

    testString = printQVector(vec1);
    text->setText(testString);
    return text;
}
