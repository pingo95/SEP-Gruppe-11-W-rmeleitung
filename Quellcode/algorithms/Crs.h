#ifndef CRS_H
#define CRS_H

#include <QVector>

#include <cassert>
#include <cmath>

namespace algorithms {

    template <class T>
    class CRS
    {
    //Funktionen:
    public:
        CRS();
        CRS(const CRS<T> & rhs);
        CRS & operator=(CRS const & rhs);
        QVector<T> operator*(QVector<T> const & vec) const;
        CRS operator+(CRS const & rMat) const;
        CRS operator-(CRS const & rMat) const;

        void A1(int const n); // 2D-Diskretisierungsmatrix Laplace-Operator
        void diag(QVector<T> const & diag); // Diagonalmatrix mit Vektoreinträgen auf der Diagonalen
        void eye(int const n); // Einheitsmatrix der Größe n*n
        void full(QVector<QVector<T> > & full) const;
        int getIndex(int const i) const;
        int getSize() const;
        T getValue(int const i, int const j) const; // Liefert Eintrag a_ij
        CRS multCRSCRS(CRS const & rMat) const; // Implementiert lediglich Diagonalmatrix * bel. Matrix
        T multRowQVector(int const i, QVector<T> const & vec) const;
        T multRowQVectorAbs(int const i, QVector<T> const & vec) const;

    //Attribute:
    private:
        QVector<int> index;
        QVector<int> ptr;
        int size;
        QVector<double> value;

    friend CRS<T> operator*(T const & scalar, CRS<T> const & mat);

    friend QString printCRS(algorithms::CRS<T> const & matrix);
    friend class TesterIterativeSolver;

    };

    template <class T>
    CRS<T> operator *(T const & scalar, CRS<T> const & mat);

    template <class T>
    QVector<T> operator *(T const & scalar, QVector<T> const & rhs);

    template <class T>
    QVector<T> addQVectors(QVector<T> const & lhs, QVector<T> const & rhs);


}

#endif // CRS_H
