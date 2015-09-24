#ifndef CRANKNICOLSON_H
#define CRANKNICOLSON_H

#include "Intmethod.h"

namespace algorithms {

    /*!
     *    \brief Die Klasse CrankNicolson implementiert die gleichnamige Integrationsmethode zur Approximation der Lösung der ortsdiskretisierten Wärmeleitungsgleichung
     *
     *    Sie implementiert:
     *        - eine Funtkion, die der Berechnung des Zustandes zum nächsten Zeitpunkt dient
     *        - eine Funktion, die die benötigten Zeitpunkte der Wärmequellen liefert und zusätzlich, ob diese aus vorherigen Berechnungen wiederverwendet werden können
     *        - eine Funktion, die für das Verfahren spezifische Vorbereitungen trifft
     */
    template <class T>
    class CrankNicolson : public IntMethod<T> {
    public:

        /*!
         *    \brief Standardkonstruktor
         */
        CrankNicolson();

        /*!
         * \brief calcNextStep dient zur Approximation des Zustandes zum nächsten Zeitpunkt
         * \param last beinhaltet den vorherigen Zustand
         * \param next dient zur Speicherung der berechneten Approximation
         * \param heatSources enthält die benötigten Wärmequellen
         */
        void calcNextStep(QVector<T> const & last, QVector<T> & next, QVector<QVector<T>* > const & heatSources) const;

        /*!
         * \brief getNeedetHeatSources gibt die zur Berechnung benötigten Wärmequellen an
         * \param neededTimeSteps dient zur Übergabe der benötigten Zeitpunkte
         * \param reusable beinhaltet, dass bei diesem Verfahren die bereits ausgewerteten Wärmequellen wiederverwendet werden können
         */
        void getNeedetHeatSources(QVector<T> & neededTimeSteps, bool & reusable) const;

        /*!
         * \brief setUpSpecific stellt die für das Crank-Nicolson-Verfahren benötigten Matrizen auf
         * \param thermalDiffusivities beinhaltet die Temperaturleitkoeffizienten eines jeden Gitterpunktes (Standard-Nummerierung)
         */
        void setUpSpecific(QVector<T> const & thermalDiffusivities);

    private:
        CRS<T> rhsMatrix;
    };

}


#endif // CRANKNICOLSON_H
