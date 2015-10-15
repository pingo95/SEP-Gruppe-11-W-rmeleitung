#ifndef OPTIMIZATIONWIDGET_H
#define OPTIMIZATIONWIDGET_H

#include <QTabWidget>

#include "../model/Model.h"

#include <QCheckBox>
#include "Qcustomplot.h"
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLineEdit>


namespace presentation {
class Controller;
    /*!
     * \brief Die Klasse OptimizationWidget implementiert den Tab zum Durchführen einer
     * Optimierung.
     *
     * Der Tab bietet Knöpfe zum Starten einer Optimierung und zum Einlesen von Messdaten.
     * Anzeigen für eingelesene Daten, aus dem Simulations-Tab übernommene
     * Simulationseinstellungen und für die Optimierungsergebnisse. Außerdem gibt es auch
     * Eingabe/Aktivierungsmöglichkeiten für die manuellen Anfangswert und das Nutzen der
     * Wärmequellen-Gebiete für die Optimierung.
     */
    class OptimizationWidget : public QTabWidget
    {
        Q_OBJECT

    //Funktionen:
    public:
        /*!
         * \brief Konstruktor für die Klasse OptimizationWidget.
         * \param parent parent-Widget
         */
        explicit OptimizationWidget(QWidget *parent = 0);
//        ~OptimizationWidget();

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
         * \see model::Model
         */
        void setModel(model::Model * model);

        /*!
         * \brief update aktualisiert den Tab mit den aktuellen Werten aus dem Modell.
         * \see UI::updateNotification
         */
        void update();

        // Slots in spe
        /*!
         * \brief nextMainStage stellt den Fortschrittsbalken für einen neuen
         * Optimierungsabschnitt ein.
         * \param stage Name des neuen Abschnittes
         * \param maximum Anzahl Teilschritte des neuen Abschnittes
         * \see UI::nextOptimizationStageSlot
         */
        void nextMainStage(QString stage, int maximum);
        /*!
         * \brief nextSubStage stellt den Fortschrittsbalken für einen neuen
         * Simulationsabschnitt während eines Optimierungsschrittes ein.
         * \param stage Name des neuen Abschnittes
         * \param maximum Anzahl Teilschritte des neuen Abschnittes
         * \see UI::nextStageSlot
         */
        void nextSubStage(QString stage, int maximum);
        /*!
         * \brief updateMainrogress aktualisiert den Fortschritt der Optimierung auf dem
         * Fortschrittsbalken.
         * \param step Teilschritt, der abgeschlossen wurde
         * \see UI::updateOptimizationProgressSlot
         */
        void updateMainProgress(int step);
        /*!
         * \brief updateProgress aktualisiert den Fortschritt der Simulation während
         * eines Optimierungsschrittes auf dem Fortschrittsbalken.
         * \param step Teilschritt, der abgeschlossen wurde
         * \see UI::updateProgressSlot
         */
        void updateSubProgress(int step);

    signals:
        /*!
         * \brief newInitialValue wird ausgesendet, wenn der manuelle Anfangswert geändert
         * werden soll, das Signal der QSpinBox wird aus Darstellungsgründen
         * dabei tranformiert.
         * \param shiftedValue neuer transformierter Wert
         */
        void newInitialValue(double shiftedValue);
        /*!
         * \brief subTabChange wird ausgesendet, wenn einer der untergeordneten Tabs
         * geändert wird.
         * \param targetTab neuer aktiver Tab
         * \see Controller::tabChangedSlot
         */
        void subTabChange(int targetTab);

    private slots:
       void transformTabIDSlot(int targetTab);
       void shiftInitialValueSlot(double newValue);

    private:
        explicit OptimizationWidget();

    //Attribute
    private:
        enum SubTab{
            TabConfiguration = 0,
            TabSolution = 1
        };

        int activeSubTab;
        Controller * controller;
        model::Model * model;
        bool updating;
        double const valueShift;

        // Config Tab
        QWidget * configurationTab;

        QGridLayout * configurationTabLayout;

        QLabel * topLabelConfiguration;

        QPushButton * loadDataButton;
        QPushButton * startOptimizationButton;

        QGroupBox * boxOverride;
        QGridLayout * boxOverrideLayout;
        QCheckBox * overrideHeatSources;
        QCheckBox * overrideThermalDiffusivities;
        QLabel * labelInitialValue;
        QDoubleSpinBox * inputInitialValue;

        QGroupBox * boxSettings;
        QGridLayout * boxSettingsLayout;
        QLabel * labelSettings;
        QLabel * labelIntMethod;
        QLineEdit * displayIntMethod;
        QLabel * labelM;
        QSpinBox * displayM;
        QLabel * labelT;
        QDoubleSpinBox * displayT;
        QLabel * labelSolver;
        QLineEdit * displaySolver;
        QLabel * labelMaxError;
        QDoubleSpinBox * displayMaxError;
        QLabel * labelMaxIt;
        QSpinBox * displayMaxIt;

        QCustomPlot * plate;
        QCPColorMap * colorMap;
        QCPColorScale * colorScale;

        QLabel * labelData;
        QTableWidget * dataTable;

        QLabel * labelMainProgressBar;
        QProgressBar * mainProgressBar;
        QLabel * labelSubProgressBar;
        QProgressBar * subProgressBar;

        QSpacerItem * spacerItem;

        // Solution Tab
        QWidget * solutionTab;
        QVBoxLayout * solutionTabLayout;
        QLabel * topLabelSolution;
        QTableWidget * solutionTable;
    };

}


#endif // OPTIMIZATIONWIDGET_H
