#ifndef CRS_H
#define CRS_H

#include <QVector>
#include <assert.h>

namespace algorithms {

    class CRS
    {
    //Funktionen:
    public:
        CRS();
        CRS(CRS const &rhs);
        CRS& operator=(CRS const &rhs);
        CRS operator*(CRS const &rMat);
        QVector<double> operator*(QVector<double> const &vec) const;
        CRS operator+(CRS const &rMat);
        CRS operator-(CRS const &rMat);

        void A1(int const n); // 2D-Diskretisierungsmatrix Laplace-Operator
        CRS& diag(QVector<double> const &diag);
        void eye(int const n); // Einheitsmatrix der Groesse n*n
        int getRowsMinCol(int i) const;
        double getValue(int i, int j) const;

    //Attribute:
    private:
        QVector<int> index;
        QVector<int> ptr;
        QVector<double> value;
        int size;

    friend CRS operator*(double const &scalar, CRS const &Mat);

    };


    QVector<double> operator *(double const &scalar, QVector<double> const &rhs);
    CRS operator *(double const &scalar, CRS const &Mat);

}

#endif // CRS_H
