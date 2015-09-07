#ifndef CRS_H
#define CRS_H

#include <QVector>

namespace algorithms {

    class CRS
    {
    //Funktionen:
    public:
        CRS();
        CRS operator-(CRS const &rMat);
        CRS& operator=(CRS const &rhs);
        void A1(int const n); // 2D-Diskretisierungsmatrix Laplace-Operator
        void eye(int const n); // Einheitsmatrix der Groesse n*n

    //Attribute:
    private:
        explicit CRS(CRS const &rhs);
        QVector<int> index;
        QVector<int> ptr;
        QVector<double> value;

    friend CRS operator*(double const &scalar, CRS const &Mat);

    };

    CRS operator*(double const &scalar, CRS const &Mat);
}

#endif // CRS_H
