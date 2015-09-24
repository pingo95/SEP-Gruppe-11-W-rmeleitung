#ifndef INTMETHOD_H
#define INTMETHOD_H

#include <QVector>

#include "Solver.h"


/*!
 *    \brief algorithms enthält alle verwendeten mathematischen Verfahren/grundlegende Datenstrukturen
 */
namespace algorithms {

    /*!
     * \brief Die abstrakte Basisklasse IntMethod stellt den Rumpf einer Integrationsmethode zur Lösung der ortsdiskretisierten Wärmeleitungsgleichung bereit
     *
     * rein virtuelle Funktionen sind:
     *      - calcNextStep: soll die Approximation zum nächsten Zeitpunkt berechnen
     *      - getNeededHeatSources: soll die benötigten Zeitpunkte, an denen die Wärmequellen ausgwertet werden müssen, zurückliefern
     *      - setUpSpecific: soll für das jeweilige Verfahren spezifische Vorbereitungen zur Berechnung der Approximation treffen (Iterationsmatrix aufstellen, ...)
     */
    template <class T>
    class IntMethod {
    //Funktionen:
    public:

        /*!
         * \brief Standardkonstruktor
         */
        IntMethod();

        /*!
         * \brief virtueller Destruktor
         */
        virtual ~IntMethod();

        virtual void calcNextStep(QVector<T> const & last, QVector<T> & next, QVector< QVector<T>* > const & heatSources) const=0;
        virtual void getNeedetHeatSources(QVector<T> & neededTimeSteps, bool &reusable) const=0;

        /*!
         * \brief getSolver liefert eine Referenz auf den aktuell aktiven Löser für lineare Gleichungssysteme
         * \return Referenz auf aktiven LGS-Löser
         */
        Solver<T>* const & getSolver() const;

        /*!
         * \brief selectSolver setzt einen neuen LGS-Löser
         * \param solver zeigt auf das Objekt des gewünschten Lösers für lineare Gleichungssysteme
         */
        void selectSolver(Solver<T>* solver);

        /*!
         * \brief setUp trifft Vorbereitungen für die folgenden Berechnungen
         * \param n ist die Stützstellenanzahl der Ortdiskretisierung in eine Dimension
         * \param m ist die Anzahl der zu verwendenen Berechnungsschritte, um die Approximation zum Endzeitpunkt zu erhalten
         * \param Time gibt den Endzeitpunkt an
         * \param thermalDiffusivities enthält die Temperaturleitkoeffizienten eines jeden Gitterpunktes (Standard-Nummerierung)
         */
        void setUp(int const n, int const m, T const Time, QVector<T> const & thermalDiffusivities);
        virtual void setUpSpecific(QVector<T> const & thermalDiffusivities)=0;

    //Attribute:
    protected:
        Solver<T>* activeSolver;
        T deltaT;
        T deltaX;
        CRS<T> itMatrix;
        int n;
    };

}

#endif // INTMETHOD_H
