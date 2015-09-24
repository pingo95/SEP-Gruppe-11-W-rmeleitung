#ifndef CRS_H
#define CRS_H

#include <QVector>

#include <cassert>
#include <cmath>

namespace algorithms {

    /*!
     *  \brief Die Klasse CRS implementiert eine Variante des CRS-Formats, die nur die für diese Anwendung benötigten Funktionalitäten enthält
     *
     *  Diese sind:
     *      - Konstruktor, Copy-Konstruktor und Zuweisungsoperator
     *      - Matrix-Vektor-Produkt, Matrizensumme/-differenz, Diagonalmatrix-Matrix-Produkt
     *      - Aufstellen der Diskretisierungsmatrix (A1), einer Diagonalmatrix, der Einheitsmatrix
     *      - Generierung einer vollbesetzten Matrix
     *      - get-Funktionen für die Größe der Matrix sowie des Eintrages a_ij
     *      - Skalarprodukt einer Matrixzeile mit einem Vektor
     */
    template <class T>
    class CRS
    {
    //Funktionen:
    public:

        /*!
         * \brief Standardkonstruktor
         */
        CRS();

        /*!
         * \brief Copy-Konstruktor
         * \param rhs ist die Matrix, die kopiert werden soll
         */
        CRS(const CRS<T> & rhs);

        /*!
         * \brief Zuweisungsoperator
         * \param rhs entspricht der gleichzusetzenden Matrix
         * \return Referenz auf die Matrix rechts des "="-Zeichen
         */
        CRS & operator=(CRS const & rhs);

        /*!
         * \brief Matrix-Vektor-Produkt
         * \param vec
         * \return Ergebnis des Produktes
         */
        QVector<T> operator*(QVector<T> const & vec) const;

        /*!
         * \brief Matrixsummationsoperation
         * \param rMat
         * \return Summe der beiden Matrizen
         */
        CRS operator+(CRS const & rMat) const;

        /*!
         * \brief Matrixdifferenzoperation
         * \param rMat
         * \return Differenz der beiden Matrizen
         */
        CRS operator-(CRS const & rMat) const;

        /*!
         * \brief A1 liefert Matrix, die bei der Diskretisierung des Laplace-Operators mit zentralen Differenzen zweiter Ordnung entsteht
         * \param n ist die Stützstellenanzahl innerhalb einer Dimension der äquidistanten Ortsdiskretisierung
         */
        void A1(int const n); // 2D-Diskretisierungsmatrix Laplace-Operator

        /*!
         * \brief diag erzeugt eine Diagonalmatrix, die die Vektoreinträge auf der Diagonalen enthält
         * \param diag enthählt die gewünschten Einträge der Diagonalen
         */
        void diag(QVector<T> const & diag); // Diagonalmatrix mit Vektoreinträgen auf der Diagonalen

        /*!
         * \brief eye liefert die Einheitsmatrix der Größe (n*n)x(n*n)
         * \param n ist die Stützstellenanzahl innerhalb einer Dimension der äquidistanten Ortsdiskretisierung
         */
        void eye(int const n); // Einheitsmatrix der Größe n*n

        /*!
         * \brief full erzeugt eine dichtbesetzte Matrix, hierbei geht der Speichervorteil des CRS-Formats verloren
         * \param full ist die CRS Matrix, die dichtbesetzt gespeichert werden soll
         */
        void full(QVector<QVector<T> > & full) const;

        /*!
         * \brief get-Funktion der Größe
         * \return Größe der gespeicherten Matrix
         */
        int getSize() const;

        /*!
         * \brief get-Funtkion des Wertes eines Matrix-Eintrags
         * \param i enthält die Zeile
         * \param j enthält die Spalte
         * \return Wert des Matrixeintrags a_ij
         */
        T getValue(int const i, int const j) const; // Liefert Eintrag a_ij

        /*!
         * \brief multCRSCRS implementiert Diagonalmatrix-Matrix-Produkt
         * \param rMat ist eine beliebige CRS-Matrix
         * \return resultierende Produktmatrix
         */
        CRS multCRSCRS(CRS const & rMat) const; // Implementiert lediglich Diagonalmatrix * bel. Matrix

        /*!
         * \brief multRowQVector implementiert das Skalarprodukt einer Matrixzeile mit einem beliebigen Vektor
         * \param i gibt die Matrixzeile an
         * \param vec stellt den Vektor dar
         * \return Ergebnis des Skalarproduktes des i. Zeilenvektors mit dem übergebenen Vektor
         */
        T multRowQVector(int const i, QVector<T> const & vec) const;

        /*!
         * \brief multRowQVectorAbs berechnet die Summe der Beträge einer elementweisen Multiplikation der Vektoreinträge
         * \param i gibt die Matrixzeile an
         * \param vec stellt den Vektor dar
         * \return Ergebnis der Summe der Beträge der elementweisen Vektor-Vektor-Multiplikation
         */
        T multRowQVectorAbs(int const i, QVector<T> const & vec) const;

    //Attribute:
    private:
        QVector<int> index;
        QVector<int> ptr;
        int size;
        QVector<double> value;

     /*!
      *  \brief Dieser Operator wird als non-member-Function implementiert und mit "friend-Zugriffsrechten" versehen",
      *  \brief da er eigentlich zur Implementierung des Skalardatentyps gehört
      */
    template <class U>
    friend CRS<U> operator*(U const & scalar, CRS<U> const & mat);

    /*!
     * \brief Zugriffsrechte für Testfunktion
     */
    template <class U>
    friend QString printCRS(algorithms::CRS<U> const & matrix);


    /*!
     * \brief Zugriffsrechte für Testklasse
     */
    friend class TesterIterativeSolver;

    };

    /*!
     * \brief Skalar-CRS-Matrix-Produkt
     * \param scalar
     * \param mat ist die zu multiplizierende CRS-Matrix
     * \return Ergebnismatrix des Skalar-CRS-Matrix-Produkts
     */
    template <class T>
    CRS<T> operator *(T const & scalar, CRS<T> const & mat);

    /*!
     * \brief Skalar-QVector-Produkt
     * \param scalar
     * \param vec ist der zu multiplizierende QVector
     * \return Ergebnis der Skalar-QVector-Multiplikation
     *
     * Dieser Operator wurde als non-member-Function implementiert, da er eigentlich zur Implementierung
     * des Skalardatentyps gehört
     */
    template <class T>
    QVector<T> operator *(T const & scalar, QVector<T> const & vec);

    /*!
     * \brief QVector-Summation
     * \param lhs entspricht dem 1. Summanden
     * \param rhs entspricht dem 2. Summanden
     * \return Summe der beiden QVectoren
     *
     * Dieser Operator wurde als non-member-Function implementiert, da er eigentlich zur Implementierung
     * des QVectors gehört
     */
    template <class T>
    QVector<T> addQVectors(QVector<T> const & lhs, QVector<T> const & rhs);


}

#endif // CRS_H
