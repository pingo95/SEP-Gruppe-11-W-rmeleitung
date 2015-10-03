#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QObject>
#include <QMap>
#include <QMutex>
#include "../algorithms/Intmethod.h"
#include "Simulationsetup.h"

namespace model {
    /*!
     * \brief Die Klasse SimulationWorker implementiert die rechenintensiven Operationen
     * des Modells.
     *
     * Zu diesen Operationen zählen:
     *      - die numerische Simulation der Wärmeleitungsgleichung
     *      - das Einlesen der Temperatur-Messdaten für die Optimierung
     *      - die numerische Optimierung der Temperaturleitkoeffizienten an
     *          die Messdaten
     *
     * Die genannte Funktionalität wurde in diese Klasse ausgelagert, um so das Verschieben
     * der rechnenintensiven Operationen in einen anderen Thread mittels QThreads zu
     * ermöglichen, sodass das UI auch während einer Simulation auf Benutzereingaben
     * reagieren kann.
     */
    class SimulationWorker : public QObject
    {
        Q_OBJECT


    // Funktionen:
    public:
        /*!
         * \brief Kontruktor für die Klasse SimulationWorker.
         * \param parent Parent-Widget, hier immer ein Model - Objekt
         *
         * Die Objekte der Integrationsmethoden (algorithms::IntMethod) und der LGS Löser
         * (algorithms::Solver) werden hier noch nicht alloziert, dies geschieht erst durch
         * den Aufruf der Funktion initializeMaps.
         * \see initializeMaps
         */
        explicit SimulationWorker(QObject *parent = 0);
        /*!
         *  \brief Destruktor für die Klasse SimulationWorker.
         *
         * Löscht, falls simuliert wurde oder Daten eingelesen wurden, das Ergebnis/die Messdaten.\n
         * Gibt auch die Objekte der Integrationsmethoden (algorithms::IntMethod) und der LGS Löser
         * (algorithms::Solver) frei, falls zuvor initializeMaps aufgerufen wurde.
         * \see initializeMaps
         */
        ~SimulationWorker();

        /*!
         * \brief abortWork ermöglicht eine laufende Simulation oder Optimierung abzubrechen.
         *
         * Der Abbruch findet dabei vor dem ersten oder nach dem nächsten berechnetem Zeit- bzw.
         * Optimierungsschritt statt.\n Da der Abbruch vom Model - Objekt ausgelöst wird und dieses
         * sich in dem Hauptthread befindet, wird die Thread-Sicherheit mit Hilfe eines QMutex
         * sichergestellt.
         * \see startOptimizationSlot, startSimulationSlot, Model::abortWork
         */
        void abortWork();
        /*!
         * \brief getIntMethodNames stellt eine Liste der wählbaren Integrationsmethoden zur Verfügung.
         * \return Liste alle wählbaren Integrationsmethoden
         *
         * Diese Liste wird vom UI (presentation::UI) dazu genutzt die Benutzereingabemöglichkeiten auf
         * gültige Integrationsmethoden zu beschränken.\n
         * initializeMaps sollte vorher aufgerufen worden sein, ansonsten wird eine leere Liste
         * zurückgegeben.
         * \see initializeMaps, SimulationSetup::selectIntMethod
         */
        QList<QString> const getIntMethodNames() const;
        /*!
         * \brief getSolverNames stellt eine Liste der wählbaren LGS Löser zur Verfügung.
         * \return Liste aller wählbaren LGS Löser
         *
         * Diese Liste wird vom UI (presentation::UI) dazu genutzt die Benutzereingabemöglichkeiten auf
         * gültige LGS Löser zu beschränken.\n
         * initializeMaps sollte vorher aufgerufen worden sein, ansonsten wird eine leere Liste
         * zurückgegeben.
         * \see initializeMaps, SimulationSetup::selectSolver
         */
        QList<QString> const getSolverNames() const;
        /*!
         * \brief Getter-Funktion für die Zeitdiskretisierungsgröße m.
         * \return die Zeitdiskretisierungsgröße
         *
         * Die im SimulationWorker - Objekt gespeicherte Zeitdiskretisierungsgröße m bezeichnet die
         * Zeitdiskretisierungsgröße der letzen Simulation, deren Ergebnis aktuell gespeichert ist
         * und wird daher im Modell und UI als resultM bezeichnet.\n
         * (Sie dient daher zu Visualisierung dieses Ergebnisses.)
         *
         * Der Fall, dass noch keine Simulation durchgeführt wurde, wird schon im Modell abgefangen.
         * \see getResult, startSimulationSlot, Model::getResultM
         */
        long getM() const;
        /*!
         * \brief Getter-Funktion für die Ortsdiskretisierungsgröße n.
         * \return die Ortsdiskretisierungsgröße
         *
         * Die im SimulationWorker - Objekt gespeicherte Ortsdiskretisierungsgröße n bezeichnet die
         * Ortsdiskretisierungsgröße der letzen Simulation, deren Ergebnis aktuell gespeichert ist
         * und wird daher im Modell und UI als resultN bezeichnet.\n
         * (Sie dient daher zu Visualisierung dieses Ergebnisses.)
         *
         * Der Fall, dass noch keine Simulation durchgeführt wurde, wird schon im Modell abgefangen.
         * \see getResult, startSimulationSlot, Model::getResultN
         */
        long getN() const;
        /*!
         * \brief Getter-Funktion für eingelesenen Temperatur-Messdaten (zur Visualisierung).
         * \return eine Referenz auf die eingelesenen Daten
         *
         * Der Fall, dass noch keine Daten eingelesen wurden, wird schon im Modell abgefangen.
         * \see startReadingDataSlot, Model::getObservations
         */
        double ** const &getObservations() const;
        /*!
         * \brief Getter-Funktion für die Dimension der eingelesenen Temperatur-Messdaten
         * (zur Visualisierung).
         * \return die Dimension der Daten
         *
         * Die Dimension der Daten ist i.A. die Quadratwurzel aus der Anzahl der eingelesenen Daten.\n
         * Der Fall, dass noch keine Daten eingelesen wurden, wird schon im Modell abgefangen.
         * \see startReadingDataSlot, Model::getObservationsDim
         */
        int getObservationsDim() const;
        /*!
         * \brief Getter-Funktion für das Ergebnis der letzten Simulation.
         * \return eine Referenz auf das Ergebnis der letzen Simulation
         *
         * Der Fall, dass noch keine Simulation durchgeführt wurde, wird schon im Modell abgefangen.
         * \see startSimulationSlot, Model::getResult
         */
        double*** const & getResult() const;
        /*!
         * \brief Getter-Funktion für den Endzeitpunkt T.
         * \return der Endzeitpunkt
         *
         * Der im SimulationWorker - Objekt gespeicherte Endzeitpunkt T bezeichnet den
         * Endzeitpunkt der letzen Simulation, deren Ergebnis aktuell gespeichert ist
         * und wird daher im Modell und UI als resultT bezeichnet.\n
         * (Sie dient daher zu Visualisierung dieses Ergebnisses.)
         *
         * Der Fall, dass noch keine Simulation durchgeführt wurde, wird schon im Modell abgefangen.
         * \see getResult, startSimulationSlot, Model::getResultT
         */
        double getT() const;
        /*!
         * \brief initializeMaps alloziert die Integrationsmethoden - und LGS Löser - Objekte.
         *
         * Die Objekte werden dabei in zwei QMaps gespeichert mit QStrings als Keys und polymorphen
         * Zeigern auf algorithms::IntMethod - bzw. auf algorithms::Solver - Objekte als Werte, sodass
         * gewünschte Integrationsmethode und gewünschter LGS Löser zur Laufzeit vom Benutzer nach
         * dem Strategy-Entwurfsmuster ausgewählt werden können. Die Dereferenzierung der Strings
         * erfolgt dabei erst während der Simulation, vorher sind diese in einem SimulationSetup -
         * Objekt im Modell gespeichert.
         *
         * Das Allozieren der Objekte wurde in diese Funktion ausgelagert (aus dem Konstruktor), damit
         * sie erst erfolgt, nachdem das SimulationWorker - Objekt im Konstruktors des Modells angelegt
         * und in seinen eigenen Thread verschoben wurde, um so ggf. einer NUMA-Architektur gerecht zu
         * allozieren.
         * \see startSimulationSlot, Model::Model
         */
        void initializeMaps();

    signals:
        /*!
         * \brief beginningStage wird ausgesendet, wenn ein neuer Abschnitt einer Simulation oder
         * Optimierung angefangen wird.
         * \param stage Name des neuen Abschnittes
         * \param stepCount Anzahl der Teilschritte des neuen Abschnittes
         * \param simulation gibt an, ob eine Simulation oder eine Optimierung das Signal ausgelöst hat
         *
         * Dieses Signal dient zur Visualisierung des Fortschritts einer Simulation oder Optimierung
         * auf einem Fortschrittsbalken.
         *
         * Verbunden wird dieses Signal im Konstruktor des Modells.
         * \see finishedStep, startOptimizationSlot, startSimulationSlot, Model::Model
         */
        void beginningStage(QString stage, int stepCount, bool simulation = true);
        /*!
         * \brief finishedOptimization wird ausgesendet, wenn eine Optimierung beendet wurde.
         * \param success gibt an, ob die Optimierung erfolgreich beendet oder abgebrochen wurde.
         *
         * Verbunden wird dieses Signal im Konstruktor des Modells.
         * \see startOptimizationSlot, Model::Model
         */
        void finishedOptimization(bool success);
        /*!
         * \brief finishedReadingData wird ausgesendet, wenn das Einlesen von Temperatur-Messdaten
         * beendet wurde.
         *
         * Verbunden wird dieses Signal im Konstruktor des Modells.
         * \see startReadingDataSlot, Model::Model
         */
        void finishedReadingData();
        /*!
         * \brief finishedSimulation wird ausgesendet, wenn eine Simulation beendet wurde.
         * \param success gibt an, ob die Simulation erfolgreich beendet oder abgebrochen wurde.
         *
         * Verbunden wird dieses Signal im Konstruktor des Modells.
         * \see startSimulationSlot, Model::Model
         */
        void finishedSimulation(bool success);
        /*!
         * \brief finishedStep wird ausgesendet, wenn ein Teilschritt eines Abschnittes einer
         * Simulation oder Optimierung beendet wurde.
         * \param step Nummer des beendeten Schrittes
         * \param simulation gibt an, ob eine Simulation oder eine Optimierung das Signal ausgelöst hat
         *
         * Dieses Signal dient zur Visualisierung des Fortschritts einer Simulation oder Optimierung
         * auf einem Fortschrittsbalken. Zuvor muss ein Aufruf von beginningStage erfolgt sein.
         *
         * Verbunden wird dieses Signal im Konstruktor des Modells.
         * \see beginningStage, startOptimizationSlot, startSimulationSlot, Model::Model
         */
        void finishedStep(int step, bool simulation = true);
        /*!
         * \brief startedWork wird ausgesendet, wenn eine Simulation oder Optimierung angefangen wurde.
         *
         * Das Signal wird dabei nicht unmittelbar nach Beginn des entsprechenden Slots ausgelöst,
         * sondern erst nach dem die Simulationseinstellungen kopiert wurden. Dies teilt dem Modell mit,
         * ab wann die Simulationseinstellungen wieder, auch noch während einer Simulation oder
         * Optimierung, (durch den Benutzer) verändert werden dürfen.
         *
         * Verbunden wird dieses Signal im Konstruktor des Modells.
         * \see startOptimizationSlot, startSimulationSlot, Model::Model
         */
        void startedWork();
        /*!
         * \brief simulationLogUpdate wird ausgesendet, wenn ein (wichtiger) Abschnitt einer
         * Simulation erreicht, bearbeitet oder beendet wird/wurde.
         * \param message Text für die Simulations-Log
         *
         * Diese Signal dient dazu den Ablauf einer Simulation in einem Log festzuhalten bzw.
         * zu visualisieren.
         *
         * Verbunden wird dieses Signal im Konstruktor des Modells.
         * \see startSimulationSlot, Model::Model
         */
        void simulationLogUpdate(QString message);

    public slots:
        /*!
         * \brief startOptimizationSlot führt eine Optimierung mit den übergebenen Einstellungen durch.
         * \param simSetupTemplate die Simulationseinstellungen für die Optimierung
         * \param overrideTD entscheidet, ob die Gebiete für die Koeffizienten aus simSetupTemplate oder (nur) overrideValue genutzt werden
         * \param overrideValue wird benutzt, falls overrideTD true ist, und dient als Anfangswert für die Optimierung
         * \param useHeatSources entscheidet, ob die Gebiete für die Wärmequellen genutzt oder ignoriert werden
         *
         * Es werden die Temperaturleitkoeffizienten mittels eines Steepest Decent Algorithmus
         * an die zuvor eingelesenen Temperatur-Messdaten optimiert. Die Zielfunktion des
         * Algoritmus ist dabei der gemittelte Quadratische Fehler des Ergebnisses der Simulation
         * mit den jeweils aktuellen Koeffizienten bezogen auf die Messdaten. Der Gradient der
         * Zielfunktion wird dabei mit Hilfe der Software dco ermittelt.
         *
         * Die Optimierung kann vom Benutzer abgebrochen werden, der Abbruch erfolgt vor dem ersten oder
         * nach dem jeweils nächsten berechneten Optimierungsschritt. Im Falle eines Abbruches kann
         * weder ein schon berechnetes Teilergebnis noch das Ergebnis der voherigen Optimierung
         * visualisiert werden, es muss zunächst wieder eine neue Simulation erfolgreich durchgeführt
         * werden.
         *
         * Der Fall, dass noch keine Daten eingelesen wurden, wird schon im Modell angefangen.
         * \see abortWork, Model::optimize
         */
        void startOptimizationSlot(SimulationSetup * simSetupTemplate, bool overrideTD,
                                   double overrideValue, bool useHeatSources);
        /*!
         * \brief startReadingDataSlot ließt aus der übergebenen Datei die Temperatur-Messdaten
         * für die Optimierung ein.
         * \param filename Dateiname bzw. Dateipfad
         * \param obsCount Anzahl Messdaten
         *
         * Die Daten werden dabei ohne Syntax-Überprüfung eingelesen, d.h. es wird angenommen dass in
         * der übergebenen Datei nur die Temperatur-Werte in Kelvin duch Leerzeichen oder Zeilenumbrüche
         * stehen.\n
         * Der erste Messwert wird dem Gitterpunkt links unten, d.h. den Koordinaten (0,0) zugeordnet,
         * die weiteren Werten zeilenweise von links nach rechts, von unten nach oben zugeordnet.
         *
         * \see startOptimizationSlot
         */
        void startReadingDataSlot(QString const filename, long const obsCount);
        /*!
         * \brief startSimulationSlot führt eine Simulation der Wärmeleitungsgleichung mit den
         * übergebenen Einstellungen durch.
         * \param simSetupTemplate die Einstellungen für die Simulation
         *
         * Die Simulation nutzt insbesondere die vom Benutzter zur Laufzeit ausgewählte
         * Integrationsmethode und LGS Löser.
         *
         * Die Simulation besteht dabei aus folgenden Schritten:
         *          - Kopieren der Simulationseinstellungen
         *          - Löschen des alten Ergebnisses
         *          - Allozieren des Speichers für das neue Ergebnis und Initialisieren mit
         *              Anfangs- und Randwerten.
         *          - Umrechnen auf die Gitterpunkte und Auswerten der Gebiete für die
         *              Temperaturleitkoeffizienten
         *          - Initialisieren der gewählten Integrationsmethode und des gewählten LGS Lösers
         *          - Umrechnen auf die Gitterpunkte und intiales Auswerten der Gebiete für
         *              die Wärmequellen
         *          - Berechnen der einzelnen Zeitschritte inklusive
         *              - Speichern des jeweils berechneten Schrittes und
         *              - falls nötig, jeweiligem neu auswerten der Wärmequellen
         *
         * Die Simulation kann vom Benutzer abgebrochen werden, der Abbruch erfolgt vor dem ersten oder
         * nach dem jeweils nächsten berechneten Zeitschritt. Im Falle eines Abbruches kann weder ein
         * schon berechnetes Teilergebnis noch das Ergebnis der voherigen Simulation visualisiert
         * werden, es muss zunächst wieder eine neue Simulation erfolgreich durchgeführt werden.
         *
         * \see algorithms::IntMethod, algorithms::Solver, startedWork, abortWork
         */
        void startSimulationSlot(SimulationSetup * simSetupTemplate);

    private:
        explicit SimulationWorker();
        explicit SimulationWorker(SimulationWorker const & rhs);
        QVector<double> * & simpleSimulation(SimulationSetup & simSetup, QVector<double> * & step1,
                              QVector<double> * & step2, QVector<double> &currentCs,
                              QList<QList<long>*> & heatSourcesIndices);

    // Attribute:
    private:
        bool abort;
        QMutex accessLock;
        bool busy;
        bool dataRead;
        QMap<QString,algorithms::IntMethod<double>*> intMethods;
        long m;
        bool mapsInitialized;
        long n;
        long obsSize;
        double ** observations;
        long optimizationN;
        bool optimized;
        QVector<double> optimizedCs;
        double *** result;
        bool simulated;
        QMap<QString,algorithms::Solver<double>*> solvers;
        double T;

    };

}

#endif // SIMULATIONWORKER_H
