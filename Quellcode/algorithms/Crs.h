#ifndef CRS_H
#define CRS_H

#include <QVector>
#include <cassert>

namespace algorithms {

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
        void diag(QVector<double> const &diag); // Diagonalmatrix mit Vektoreinträgen auf der Diagonalen
        void full(QVector<QVector<double> > & full) const;
        void eye(int const n); // Einheitsmatrix der Größe n*n
        int getIndex(int const i) const;
        int getRowsNumElem(int const i) const;
        int getSize() const;
        double getValue(int const i, int const j) const; // Liefert Eintrag a_ij
        CRS multCRSCRS(CRS const &rMat) const; // Implementiert lediglich Diagonalmatrix * bel. Matrix
        double multRowQVector(int const i, QVector<double> const vec) const;

    //Attribute:
    private:
        QVector<int> index;
        QVector<int> ptr;
        int size;
        QVector<double> value;

    friend CRS operator*(double const &scalar, CRS const &Mat);

    friend QString printCRS(algorithms::CRS const &matrix);
    friend class TesterIterativeSolver;

    };

    CRS operator *(double const &scalar, CRS const &Mat);

    QVector<double> operator *(double const &scalar, QVector<double> const &rhs);
    QVector<double> addQVectors(QVector<double> const &lhs, QVector<double> const &rhs);


}

#endif // CRS_H
