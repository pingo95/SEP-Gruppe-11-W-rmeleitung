#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../model/Model.h"
#include "Ui.h"
#include <QObject>
#include <QInputDialog>
#include <QMessageBox>

namespace presentation {
    /*!
     * \brief Die Klasse Controller implementiert den Controller des MVC.
     *
     * Insbesondere implementiert diese Klasse Slots, die die Benutzereingaben verarbeiten,
     * d.h. u.a. ggf. Werte auf Gültigkeit überprüfen, und diese dann entweder an das Modell
     * weiterleitet oder eine Update-Nachricht an das UI sendet, sodass dieses durch ein
     * Update (fehlerhafte) Eingaben rückgängig macht.
     */
    class Controller : public QObject
    {
        Q_OBJECT

    //Funktionen:
    public:
        /*!
         * \brief Konstruktor für die Klasse Controller.
         * \param parent parent-Widget
         */
        Controller(QObject * parent = 0);
        /*!
         * \brief Destruktor für die Klasse Controller.
         */
        ~Controller();

        /*!
         * \brief Getter-Funktion für angefangene, unfertige Gebiete.
         * \param xKoords x-Koordinaten der Punkte des unfertigen Gebietes
         * \param yKoords y-Koordinaten der Punkte des unfertigen Gebietes
         *
         * Neue, angefangene Gebiete werden vom Controller verwaltet bis sie entweder
         * abgebrochen/verworfen oder abgeschlossen und im Modell gespeichert werden.
         *
         * Diese Abfrage-Funktion wird von der update-Funktion der Gebiete-Tabs benötigt
         * für den seltenen Fall, dass während des (vom Controller verwaltetem) Erstellen
         * eines neuen Gebietes eine Update-Nachricht (vom Modell) aufgrund einer
         * abgeschlossenen Simulation gesendet wird.
         * \see areaClickSlot, AreaWidget::update
         */
        void getPartialArea(QVector<double> & xKoords,
                            QVector<double> & yKoords) const;
        /*!
         * \brief Abfrage-Funktion, ob es aktuell möglich ist das Löschen eines Punktes
         * rückgängig zu machen.
         * \param type Gebiet-Typ, für den abgefragt wird
         * \return true/false für Aktion wiederherstellen möglich
         *
         * Eine Aktion wiederherzustellen ist möglich, wenn zuvor ein neues Gebiet
         * angefangen wurde, dann ein oder mehrere Punkte des neuen Gebietes rückgängig
         * gemacht wurden und seitdem kein neuer anderer Punkt hinzugefügt wurde.
         * \see redoSlot, undoSlot
         */
        bool getRedoPossible(model::SimulationSetup::AreaType type) const;

        /*!
         * \brief setModel setzt die Referenz auf das Modell.
         * \param model das Modell
         * \see model::Model
         */
        void setModel(model::Model * model);
        /*!
         * \brief setUI setzt die Referenz auf das UI.
         * \param ui das UI
         * \see presentation::UI
         */
        void setUI(UI * ui);


    public slots:
        /*!
         * \brief Slot zum Abbrechen einer laufenden Simulation oder Optimierung.
         * \see model::Model::abortWork
         */
        void abortWorkSlot();
        /*!
         * \brief Slot zum Erstellen eines Gebietes.
         * \param xKoord x-Koordinate des Klicks
         * \param yKoord y-Koordinate des Klicks
         * \param plateSize aktuelle Größe der Gebiet-Platte
         * \param valueShift Werte-Shift zur Visualisierung der Gebietswerte
         * \param mouseClick true/false für Mausklick oder per Tastatur
         * \param type Gebietstyp, für den Aktion durchgeführt wird
         * \see model::Model::addNewArea
         */
        void areaClickSlot(double xKoord, double yKoord, QSize plateSize,
                           double valueShift, bool mouseClick,
                           model::SimulationSetup::AreaType type);
        /*!
         * \brief Slot zum Ändern des Wertes eines Gebietes.
         * \param pos Position des Gebietes, dessen Wert geändert werden soll
         * \param newValue neuer Wert
         * \param type Gebietstyp, für den Aktion durchgeführt wird
         * \see model::Model::updateAreaValue
         */
        void areaValueChangedSlot(int pos, double newValue,
                                  model::SimulationSetup::AreaType type);
        /*!
         * \brief Slot zum Übernehmen von Temperaturleitkoeffizienten.
         *
         * Wenn zum Zeitpunkt des Aufrufs keine Simulation oder Optimierung
         * durchgeführt wird und bereits eine Optimierung erfolgreich
         * abgeschlossen wurde, werden die resultierenden Temperaturleit
         * koeffizienten in Gebiete umgewandelt, die dann für Simulationen
         * genutzt werden können.
         * \see model::Model::applyFitted
         */
        void applyFittedCoefficientsSlot();
        /*!
         * \brief Slot zum Löschen aller Gebiete.
         * \param type Gebietstyp, für den Aktion durchgeführt wird
         * \see model::Model::removeLastArea
         */
        void clearAreasSlot(model::SimulationSetup::AreaType type);
        /*!
         * \brief Slot zum Löschen eines Gebietes.
         * \param pos Position des Gebietes, das gelöscht werden soll
         * \param type Gebietstyp, für den Aktion durchgeführt wird
         * \see model::Model::removeLastArea
         */
        void deleteAreaSlot(int pos, model::SimulationSetup::AreaType type);
        /*!
         * \brief Slot zum Abbrechen des Erstellens eines neuen Gebietes.
         * \param type Gebietstyp, für den Aktion durchgeführt wird
         */
        void discardAreaSlot(model::SimulationSetup::AreaType type);
        /*!
         * \brief Slot zum Starten des Einlsens von Messdaten für die Optimierung.
         * \see model::Model::readObservations
         */
        void loadObservationsSlot();
        /*!
         * \brief Slot zum Laden von gespeicherten Simulationseinstellungen.
         * \see model::Model::loadSetup
         */
        void loadSimulationSetupSlot();
        /*!
         * \brief Slot zum Ändern eines Rand- oder des Anfangswertes.
         * \param newValue neuer Wert
         * \param ibv entscheidet, welcher Wert geändert wird
         * \see model::Model::setIBV
         */
        void newIBVValueSlot(double newValue, model::SimulationSetup::IBV ibv);
        /*!
         * \brief Slot zum Ändern der Fehlertoleranz für den LGS Löser.
         * \param newMaxError neuer Wert
         * \see model::Model::setSolverMaxError
         */
        void newMaxErrorSlot(double newMaxError);
        /*!
         * \brief Slot zum Ändern der maximalen Iterationsanzahl für den LGS Löser.
         * \param newMaxIt neuer Wert
         * \see model::Model::setSolverMaxIt
         */
        void newMaxItSlot(int newMaxIt);
        /*!
         * \brief Slot zum Ändern der Zeitdiskretisierungsgröße für die Simulation.
         * \param newM neuer Wert
         * \see model::Model::setM
         */
        void newMSlot(int newM);
        /*!
         * \brief Slot zum Ändern der Ortsdiskretisierungsgröße für die Simulation.
         * \param newN neuer Wert
         * \see model::Model::setN
         */
        void newNSlot(int newN);
        /*!
         * \brief Slot zum Ändern des manuellen Anfangswertes für die Optimierung.
         * \param newValue neuer Wert
         * \see model::Model::setOverrideValue
         */
        void newOverrideValueSlot(double newValue);
        /*!
         * \brief Slot zum Ändern des Endzeitpunktes für die Simulation.
         * \param newT neuer Wert
         * \see model::Model::setT
         */
        void newTSlot(double newT);
        /*!
         * \brief Slot zum Starten einer Optimierung der Temperaturleitkoeffizienten.
         * \see model::Model::optimize
         */
        void optimizationSlot();
        /*!
         * \brief Slot zum Aktivieren/Deaktivieren des manuellen Anfangswertes für die
         * nächste Optimierung.
         * \param override true/false für Aktivieren/Deaktivieren
         * \see model::Model::setOverrideThermalDiffusivities
         */
        void overrideThermalDiffusivitiesSlot(bool override);
        /*!
         * \brief Slot zum Abspielen des Videos, d.h. zum Visualisieren des letzten
         * Simulationsergebnisses.
         */
        void playVideoSlot();
        /*!
         * \brief redoSlot
         * \param type Gebietstyp, für den Aktion durchgeführt wird
         */
        void redoSlot(model::SimulationSetup::AreaType type);/*!
         * \brief Slot zum Ändern der Reihenfolge der Gebiete.
         * \param pos Position des Gebietes, das verschoben werden soll
         * \param dir Richtung, in die das Gebiet verschoben werden soll
         * \param type Gebietstyp, für den Aktion durchgeführt wird
         * \see model::Model::reorderArea
         */
        void reorderAreaSlot(int pos, int dir, model::SimulationSetup::AreaType type);
        /*!
         * \brief Slot zum Zurücksetzen der Simulationseinstellungen.
         * \see model::Model::resetSetup
         */
        void resetSimulationSetupSlot();
        /*!
         * \brief Slot zum Speichern der aktuellen Simulationseinstellungen.
         * \see model::Model::saveSetup
         */
        void saveSimulationSetupSlot();
        /*!
         * \brief Slot zum Ändern der Integrationsmethode.
         * \param newIntMethod neue Integrationsmethode
         * \see model::Model::selectIntMethod
         */
        void selectIntMethodSlot(QString newIntMethod);
        /*!
         * \brief Slot zum Ändern des LGS Lösers.
         * \param newIterativeSolver neuer LGS Löser
         * \see model::Model::selectSolver
         */
        void selectSolverSlot(QString newIterativeSolver);
        /*!
         * \brief Slot zum Starten einer Simulation der Wärmeleitungsgleichung.
         * \see model::Model::simulate
         */
        void simulateSlot();
        /*!
         * \brief Slot zum Ändern des aktiven und damit sichtbaren Tabs.
         * \param newTab neuer aktiver Tab
         *
         * Überpürft insbsondere, ob  versucht wird ein Gebiet-Tab vorzeitig, d.h. während
         * dem Erstellen eines neuen Gebietes, zu verlassen.
         * \see UI::setActiveTab
         */
        void tabChangedSlot(int newTab);
        /*!
         * \brief Slot zum Löschen des zuletzt hinzugefügten Punktes eines neuen Gebietes.
         * \param type Gebietstyp, für den Aktion durchgeführt wird
         */
        void undoSlot(model::SimulationSetup::AreaType type);
        /*!
         * \brief Slot zum Aktivieren/Deaktivieren der Wärmequellen-Gebiete für die
         * nächste Optimierung.
         * \param use true/false für Aktivieren/Deaktivieren
         * \see model::Model::setUseHeatSources
         */
        void useHeatSourcesSlot(bool use);
        /*!
         * \brief Slot zum Darstellen eines einzelnen Zeitpunktes einer vorausgegangenen
         * Simulation.
         * \see UI::visualizeState
         */
        void visualizeStateSlot();

    private:
        void clearRedo();

    //Attribute:
    private:
        bool loopBack;
        model::Model * model;
        QVector<double> partialAreaX;
        QVector<double> partialAreaY;
        bool redoPossible[2];
        QList<double> redoXStack;
        QList<double> redoYStack;
        bool started[2];
        UI * ui;
        QInputDialog * userInput;
        QMessageBox * errorMessages;
    };

}

#endif // CONTROLLER_H
