#ifndef JACOBI_H
#define JACOBI_H

#include "Iterativesolver.h"

namespace algorithms {

    /*!
     * \brief Die Klasse Jacobi implementiert das gleichnamige iterative Verfahren zum Lösen von LGS-Systemen (Ax=b)
     *
     * Sie implementiert einzig die Lösungsfunktion
     */
    template <class T>
    class Jacobi : public IterativeSolver<T> {
    public:


        /*!
         * \brief Standardkonstruktor
         */
        Jacobi();

        /*!
         * \brief solve implementiert die Approximation der Lösung des LGS-Systems nach der Vorschrift des Jacobi-Algorithmus
         * \param result dient zur Speicherung der approximierten Lösung
         * \param matrix ist die Matrix A des Gleichungssystems
         * \param rhs ist die rechte Seite des Gleichungssystems
         */
        void solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs);
    };

}

#endif // JACOBI_H
