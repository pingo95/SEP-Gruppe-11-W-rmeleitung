#ifndef ITERATIVESOLVER_H
#define ITERATIVESOLVER_H

#include "Solver.h"

namespace algorithms {

    /*!
     * \brief Die abstrakte Basisklasse IterativeSolver stellt einen Rumpf für iterative Löser linearer Gleichungssysteme bereit
     *
     *  lediglich die solve-Methode ist zu implementieren, diese soll die Lösung des LGS Ax=b iterativ approximieren
     */
    template <class T>
    class IterativeSolver : public Solver<T> {
    public:

        /*!
         * \brief Standardkonstruktor
         */
        IterativeSolver();

        /*!
         * \brief virtueller Destruktor
         */
        virtual ~IterativeSolver();

        /*!
         * \brief getEps liefert die gewählte Schranke für den relativen Fehler
         * \return aktuelle Fehlerschranke
         */
        double getEps() const;

        /*!
         * \brief get-Funktion für die Anzahl bei der Approximation der Lösung des linearen Gleichungssystems benötigten Iterationen
         * \return Anzahl Berechnungsschritte
         */
        int getItCount() const;

        /*!
         * \brief get-Funktion für die aktuell gewählte obere Begrenzung der Berechnungsschrittanzahl
         * \return aktuell gewähltes Maximum an Iterationen
         */
        int getMaxIt() const;

        /*!
         * \brief set-Funktion für die obere Schranke des relativen Fehlers
         * \param eps stellt die zu setzende obere Schranke dar
         */
        void setEps(double const eps);

        /*!
         * \brief set-Funktion für die obere Begrenzung der Berechnugsschrittanzahl
         * \param maxIt entspricht der neuen maximalen Anzahl an Iterationen
         */
        void setMaxIt(int const maxIt);
        virtual void solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs)=0;
    };
}

#endif // ITERATIVESOLVER_H
