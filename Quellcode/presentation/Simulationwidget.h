#ifndef SIMULATIONWIDGET_H
#define SIMULATIONWIDGET_H

#include <QWidget>

#include "../model/Model.h"

#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressBar>

namespace presentation {
class Controller;
    /*!
     * \brief Die Klasse SimulationWidget implementiert den Tab zum Durchführen einer
     * Simulation.
     *
     * Der Tab bietet Eingabemöglichkeiten für die Diskretisierungsgrößen, den Endzeitpunkt
     * der Simulation, für die Wahl der Integrationsmethode und des LGS Lösers und für die
     * Fehlertoleranz und maximale Iterationsanzahl des LGS Lösers. Des Weiteren bietet er
     * Knöpfe zum Starten und Abbrechen einer Simulation und zum Speichern, Laden und
     * Zurücksetzten der Simulationseinstellungen.
     */
    class SimulationWidget : public QWidget
    {
        Q_OBJECT

    //Funktionen:
    public:
        /*!
         * \brief Konstruktor für die Klasse SimulationWidget.
         * \param parent parent-Widget
         */
        explicit SimulationWidget(QWidget *parent = 0);
//        ~SimulationWidget();

        /*!
         * \brief setController setzt die Referenz auf den Controller und verbindet
         * Signale und Slots.
         * \param controller der Controller
         * \see Controller
         */
        void setController(Controller * controller);
        /*!
         * \brief setModel setzt die Referenz auf das Modell.
         * \param model das Modell
         *
         * Lädt außerdem aus dem Modell die Listen der wählbaren Integrationsmethoden
         * und LGS Löser, um die möglichen Benutzereingaben auf diese einzuschränken.
         * \see model::Model, model::Model::getIntMethodsNames,
         * model::Model::getSolverNames
         */
        void setModel(model::Model * model);

        /*!
         * \brief update aktualisiert den Tab mit den aktuellen Werten aus dem Modell.
         * \see UI::updateNotification
         */
        void update();

        // Slots in spe
        /*!
         * \brief appendToSimulationLog fügt den übergebenen Text zum Simulations-Log
         * hinzu.
         * \param text Text, der hinzugefügt wird
         * \see UI::appendToSimulationLogSlot
         */
        void appendToSimulationLog(QString text);
        /*!
         * \brief nextStage stellt den Fortschrittsbalken für einen neuen
         * Simulationsabschnitt ein.
         * \param stage Name des neuen Abschnittes
         * \param maximum Anzahl Teilschritte des neuen Abschnittes
         * \see UI::nextStageSlot
         */
        void nextStage(QString stage, int maximum);
        /*!
         * \brief updateProgress aktualisiert den Fortschritt der Simulation auf dem
         * Fortschrittsbalken.
         * \param step Teilschritt, der abgeschlossen wurde
         * \see UI::updateProgressSlot
         */
        void updateProgress(int step);

    signals:
        /*!
         * \brief newMaxErrorValue wird ausgesendet, wenn die Fehlertoleranz des LGS Lösers
         * angepasst werden soll, das Signal der QSpinBox wird aus Darstellungsgründen
         * dabei tranformiert.
         * \param newValue transformieter Wert
         * \see Controller::newMaxErrorSlot
         */
        void newMaxErrorValue(double newValue);

    private slots:
        void transformMaxErrorSlot(int value);

    private:
        explicit SimulationWidget();

    //Attribute:
    private:
        Controller * controller;
        model::Model * model;
        bool updating;

        QGridLayout * layout;

        QLabel * topLabel;

        QGroupBox * boxIntMethod;
        QGridLayout * boxIntMethodLayout;
        QLabel * labelBoxIntMethod;
        QLabel * labelIntMethod;
        QComboBox * inputIntMethod;
        QLabel * labelM;
        QSpinBox * inputM;
        QLabel * labelT;
        QDoubleSpinBox * inputT;

        QGroupBox * boxSolver;
        QGridLayout * boxSolverLayout;
        QLabel * labelBoxSolver;
        QLabel * labelSolver;
        QComboBox * inputSolver;
        QLabel * labelMaxError;
        QSpinBox * inputMaxError;
        QLabel * labelMaxIt;
        QSpinBox * inputMaxIt;

        QGroupBox * boxSimulate;
        QGridLayout * boxSimulateLayout;
        QLabel * labelBoxSimulate;
        QLabel * labelN;
        QSpinBox * inputN;
        QPushButton * simulateButton;
        QPushButton * abortButton;

        QGroupBox * boxSaveLoad;
        QGridLayout * boxSaveLoadLayout;
        QLabel * labelBoxSaveLoad;
        QPushButton * saveButton;
        QPushButton * loadButton;
        QPushButton* resetButton;

        QTextEdit * simulationLog;

        QLabel * labelProgressBar;
        QProgressBar * progressBar;
    };

}

#endif // SIMULATIONWIDGET_H
