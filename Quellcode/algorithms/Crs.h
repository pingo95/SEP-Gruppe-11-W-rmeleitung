#ifndef CRS_H
#define CRS_H

#include <QVector>
#include <cassert>
#include <iostream>
#include <fstream>

namespace algorithms {

    class TesterCRS;

    class CRS
    {
    //Funktionen:
    public:
        CRS();
        CRS(CRS const &rhs);
        CRS& operator=(CRS const &rhs);
        QVector<double> operator*(QVector<double> const &vec) const;
        CRS operator+(CRS const &rMat) const;
        CRS operator-(CRS const &rMat) const;

        void A1(int const n); // 2D-Diskretisierungsmatrix Laplace-Operator
        CRS& diag(QVector<double> const &diag);
        void eye(int const n); // Einheitsmatrix der Groesse n*n
        int getIndex(int i) const;
        int getRowsNumElem(int i) const;
        double getValue(int i, int j) const;

    //Attribute:
    private:
        QVector<int> index;
        QVector<int> ptr;
        QVector<double> value;
        int size;

    friend CRS operator*(double const &scalar, CRS const &Mat);
    friend CRS multCRSCRS(CRS const &lMat,CRS const &rMat);

    friend QString printCRS(algorithms::CRS const &matrix);

    };

    CRS operator *(double const &scalar, CRS const &Mat);
    CRS multCRSCRS(CRS const &lMat,CRS const &rMat);

    QVector<double> operator *(double const &scalar, QVector<double> const &rhs);
    QVector<double> addQVectors(QVector<double> const &lhs, QVector<double> const &rhs);


}

#endif // CRS_H
