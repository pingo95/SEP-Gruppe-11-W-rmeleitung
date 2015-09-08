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
        CRS operator-(CRS const &rMat);

        void A1(int const n); // 2D-Diskretisierungsmatrix Laplace-Operator
        void diag(QVector<double> const &diag);
        void eye(int const n); // Einheitsmatrix der Groesse n*n

    //Attribute:
    private:
        QVector<int> index;
        QVector<int> ptr;
        QVector<double> value;
        int size;

    friend CRS operator*(double const &scalar, CRS const &Mat);

    };

    CRS operator*(double const &scalar, CRS const &Mat);
}

#endif // CRS_H
