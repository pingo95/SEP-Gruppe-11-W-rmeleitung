#ifndef GAUSS_H
#define GAUSS_H

#include <cassert>
#include "Solver.h"

namespace algorithms {

    /*!
     * \brief Die Klasse LU implementiert das gleichnamige direkte Lösungsverfahren von LGS-Systemen der Form Ax=b
     *
     * Sie implementiert eine LU-Zerlegung der Gleichungssystemmatrix A sowie die Lösungsfunktion
     */
    template <class T>
    class LU : public Solver<T> {
    //Funktionen:
    public:

        /*!
         * \brief Standardkonstruktor
         */
        LU();

        /*!
         * \brief decompose zerlegt die übergebene Matrix in eine untere Dreiecksmatrix L und eine obere Dreiecksmatrix U
         * \param matrix stellt die zu zerlegende Matrix da
         */
        void decompose(CRS<T> const & matrix);

        /*!
         * \brief solve implementiert eine direkte Lösung des linearen Gleichungssystems
         * \param result dient zur Speicherung der approximierten Lösung
         * \param matrix ist die Matrix A des Gleichungssystems
         * \param rhs ist die rechte Seite des Gleichungssystems
         *
         * Zur Lösung sind zwei Schritte notwendig:
         *      1. Lösen von Ly=b durch Vorwärtseinsetzen
         *      2. Lösen von Ux=y durch Rückwärtssubstitution
         */
        void solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs);

    //Attribute:
    private:
        QVector<QVector<T> > L;
        QVector<QVector<T> > U;

    };
}

#endif // GAUSS_H
