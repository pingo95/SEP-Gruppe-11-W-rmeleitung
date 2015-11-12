#ifndef MODEL_H
#define MODEL_H
#include <QWidget>
#include <QThread>
#include "Simulationworker.h"

namespace presentation{
    class UI;
}

namespace model {
    /*!
     * \brief Die Klasse Model implementiert das Modell des MVC.
     *
     * Dieses besteht aus einem
     * SimulationSetup - Objekt, das die grundlegenden Simulationseinstellungen enthält,
     * einem SimulationWorker - Objekt, das die Funtkionalitäten zur Durchführung von
     * Simulationen der Wärmeleitungsgleichung und Optimierungen der
     * Temperaturleitkoeffizienten bereit stellt, und weiteren Einstellungen unter anderem
     * für die genannten Optimierungen.
     *
     * Die Klasse implementiert folgende Funktionalitäten:
     *      - Funktionen, um genannte Simulationen und Optimierungen zu starten oder
     *        abzubrechen
     *      - Status-Abfrage-Funktionen für Simulationen/Optimierungen u.ä.
     *      - Setter-Funktionen für alle Einstellungen, die nach dem Beobachter-
     *        Entwurfsmuster eine Update-Nachricht an das UI senden
     *      - eine Getter Funktion für das SimulationSetup - Objekt, um Zugriff auf dessen
     *        Getter-Funktionen zu bieten
     *      - Getter-Funktionen für die restlichen Einstellungen
     *      - Funktionen zum Hinzufügen/Bearbeiten/Entfernen von Gebieten
     */
    class Model : public QWidget
    {
        Q_OBJECT

    // Funktionen:
    public:
        /*!
         * \brief Konstruktor der Klasse Model.
         *
         * Erstellt ein Simulations-Modell mit Standardeinstellungen. Des Weiteren wird
         * das SimulationWorker - Objekt angelegt und in einen QThread ausgelagert und
         * Signale & Slots zwischen dem SimulationWorker - Objekt und dem neuen Model -
         * Objekt verbunden.
         */
        explicit Model();
        /*!
         * Destruktor der Klasse Model.
         */
        ~Model();

        /*!
         * \brief abortWork bricht eine laufende Simulation/Optimierung ab.
         * \see isWorking, SimulationWorker::abortWork
         */
        void abortWork();

        /*!
         * \brief addNewArea fügt ein neues Gebiet zu den Simulationseinstellungen
         *  hinzu und informiert das UI.
         * \param xKoords x-Werte der Punkte des neuen Gebietes
         * \param yKoords y-Werte der Punkte des neuen Gebietes
         * \param value Wert des neuen Gebietes
         * \param type Typ des neuen Gebietes
         * \see SimulationSetup::addNewArea, presentation::UI::updateNotification
         */
        void addNewArea(QVector<double> const & xKoords,
                        QVector<double> const & yKoords, double value,
                        SimulationSetup::AreaType type);

        /*!
         * \brief applyFitted überträgt das Ergebnis einer Optimierung in
         * Gebiete, die für Simulationen genutzt werden können.
         *
         * \see optimize, simulate
         */
        void applyFitted();

        /*!
         * \brief deleteArea löscht ein Gebiet aus den Simulationseinstellungen
         * und informiert das UI.
         * \param pos Position des zu löschenden Gebietes
         * \param type Typ des zu löschenden Gebietes
         * \see SimulationSetup::deleteArea, presentation::UI::updateNotification
         */
        void deleteArea(int const pos, SimulationSetup::AreaType type);

        /*!
         * \brief Abfrage-Funktion, ob schon Messdaten eingelesen wurden.
         * \return true/false für Daten bereits eingelesen
         * \see readObservations
         */
        bool getDataRead() const;
        /*!
         * \brief getIntMethodNames stellt eine Liste der wählbaren Integrationsmethoden
         * zur Verfügung.
         * \return Liste alle wählbaren Integrationsmethoden
         *
         * Weiterleitung an SimulationWorker::getIntMethodNames.
         * \see SimulationWorker::getIntMethodNames
         */
        QList<QString> const getIntMethodsNames() const;
        /*!
         * \brief Getter-Funktion für eingelesene Temperatur-Messdaten.
         * \return eingelesene Daten
         *
         * Überprüft, ob bereits Daten eingelesen wurden, falls ja Weiterleitung an
         * SimulationWorker::getObservations.
         * \see getDataRead, loadObservations, SimulationWorker::getObservations
         */
        double **getObservations() const;
        /*!
         * \brief Getter-Funktion für Dimension eingelesener Temperatur-Messdaten.
         * \return die Dimension der Daten
         *
         * Überprüft, ob bereits Daten eingelesen wurden, falls ja Weiterleitung an
         * SimulationWorker::getObservationsDim.
         * \see getDataRead, readObservations, SimulationWorker::getObservationsDim
         */
        int getObservationsDim() const;
        /*!
         * \brief Getter-Funktion für die Dimension der optimierten Werte der letzten
         * Optimierung.
         * \return die Dimension
         * \see getOptimzed, optimize, SimulationWorker::getOptimizationDim
         */
        int getOptimizationDim() const;
        /*!
         * \brief Abfrage-Funktion, ob schon eine Optimierung durchgeführt wurde.
         * \return true/false für bereits optimiert
         * \see optimize
         */
        bool getOptimized() const;
        /*!
         * \brief Getter-Funktion für optimierte Temperatur-Leifkoeffizienten.
         * \return optimierte Koeffizienten
         *
         * Überprüft, ob bereits optimiert wurde, falls ja Weiterleitung an
         * SimulationWorker::getOptimizedCoeffs.
         * \see getOptimzed, optimize, SimulationWorker::getOptimizedCoeffs
         */
        QVector<double> getOptimizedCoeffs() const;
        /*!
         * \brief Abfrage-Funktion, ob der manuelle Anfangswert für die Optimierung
         * aktiviert wurde.
         * \return true/false für manueller Anfangswert aktiviert
         * \see setOverrideThermalDiffusivities, optimize
         */
        bool getOverrideThermalDiffusivities() const;
        /*!
         * \brief Getter-Funktion für den manuellen Anfangswert für die Optimierung.
         * \return aktueller manueller Anfangswert
         * \see setOverrideValue, optimize, getOverrideThermalDiffusivities
         */
        double getOverrideValue() const;
        /*!
         * \brief Getter-Funktion für das Ergebnis der letzten Simulation.
         * \return das Ergebnis der letzten Simulation
         *
         * Überprüft, ob ein gültiges Simulationsergebnis vorliegt, falls ja Weiterleitung
         * an SimulationWorker::getResult. Ein gültiges Ergebnis liegt vor, falls
         * mindestens eine Simulation erfolgreich durchgeführt wurde und danach keine
         * Simulation angefangen und abgebrochen wurde.
         * \see simulate, getSimulated, SimulationWorker::getResult
         */
        double*** getResult() const;
        /*!
         * \brief Getter-Funktion für die Zeitdiskretisierungsgröße m der letzten
         * Simulation.
         * \return die Zeitdiskretisierungsgröße der letzten Simulation
         *
         * Überprüft, ob ein gültiges Simulationsergebnis vorliegt, falls ja Weiterleitung
         * an SimulationWorker::getM. Ein gültiges Ergebnis liegt vor, falls
         * mindestens eine Simulation erfolgreich durchgeführt wurde und danach keine
         * Simulation angefangen und abgebrochen wurde.
         * \see simulate, getSimulated, SimulationWorker::getM
         */
        long getResultM() const;
        /*!
         * \brief Getter-Funktion für die Ortsdiskretisierungsgröße n der letzten
         * Simulation.
         * \return die Ortsdiskretisierungsgröße der letzten Simulation
         *
         * Überprüft, ob ein gültiges Simulationsergebnis vorliegt, falls ja Weiterleitung
         * an SimulationWorker::getN. Ein gültiges Ergebnis liegt vor, falls
         * mindestens eine Simulation erfolgreich durchgeführt wurde und danach keine
         * Simulation angefangen und abgebrochen wurde.
         * \see simulate, getSimulated, SimulationWorker::getN
         */
        long getResultN() const;
        /*!
         * \brief Getter-Funktion für den Endzeitpunkt T der letzten
         * Simulation.
         * \return der Endzeitpunkt der letzten Simulation
         *
         * Überprüft, ob ein gültiges Simulationsergebnis vorliegt, falls ja Weiterleitung
         * an SimulationWorker::getT. Ein gültiges Ergebnis liegt vor, falls
         * mindestens eine Simulation erfolgreich durchgeführt wurde und danach keine
         * Simulation angefangen und abgebrochen wurde.
         * \see simulate, getSimulated, SimulationWorker::getT
         */
        double getResultT() const;

        /*!
         * \brief Abfrage-Funktion, ob ein gültiges Simulationsergebnis vorliegt.
         * \return true/falls für gültiges Simulationsergebnis
         *
         * Ein gültiges Ergebnis liegt vor, falls
         * mindestens eine Simulation erfolgreich durchgeführt wurde und danach keine
         * Simulation angefangen und abgebrochen wurde.
         * \see simulate
         */
        bool getSimulated() const;
        /*!
         * \brief Getter-Funktion für die grundlegenden Simulationseinstellungen.
         * \return das SimulationSetup - Objekt
         *
         * Ermöglicht den Zugriff auf die Getter-Funktionen des SimulationSetup - Objektes.
         */
        SimulationSetup * const & getSimulationSetup() const;

        /*!
         * \brief getSolverNames stellt eine Liste der wählbaren LGS Löser zur Verfügung.
         * \return Liste aller wählbaren LGS Löser
         *
         * Weiterleitung an SimulationWorker::getSolverNames.
         * \see SimulationWorker::getSolverNames
         */
        QList<QString> const getSolverNames() const;

        /*!
         * \brief Abfrage-Funktion, ob das Nutzen der Wärmequellen für die Optimierung
         * aktiviert wurde.
         * \return true/false für aktiviertes Nutzen
         * \see setUseHeatSources, optimize
         */
        bool getUseHeatSources() const;

        /*!
         * \brief Abfrage-Funktion, ob das Model gerade mit einer Simulation oder
         * Optimierung oder dem Einlesen von Messdaten beschäftigt ist.
         * \return true/false für beschäftigt
         * \see simulate, optimize, readObservations
         */
        bool isWorking() const;

        /*!
         * \brief loadSetup lädt gespeichert Simulationeinstellungen aus einer Datei ein.
         * \param filename Dateipfad, der gespeicherten Einstellungen
         *
         * Sendet eine Update-Nachricht an das UI.
         * \see saveSetup, presentation::UI::updateNotification
         */
        void loadSetup(QString filename);

        /*!
         * \brief optimize startet eine Optimierung der Temperaturleitkoeffizienten.
         *
         * Informiert das UI über den Beginn der Optimierung.
         * \see SimulationWorker::startOptimizationSlot,
         * presentation::UI::updateNotification
         */
        void optimize();

        /*!
         * \brief readObservations startet das Einlesen von Messdaten.
         * \param filename Dateipfad für die Messdaten
         * \param obsCount Anzahl Messdaten
         *
         * Informiert das UI über den Beginn des Einlesens.
         * \see SimulationWorker::startReadingDataSlot,
         * presentation::UI::updateNotification
         */
        void readObservations(QString const filename, long const obsCount);

        /*!
         * \brief removeLastArea entfernt das zuletzt hinzugefügte Gebiet aus den
         * Simulationseinstellungen und informiert das UI.
         * \param type Typ des zu löschenden Gebietes
         * \see SimulationSetup::removeLastArea, presentation::UI::updateNotification
         */
        void removeLastArea(SimulationSetup::AreaType type);
        /*!
         * \brief reorderArea verändert die Reihenfolge der Gebiete und informiert das UI.
         * \param pos Position des zu verschiebenden Gebietes
         * \param dir Richtung, in die das Gebiet verschoben wird
         * \param type Typ der Gebiete, deren Reihenfolge geändert wird
         * \see SimulationSetup::reorderAreas, presentation::UI::updateNotification
         */
        void reorderArea(int const pos, int const dir,
                         model::SimulationSetup::AreaType type);

        /*!
         * \brief resetSetup setzt die Simulationseinstellungen auf die Standardwerte
         * zurück.
         *
         * Sendet eine Update-Nachricht an das UI.
         * \see presentation::UI::updateNotification
         */
        void resetSetup();

        /*!
         * \brief saveSetup speichert die aktuellen Simulationseinstellungen.
         * \param filename Dateipfad, an dem die Einstellungen gespeichert werden
         *
         * Sendet eine Update-Nachricht an das UI.
         * \see loadSetup, presentation::UI::updateNotification
         */
        void saveSetup(QString filename);

        /*!
         * \brief selectIntMethod setzt die Intgrationsmethode und informiert das UI.
         * \param newIntMethod neue Integrationsmethode
         * \see SimulationSetup::selectIntMethod, presentation::UI::updateNotification
         */
        void selectIntMethod(QString const newIntMethod);
        /*!
         * \brief selectSolver setzt den LGS Löser und informiert das UI.
         * \param newSolver neuer LGS Löser
         * \see SimulationSetup::selectSolver, presentation::UI::updateNotification
         */
        void selectSolver(QString const newSolver);
        /*!
         * \brief setAreaBackgroundValue setzt den Hintergrund für einen Gebietstyp und
         * informiert das UI.
         * \param newValue neuer Hintergrundwert
         * \param type Typ der Gebiete, deren Hintergrundwert geänder wird
         * \see SimulationSetup::setAreaBackgroundValue,
         * presentation::UI::updateNotification
         */
        void setAreaBackgroundValue(double const newValue, SimulationSetup::AreaType type);
        /*!
         * \brief setIBV setzt einen Rand- oder den Anfangswert und informiert das UI.
         * \param newValue neuer Rand bzw. Anfangswert
         * \param ibv Entscheidet welcher Wert geändert wird
         * \see SimulationSetup::setIBV, presentation::UI::updateNotification
         */
        void setIBV(double const newValue, SimulationSetup::IBV ibv);
        /*!
         * \brief setM setzt die Zeitdiskretisierungsgröße m und informiert das UI.
         * \param newM neue Zeitdiskretisierungsgröße
         * \see SimulationSetup::setM, presentation::UI::updateNotification
         */
        void setM(int const newM);
        /*!
         * \brief setN setzt die Ortsdiskretisierungsgröße n und informiert das UI.
         * \param newN neue Ortsdiskretisierungsgröße
         * \see SimulationSetup::setN, presentation::UI::updateNotification
         */
        void setN(int const newN);
        /*!
         * \brief setOverrideThermalDiffusivities aktiviert bzw. deaktiviert den manuellen
         * Anfangswert für die Optimierung und informiert das UI.
         * \param override true/false für aktivieren/deaktivieren
         * \see getOverrideThermalDiffusivities, getOverrideValue, optimize
         */
        void setOverrideThermalDiffusivities(bool const override);
        /*!
         * \brief setOverrideValue setzt den manuellen Anfangswert für die Optimierung und
         * informiert das UI.
         * \param newValue neuer manueller Anfangswert
         * \see getOverrideValue, getOverrideThermalDiffusivities,
         * presentation::UI::updateNotification
         */
        void setOverrideValue(double const newValue);
        /*!
         * \brief setSolverMaxError setzt die Fehlertoleranz für den LGS Löser und
         * informiert das UI.
         * \param newMaxError neue Fehlertoleranz
         * \see SimulationSetup::setSolverMaxError, presentation::UI::updateNotification
         */
        void setSolverMaxError(double const newMaxError);
        /*!
         * \brief setSolverMaxIt setzt die maximale Iterationsanzahl für den LGS Löser und
         * informiert das UI.
         * \param newMaxIt neue maximale Iterationsanzahö
         * \see SimulationSetup::setSolverMaxIt, presentation::UI::updateNotification
         */
        void setSolverMaxIt(int const newMaxIt);
        /*!
         * \brief setT setzt den Endzeitpunkt T und informiert das UI.
         * \param newT neuer Endzeitpunkt
         * \see SimulationSetup::setT, presentation::UI::updateNotification
         */
        void setT(double const newT);
        /*!
         * \brief setUI setzt die Referenz auf das UI und verbindet Signale und Slots.
         * \param ui das UI
         * \see presentation::UI
         */
        void setUI(presentation::UI * ui);
        /*!
         * \brief setUseHeatSources aktiviert bzw. deaktiviert das Nutzen der Wärmequellen
         * für die Optimierung und informiert das UI
         * \param useHeatSources true/false für aktivieren/deaktivieren
         * \see getUseHeatSources, presentation::UI::updateNotification
         */
        void setUseHeatSources(bool const useHeatSources);

        /*!
         * \brief simulate startet eine Simulation der Wärmeleitungsgleichung.
         *
         * Informiert das UI über den Beginn der Simulation.
         * \see SimulationWorker::startSimulationSlot,
         * presentation::UI::updateNotification
         */
        void simulate();

        /*!
         * \brief updateAreaValue ändert den Wert eines Gebietes und informiert das UI.
         * \param pos Position des zu veränderen Gebietes
         * \param newValue neuer Wert des Gebietes
         * \param type Typ des zu veränderen Gebietes
         * \see SimulationSetup::updateAreaValue, presentation::UI::updateNotification
         */
        void updateAreaValue(int const pos, double const newValue,
                             SimulationSetup::AreaType type);

    signals:
        /*!
         * \brief startOptimization wird von optimize ausgesendet und lässt das
         * SimulationWorker - Objekt eine Optimierung der Temperaturleitkoeffizienten
         * durchführen.
         * \param simSetup Simulationseinstellungen für die Optimierung
         * \param overrideTD true/false für manueller Anfangswert aktiviert
         * \param overrideValue manueller Anfangswert
         * \param useHeatSources true/false für aktiviertes Nutzen
         * \see optimize, setOverrideThermalDiffusivities, setUseHeatSources,
         * SimulationWorker::startOptimizationSlot
         */
        void startOptimization(SimulationSetup * simSetup, bool overrideTD,
                               double overrideValue, bool useHeatSources);
        /*!
         * \brief startReadingData wird von readObservations ausgesendet und lässt das
         * SimulationWorker - Objekt Messdaten einlesen.
         * \param filename Dateipfad der Messdaten
         * \param obsCount Anzahl der Messdaten
         * \see readObservations, SimulationWorker::startReadingDataSlot
         */
        void startReadingData(QString const filename, long const obsCount);
        /*!
         * \brief startSimulation wird von simulate ausgesendet und lässt das
         * SimulationWorker - Objekt eine Simulation der Wärmeleitungsgleichung
         * durchführen.
         * \param simSetup die Einstellungen für die Simulation
         * \see simulate, SimulationWorker::startSimulationSlot
         */
        void startSimulation(SimulationSetup * simSetup);

    private slots:
        void startedWorkSlot();
        void finishedOptimizationSlot(bool success);
        void finishedReadingDataSlot();
        void finishedSimulationSlot(bool success);

    //Attribute:
    private:
        bool blocking;
        bool dataRead;
        bool optimized;
        bool overrideDiffusivities;
        double overrideInitialTDvalue;
        SimulationSetup * simSetup;
        bool simulated;
        SimulationWorker * simWorker;
        presentation::UI * ui;
        bool useHeatSources;
        QThread workerThread;
        bool working;
    };

}

#endif // MODEL_H
