#ifndef SIMULATIONSETUP_H
#define SIMULATIONSETUP_H
#include <QList>
#include <QString>
#include "Area.h"
#include <QMetaType>

namespace model {
    /*!
     * \brief Die Klasse SimulationSetup implementiert die Datenhaltung für
     *  die Simulationseinstellungen.
     *
     * Die Einstellungen bestehen aus:
     *      - der ausgewählten Integrationsmethode mit
     *             - der Zeitdiskretisierungsgröße m
     *             - dem Endzeitpunkt T
     *      - dem ausgewählten LGS Löser mit
     *              - der relativen Fehlertoleranz
     *              - der maximalen Iterationsanzahl
     *      - der Ortsdiskretisierungsgröße n
     *      - vier Randwerten und einem Anfangswert
     *      - den Gebieten für Temperaturleitkoeffizienten und Wärmequellen mit jeweils
     *              - einem Count für die Gebietsanzahl
     *              - einer Liste der Gebiete vom Typ Area
     *              - einem Hintergrundwert
     */
    class SimulationSetup
    {

    // Enum:
    public:
        /*!
         * \brief Der Enum SimulationSetup::AreaType dient zur Unterscheidung
         * der beiden Gebietstypen.
         *
         * Funktionen, die Gebiete bearbeiten o.ä., arbeiten i.A. mit Objekten vom
         * Typ Area bzw. Zeiger of Objekte dieses Typen. Dieser Enum dient dann dazu
         * zu entscheiden, welcher Gebietstyp bearbeitet werden soll.
         */
        enum AreaType
        {
            /*! Wärmequellen-Gebiet */
            AreaHeatSource = 0,
            /*! Temperaturleitkoeffizienten-Gebiet */
            AreaThermalDiffusivity = 1,
        };
        /*!
         * \brief Der Enum SimulationSetup::IBV dient zur Unterscheidung der
         * Randwerte und des Anfangswertes.
         *
         */
        enum IBV
        {
            /*! Unterer Randwert */
            BottomBoundary = 0,
            /*! Anfangswert */
            InitialValue = 1,
            /*! Linker Randwert */
            LeftBoundary = 2,
            /*! Rechter Randwert */
            RightBoundary = 3,
            /*! Oberer Randwert */
            TopBoundary = 4
        };

        /*!
         * \brief AreaMaxValue enthält die Obergrenzen für die beiden Gebietstypen.
         *
         * Die Dereferenzierung hat dabei mittels des Enums AreaType zu erfolgen,
         * also etwa SimulationSetup::AreaMaxValue[SimulationSetup::AreaType].\n
         * Die Obergrenzen dabei sind:
         *      - 1000 K/s für die Wärmequellen und
         *      - 2e-4 m²/s für die Temperaturleitfähigkeiten.
         */
        static double const AreaMaxValue[2];
        /*!
         * \brief AreaMinValue enthält die Untergrenzen für die beiden Gebietstypen.
         *
         * Die Dereferenzierung hat dabei mittels des Enums AreaType zu erfolgen,
         * also etwa SimulationSetup::AreaMinValue[SimulationSetup::AreaType].\n
         * Die Untergrenzen dabei sind:
         *      - 0 K/s für die Wärmequellen und
         *      - 1e-6 m²/s für die Temperaturleitfähigkeiten.
         */
        static double const AreaMinValue[2];
        /*!
         * \brief MaxTemperature ist die Obergrenze für die einstellbaren Temperaturen.
         *
         * Die Obergrenze beträgt 1000 K.\n
         * Die Grenze gilt insbesondere für die Randwerte und den Anfangswert und das
         * Skalarieren der Farben beim Visualisieren der Simulationsergebnisse, sie
         * gilt dabei aber nicht zwingenderweise für die Simulationsergebnisse selber.
         */
        static double const MaxTemperature;
        /*!
         * \brief MinTemperature ist die Untergrenze für die einstellbaren Temperaturen.
         *
         * Die Untergrenze beträgt 1000 K. \n
         * Die Grenze gilt insbesondere für die Randwerte und den Anfangswert und das
         * Skalarieren der Farben beim Visualisieren der Simulationsergebnisse, sie
         * gilt dabei aber nicht zwingenderweise für die Simulationsergebnisse selber.
         */
        static double const MinTemperature;

    // Funktionen:
    public:
        /*!
         * \brief Standard-Konstruktor für die Klasse SimulationSetup.
         *
         * Erstellt ein SimulationSetup Objekt mit folgenden Standard-Einstellungen:
         *      - impliziter Euler (algorithms::ImpEuler) als Integrationsmethode mit
         *          - m = 1 als Zeitdiskretisierung
         *          - T = 60.0s als Endzeitpunkt
         *      - Jacobi (algorithms::Jacobi) als LGS löser mit
         *          - 1e-10 als relative Genauigkeit
         *          - 1000 als maximale Iterationsanzahl
         *      - n = 3 als Ortsdiskretisierung
         *      - 300.00 K für die Randwerte und den Anfangswert
         *      - 0 K als Hintergrundwert für die Wärmequellen-Gebiete
         *      - 1e-4 m²/s als Hintergrundwert für die Temperaturleitkoeffizienten-Gebiete
         */
        explicit SimulationSetup();
        /*!
         * \brief Copy-Konstruktor für die Klasse SimulationSetup.
         * \param rhs SimulationSetup Objekt, das kopiert wird.
         *
         * Insbesondere kopiert der Konstruktor jedes einzelne Gebiet vom Type Area,
         * nicht (nur) die Zeiger auf diese.
         */
        explicit SimulationSetup(SimulationSetup const & rhs);
        /*!
         * \brief Desktrukor für die Klasse SimulationSetup.
         */
        ~SimulationSetup();

        /*!
         * \brief addNewArea fügt den Simulationseinstellungen ein neues Area - Gebiet hinzu.
         * \param xKoords x-Werte der Punkte des neuen Gebietes
         * \param yKoords y-Werte der Punkte des neuen Gebietes
         * \param value Wert des neuen Gebietes
         * \param type Typ des neuen Gebietes
         *
         * Der Parameter type entscheidet dabei, ob das Gebiet als Wärmequellen-
         * oder Temperaturleitkoeffizienten-Gebiet hinzugefügt wird.
         * \see AreaType
         */
        void addNewArea(QVector<double> const & xKoords,
                        QVector<double> const & yKoords,
                        double value,
                        SimulationSetup::AreaType type);
        /*!
         * \brief deleteArea löscht ein Area - Gebiet.
         * \param pos Position des zu löschenden Gebietes
         * \param type Typ des zu löschenden Gebietes
         *
         * Der Parameter type entscheidet dabei, ob ein Wärmequellen-
         * oder Temperaturleitkoeffizienten-Gebiet gelöscht wird.
         * \see AreaType
         */
        void deleteArea(int const pos, SimulationSetup::AreaType type);

        /*!
         * \brief Getter-Funktion für den Hintergrundwert der Gebiete
         * \param type Entscheidet, welcher Hintergrundwert zurückgegeben wird
         * \return Wärmequellen- oder Temperaturleitkoeffizienten-Hintergrundwert
         * \see AreaType, setAreaBackgroundValue
         */
        double getAreaBackgroundValue(SimulationSetup::AreaType type) const;
        /*!
         * \brief Getter-Funktion für die Gebiete (zum Visualisieren).
         * \param type Entscheidet, welche Gebiete zurückgegeben werden
         * \return Wärmequellen- oder Temperaturleitkoeffizienten-Gebiete
         * \see AreaType
         */
        QList<Area*> const & getAreas(SimulationSetup::AreaType type) const;
        /*!
         * \brief Getter-Funktion für der Gebietsanzahl
         * \param type Entscheidet, welcher Count zurückgegeben wird
         * \return Wärmequellen- oder Temperaturleitkoeffizienten-Anzahl
         * \see AreaType
         */
        int getAreaCount(SimulationSetup::AreaType type) const;
        /*!
         * \brief getContainingAreaID überprüft, ob ein Area - Gebiet den übergebenen
         * Punkt enthält.
         * \param xKoord x-Koordinate des zu überprüfenden Punktes
         * \param yKoord y-Koordinate des zu überprüfenden Punktes
         * \param type Entscheidet, welche Gebiete überprüft werden
         * \return Die ID des umschließenden Gebietes
         *
         * Die Funktion überprüft abhängig vom Parameter type entweder die
         * Wärmequellen- oder die Temperaturleitkoeffizienten-Gebiete der Reihe
         * nach, ob sie den übergebenen Punkt enthalten und gibt im Erfolgsfall
         * die ID des ersten umschließenden Gebietes zurück, ansonsten -1.
         * \see AreaType
         */
        int getContainingAreaID(double const xKoord, double const yKoord,
                                SimulationSetup::AreaType type);

        /*!
         * \brief Getter-Funktion für die Randwerte und den Anfangswert
         * \param ibv Entscheidet, welcher Wert zurückgegeben wird
         * \return aktueller Rand- oder Anfangswert
         * \see IBV, setIBV
         */
        double getIBV(SimulationSetup::IBV ibv);
        /*!
         * \brief Getter-Funktion für die Zeitdiskretisierungsgröße m
         * \return aktueller Wert der Zeitdiskretisierungsgröße
         * \see setM
         */
        long getM() const;
        /*!
         * \brief Getter-Funktion für die Ortsdiskretisierungsgröße n
         * \return aktueller Wert der Ortsdiskretisierungsgröße
         * \see setN
         */
        long getN() const;
        /*!
         * \brief Getter-Funktion für die ausgewählte Integrationsmethode
         * \return aktuell ausgewählte Integrationsmethode
         * \see selectIntMethod
         */
        QString getSelectedIntMethod() const;
        /*!
         * \brief Getter-Funktion für den ausgewählten LGS Löser
         * \return aktuell ausgewählter LGS Löser
         * \see selectSolver
         */
        QString getSelectedSolver() const;
        /*!
         * \brief Getter-Funktion für die relative Fehlertoleranz des LGS Lösers
         * \return aktuelle relative Fehlertoleranz
         * \see setSolverMaxError
         */
        double getSolverMaxError() const;
        /*!
         * \brief Getter-Funktion für die maximale Iterationsanzahl des LGS Lösers
         * \return aktuelle maximale Iterationsanzahl
         * \see setSolverMaxIt
         */
        int getSolverMaxIt() const;
        /*!
         * \brief Getter-Funktion für den Endzeitpunkt T
         * \return aktueller Endzeitpunkt
         * \see setT
         */
        double getT() const;

        /*!
         * \brief removeLastArea entfernt ein zuletzt hinzugefügtes Area - Gebiet
         * \param type Typ des zu entfernenden Gebietes
         *
         * Der Parameter type entscheidet dabei, ob das letzte Wärmequellen-
         * oder Temperaturleitkoeffizienten-Gebiet entfernt wird.
         * \see AreaType
         */
        void removeLastArea(SimulationSetup::AreaType type);
        /*!
         * \brief reorderAreas verändert die Reihenfolge der Area - Gebiete.
         * \param pos Position des zu verschiebenden Gebietes
         * \param dir Richtung, in die das Gebiet verschoben wird
         * \param type Typ der Gebiete, deren Reihenfolge geändert wird
         *
         * Die Werte des Parameters dir haben folgende Bedeutungen:
         *      - -2 an die vorderste (visuell) Position (ganz nach unten in der Tabelle)
         *      - -1 eine Position (visuell) nach vorne (nach unten in der Tabelle)
         *      -  1 eine Position (visuell) nach hinten (nach oben in der Tabelle)
         *      -  2 an die letzte (visuell) Position (ganz nach oben in der Tabelle).
         *
         * Die Reihenfolge der Gebiete ist von Bedeutung, sobald sich zwei Gebiete
         * überlappen, in diesem Fall gilt bei einer Simulation der Wert des (visuell)
         * vorderen Gebietes.
         *
         * Der Parameter type entscheidet dabei, ob die Reihenfolge der Wärmequellen-
         * oder Temperaturleitkoeffizienten-Gebiete verändert wird.
         * \see AreaType
         */
        void reorderAreas(int const pos, int const dir, SimulationSetup::AreaType type);

        /*!
         * \brief Setter-Funktion für die ausgewählte Integrationsmethode.
         * \param newIntMethod neue Integrationsmethode
         *
         * Es findet keine Überprüfung der Gültigkeit des neuen Wertes statt, die Klasse
         * SimulationWorker stellt eine Liste der gültigen Integrationsmethoden zur Verfügung, da diese Klasse auch die
         * IntMethod (algorithms::IntMethod) - Objekte verwaltet und das UI (presentation::UI)
         * beschränkt die Eingabemöglichkeiten des Benutzers auf die Elemente dieser Liste.
         * \see getSelectedIntMethod, SimulationWorker::getIntMethodNames
         */
        void selectIntMethod(QString const newIntMethod);
        /*!
         * \brief Setter-Funktion für den ausgewählten LGS Löser.
         * \param newSolver neuer LGS Löser
         *
         * Es findet keine Überprüfung der Gültigkeit des neuen Wertes statt, die Klasse
         * SimulationWorker stellt eine Liste der gültigen Integrationsmethoden zur Verfügung, da diese Klasse auch die
         * Solver (algorthims::Solver) - Objekte verwaltet und das UI (presentation::UI)
         * beschränkt die Eingabemöglichkeiten des Benutzers auf die Elemente dieser Liste.
         * \see getSelectedSolver, SimulationWorker::getSolverNames
         */
        void selectSolver(QString const newSolver);
        /*!
         * \brief Setter-Funktion für den Hintergrundwert der Area - Gebiete.
         * \param newValue Hintergrundwert
         * \param type Typ der Gebiete, deren Hintergrundwert geänder wird.
         *
         * Der Hintergrundwert wird bei einer Simulation für alle Gitterpunkte verwendet,
         * die von keinem Area - Gebiet abgedeckt werden.
         *
         * Der Parameter type entscheidet dabei, ob der Hintergrundwert der Wärmequellen-
         * oder Temperaturleitkoeffizienten-Gebiete verändert wird.
         * \see AreaType, getAreaBackgroundValue
         */
        void setAreaBackgroundValue(double const newValue, SimulationSetup::AreaType type);
        /*!
         * \brief Setter-Funktion für die Randwerte und den Anfangswert
         * \param newValue neuer Rand- bzw Anfangswert
         * \param ibv Entscheidet, welcher Wert geändert wird
         * \see IBV, getIBV
         */
        void setIBV(double const newValue, SimulationSetup::IBV ibv);
        /*!
         * \brief Setter-Funktion für die Zeitdiskretisierungsgröße m
         * \param newM neuer Wert der Zeitdiskretisierungsgröße
         * \see getM
         */
        void setM(int const newM);
        /*!
         * \brief Setter-Funktion für die Ortsdiskretisierungsgröße m
         * \param newN neuer Wert der Ortsdiskretisierungsgröße
         * \see getN
         */
        void setN(int const newN);
        /*!
         * \brief Setter-Funktion für die relative Fehlertoleranz des LGS Lösers.
         * \param newMaxError neue relative Fehlertoleranz
         * \see getSolverMaxError
         */
        void setSolverMaxError(double const newMaxError);
        /*!
         * \brief Setter-Funktion für die maximale Iterationsanzahl des LGS Lösers.
         * \param newMaxIt neue maximale Iterationsanzahl
         * \see getSolverMaxIt
         */
        void setSolverMaxIt(int const newMaxIt);
        /*!
         * \brief Setter-Funktion für den Endzeitpunkt T
         * \param newT neuer Endzeitpunkt
         * \see getT
         */
        void setT(double const newT);

        /*!
         * \brief updateAreaValue ändert den Wert eines Area - Gebietes.
         * \param pos Position des zu veränderen Gebietes
         * \param newValue neuer Wert des Gebietes
         * \param type Typ des zu veränderen Gebietes
         * \see AreaType
         */
        void updateAreaValue(int const pos, double const newValue,
                             SimulationSetup::AreaType type);

    // Attribute:
    private:
        QList<Area*> heatSources;
        double heatSourcesBackgroundValue;
        int heatSourcesCount;
        double iBVs[5];
        long m;
        long n;
        QString selectedIntMethod;
        QString selectedSolver;
        double solverMaxError;
        int solverMaxIt;
        double T;
        QList<Area*> thermalDiffusivities;
        double thermalDiffusivitiesBackgroundValue;
        int thermalDiffusivitiesCount;

        friend QTextStream & operator<<(QTextStream & out, SimulationSetup & simSetup);
        friend QTextStream & operator>>(QTextStream & in, SimulationSetup & simSetup);
    };

    /*!
     * \brief Überladen des operator << für QTextStream und SimulationSetup.
     * \param out QTextStream, in den die Einstellungen geschrieben wird
     * \param simSetup auszugebene Simulationeinstellungen
     * \return QTextStream, in den die Einstellungen geschrieben wird
     *
     * Dieser Operator wurde für das Speichern von Simulationseinstellungen überladen.
     */
    QTextStream & operator<<(QTextStream & out, SimulationSetup & simSetup);
    /*!
     * \brief Überladen des operator >> für QTextStream und SimulationSetup.
     * \param in QTextStream, aus dem die Einstellungen eingelesen wird
     * \param simSetup "Ziel"-Simulationeinstellungen
     * \return QTextStream, aus dem die Einstellungen eingelesen wird
     *
     * Dieser Operator wurde für das Laden von Simulationseinstellungen überladen.
     */
    QTextStream & operator>>(QTextStream & in, SimulationSetup & simSetup);
}
Q_DECLARE_METATYPE(model::SimulationSetup *)
#endif // SIMULATIONSETUP_H
