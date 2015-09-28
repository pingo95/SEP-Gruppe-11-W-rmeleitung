#include "Testercrs.h"
#include "Testwindow.cpp"

algorithms::TesterCRS::TesterCRS() {

}

algorithms::TesterCRS::~TesterCRS() {

}

QLabel* algorithms::TesterCRS::testA1() {
    QLabel* text = new QLabel();
    QString testString;
    int testSize=3;

    testMatrix.A1(testSize);

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testDiag() {
    QLabel* text = new QLabel();
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
    QLabel* text = new QLabel();
    QString testString;
    int testSize=3;

    algorithms::CRS<double> mat1, mat2;
    mat1.A1(testSize); mat2.eye(testSize);

    testMatrix = mat1 - mat2;

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testEye() {
    QLabel* text = new QLabel();
    QString testString;
    int testSize=3;

    testMatrix.eye(testSize);

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testFull() {
    QLabel* text = new QLabel();
    QString testString;
    int testSize=5;

    testMatrix.A1(testSize);
    QVector<QVector<double> > tmp;
    testMatrix.full(tmp);

    testString = printFull(tmp);
    text->setText(testString);
    return text;
}


QLabel* algorithms::TesterCRS::testMultCRSCRS() {
    QLabel* text = new QLabel();
    QString testString;
    int testSize=3;

    QVector<double> testVec(testSize*testSize);
    for(int i=0;i<testSize*testSize;++i) testVec[i]=(i+1)*(i+1);
    algorithms::CRS<double> mat1, mat2;
    mat1.diag(testVec); mat2.A1(testSize);

    testMatrix = mat1.multCRSCRS(mat2);

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testMultCRSQVector() {
    QLabel* text = new QLabel();
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

QLabel* algorithms::TesterCRS::testMultRowQVector() {
    QLabel* text = new QLabel();
    int testSize=3;

    QVector<double> testVec(testSize*testSize);
    for(int i=0;i<testSize*testSize;++i) testVec[i]=(i+1)*(i+1);

    testMatrix.A1(testSize);

    text->setText(QString::number(testMatrix.multRowQVector(4,testVec)));
    return text;
}

QLabel* algorithms::TesterCRS::testMultRowQVectorAbs() {
    QLabel* text = new QLabel();
    int testSize=3;

    QVector<double> testVec(testSize*testSize);
    for(int i=0;i<testSize*testSize;++i) testVec[i]=(i+1)*(i+1);

    testMatrix.A1(testSize);

    text->setText(QString::number(testMatrix.multRowQVectorAbs(4,testVec)));
    return text;
}

QLabel* algorithms::TesterCRS::testScalarCRS() {
    QLabel* text = new QLabel();
    QString testString;
    int testSize=3;

    testMatrix.eye(testSize);
    testMatrix = 3. * testMatrix;
    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testScalarQVector() {
    QLabel* text = new QLabel();
    QString testString;
    int testSize=3;

    QVector<double> testVec(testSize*testSize);
    for(int i=0;i<testSize*testSize;++i) testVec[i]=(i+1)*(i+1);

    testVec = 3. * testVec;

    testString = printQVector(testVec);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testSumCRS() {
    QLabel* text = new QLabel();
    QString testString;
    int testSize=3;

    algorithms::CRS<double> mat1, mat2;
    mat1.A1(testSize); mat2.eye(testSize);

    testMatrix = mat1 + mat2;

    testString = printCRS(testMatrix);
    text->setText(testString);
    return text;
}

QLabel* algorithms::TesterCRS::testSumQVector() {
    QLabel* text = new QLabel();
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
