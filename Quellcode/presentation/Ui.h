#ifndef UI_H
#define UI_H

#include <QMainWindow>

#include <QTabWidget>

#include "Areawidget.h"
#include "Ibvswidget.h"
#include "Simulationwidget.h"
#include "Visualizationwidget.h"
#include "Optimizationwidget.h"
#include "Helpwidget.h"

namespace presentation {
class Controller;
    /*!
     * \brief Die Klasse UI implementiert das User Interface des MVC.
     *
     * Das UI besteht dabei aus folgenden Tabs:
     *      - einem Tab zur Eingabe der Temperaturleitkoeffizienten
     *      - einem Tab zur Eingabe der Wärmequellen
     *      - einem Tab zur Eingabe der IBVs
     *      - einem Tab zum Durchführen einer Simulation
     *      - einem Tab zum Visualisieren der Ergebnisse
     *      - einem Tab zum Durchführen einer Optimierung
     *      - einem Tab zum Anzeigen der Hilfe
     */
    class UI : public QMainWindow
    {
        Q_OBJECT

    //Enum
    public:
        /*!
         * \brief Der Enum ActiveTab dient zur Unterscheidung der Tabs des UI.
         *
         * Insbesondere wird dieser Enum dazu benutzt in updateNotification zu entscheiden,
         * welcher Tab aktualisiert werden muss.
         */
        enum ActiveTab
        {
            /*! Konfigurations-Tab */
            TabConfiguration = 0,
            /*! Temperaturleitkoeffizienten-Tab */
            TabThermalDiffusivities = 5,
            /*! Wärmequellen-Tab */
            TabHeatSources = 6,
            /*! IBVs-Tab */
            TabIBVs = 7,
            /*! Simulations-Tab */
            TabSimulating = 1,
            /*! Visualisierungs-Tab */
            TabVisualization = 2,
            /*! Optimierungs-Tab */
            TabParameterFitting = 3,
            /*! Hilfe-Tab */
            TabHelp = 4
        };

    //Funktionen:
    public:
        /*!
         * \brief Konstruktor für die Klasse UI.
         * \param parent parent-Widget
         */
        UI(QWidget *parent = 0);
        /*!
         * \brief Destruktor für die Klasse UI.
         */
        ~UI();

        /*!
         * \brief drawPartialArea leitet das Zeichnen eines angefangenen Gebietes an den
         * entsprechenden Tab weiter.
         * \param partialAreaX x-Koordinaten der Punkte des Gebietes
         * \param partialAreaY y-Koordinaten der Punkte des Gebietes
         * \param type entscheidet an welchen Tab weitergeleitet wird
         * \see AreaWidget::drawPartialArea, Controller::areaClickSlot
         */
        void drawPartialArea(QVector<double> const & partialAreaX,
                             QVector<double> const & partialAreaY,
                             model::SimulationSetup::AreaType type);

        /*!
         * \brief Weiterleitung an den Visualisierungs-Tab
         * \return die aktuelle Position des Schiebereglers
         * \see VisualizationWidget::getInitialFrame
         */
        int getInitialFrame() const;

        /*!
         * \brief revertTabChange macht einen Tab-Wechsel rückgängig.
         * \param targetTab Tab, zu dem zurückgewechselt wird
         *
         * Diese Funktion wird aufgerufen, falls versucht wurde, einen Gebiet-Eingabe-Tab
         * zu verlassen, während ein neues Gebiet angefangen wurde.
         * \see updateNotification, Controller::tabChangedSlot
         */
        void revertTabChange(UI::ActiveTab targetTab);
        /*!
         * \brief setActiveTab ändert den aktiven und damit sichtbaren Tab und erzwingt ein
         * Update des UI.
         * \param targetTab neuer aktiver Tab
         * \see updateNotification, Controller::tabChangedSlot
         */
        void setActiveTab(int targetTab);

        /*!
         * \brief setController setzt die Referenz auf den Controller und verbindet
         * Signale und Slots.
         * \param controller der Controller
         *
         * Ruft auch die setController-Funktionen der einzelnen Tab-Widgets auf.
         * \see Controller
         */
        void setController(Controller * controller);
        /*!
         * \brief setModel setzt die Referenz auf das Modell.
         * \param model das Modell
         *
         * Ruft auch die setModel-Funktionen der einzelnen Tab-Widgets auf und erzwingt ein
         * initiales Update des UI.
         * \see model::Model
         */
        void setModel(model::Model * model);

        /*!
         * \brief updateNotification ist die Benachrichtigungs-Funktion des Beobachter
         * Entwurfsmusters.
         *
         * Diese Funktion wird vom Modell aufgerufen, wenn sich etwas an dessen Zustand
         * ändert, oder vom Controller, wenn fehlerhafte Benutzer-Eingaben rückgängig
         * gemacht werden sollen.
         *
         * Mit Hilfe des ActiveTab - Enums wird dabei unterschieden welcher Tab
         * aktualisiert werden muss.
         * \see AreaWidget::update, IBVsWidget::update, OptimizationWidget::update,
         * SimulationWidget::update, VisualizationWidget::update
         */
        void updateNotification();

        /*!
         * \brief Weiterleitung an den Visualisierungs-Tab.
         * \param frame Zustand, der dargestellt wird
         * \see VisualizationWidget::visualizeState
         */
        void visualizeState(int const frame);

    signals:
        /*!
         * \brief subTabChange wird ausgesendet, wenn einer der untergeordneten Tabs
         * geändert wird.
         * \param targetTab neuer aktiver Tab
         * \see Controller::tabChangedSlot
         */
        void subTabChange(int targetTab);

    public slots:
        // Weiterleitungen
        /*!
         * \brief Weiterleitung an den Simulations-Tab
         * \param text Text, der hinzugefügt wird
         * \see SimulationWidget::appendToSimulationLog
         */
        void appendToSimulationLogSlot(QString text);
        /*!
         * \brief Weiterleitung an den Simulations- bzw. Optimierungs-Tab.
         * \param stage Name des neuen Abschnittes
         * \param maximum Anzahl Teilschritte des neuen Abschnittes
         * \param simulation true/false, ob Simulations- oder Optimierungs-Tab
         * \see SimulationWidget::nextStage, OptimizationWidget::nextStage
         */
        void nextStageSlot(QString stage, int maximum, bool simulation);
        /*!
         * \brief Weiterleitung an den Simulations- bzw. Optimierungs-Tab.
         * \param step Teilschritt, der abgeschlossen wurde
         * \param simulation true/false, ob Simulations- oder Optimierungs-Tab
         * \see SimulationWidget::updateProgress, OptimizationWidget::updateProgress
         */
        void updateProgressSlot(int step, bool simulation);

    private slots:
        void transformTabIDSlot(int targetTab);

    //Attribute:
    private:
        int activeTab;
        Controller * controller;
        model::Model * model;
        int const tabMainCount;

        //Qt Elemente:
        QTabWidget * tabWidgetMain;
        QTabWidget * tabWidgetSub;

        AreaWidget * thermalDiffusivitiesTab;
        AreaWidget * heatSourcesTab;
        IBVsWidget * iBVsTab;
        SimulationWidget * simulationTab;
        OptimizationWidget * optimizationTab;
        VisualizationWidget * visualizationTab;
        HelpWidget * helpTab;


    };
}

#endif // UI_H
