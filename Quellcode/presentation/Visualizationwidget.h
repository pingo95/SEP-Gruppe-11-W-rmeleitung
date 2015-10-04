#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <QWidget>

#include "../model/Model.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include "Qcustomplot.h"
#include <QSlider>
#include <QLCDNumber>

namespace presentation {
class Controller;
    /*!
     * \brief Die Klasse VisualizationWidget implementiert den Tab zur Visualisierung der
     * Simulationsergebnisse.
     *
     * Der Tab besteht u.a. aus einem QCustomPlot mit einer QCPColorMap und einer
     * QCPColorScale zum Visualisieren der Simulationsergebnisse, einem Knopf zum Starten
     * des Videos und einem Schieberegler zum Darstellen von Einzelbildern.
     */
    class VisualizationWidget : public QWidget
    {
        Q_OBJECT

    //Funktionen:
    public:
        /*!
         * \brief Konstruktor für die Klasse VisualizationWidget.
         * \param parent parent-Widget
         */
        explicit VisualizationWidget(QWidget *parent = 0);

        /*!
         * \brief Getter-Funktion für die aktuelle Position des Schiebereglers.
         * \return die aktuelle Position
         *
         * Die Position des Schiebereglers entspricht dem Zeitschritt der letzten
         * Simulation, der als Einzelbild visualisiert wird und beim Abspielen des Videos
         * der Startzustand ist.
         * \see visualizeState, Controller::visualizeStateSlot, Controller::playVideoSlot
         */
        int getInitialFrame() const;

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
         *
         * Insbesondere werden der Schieberegler und der Video-abspiel-Knopf freigeschaltet
         * bzw. gesperrt abhängig davon, ob ein gültiges Simulationsergebnis vorliegt.
         * \see UI::updateNotification
         */
        void update();

        /*!
         * \brief visualizeState visualisiert einen einzelnen Zeitpunkt einer Simulation.
         * \param frame Zustand, der visualisiert wird.
         * \see getInitialFrame, Controller::visualizeStateSlot, Controller::playVideoSlot
         */
        void visualizeState(int const frame);

    private slots:
        void updateLcdSlot(int value);

    private:
        explicit VisualizationWidget();
//        ~VisualizationWidget();

    //Attribute:
    private:
        Controller * controller;
        model::Model * model;
        double *** result;
        int resultM;
        int resultN;
        double resultT;
        bool updating;

        QGridLayout * layout;

        QLabel * topLabel;

        QPushButton * playButton;

        QCustomPlot * plate;
        QCPColorScale * colorScale;
        QCPColorMap * colorMap;

        QSlider * slider;
        QLCDNumber * lcdTimestep;
    };

}

#endif // VISUALIZATIONWIDGET_H
