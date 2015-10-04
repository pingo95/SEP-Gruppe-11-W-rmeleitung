#ifndef IBVSWIDGET_H
#define IBVSWIDGET_H

#include <QWidget>

#include "../model/Model.h"

#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpacerItem>

namespace presentation {
class Controller;

    /*!
     * \brief Die Klasse IBVsWidget implementiert den Tab zum Einstellen der Einstellen
     * der Rand- und des Anfangswertes.
     *
     * Der Tab besteht u.a. aus fünf QDoubleSpinBoxen zum Ändern der Werte.
     */
    class IBVsWidget : public QWidget
    {
        Q_OBJECT

    //Funktionen:
    public:
        /*!
         * \brief Konstruktor für die Klasse IBVsWidget.
         * \param parent parent-Widget
         */
        explicit IBVsWidget(QWidget *parent = 0);
//        ~IBVsWidget();

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

    signals:
        /*!
         * \brief iBVsValueChange wird ausgesendet, wenn ein Rand- oder der Anfangswert
         * geändert soll.
         * \param newValue neuer Wert
         * \param ibv entscheidet, welcher Wert geändert wird
         * \see model::SimulationSetup::IBV, Controller::newIBVValueSlot
         */
        void iBVsValueChange(double newValue, model::SimulationSetup::IBV ibv);

    private slots:
        void iBVsMapperSlot(double newValue);

    //Attribute:
    private:
        Controller * controller;
        model::Model * model;
        bool updating;

        QGridLayout * layout;

        QLabel * topLabel;

        // Zahlen im Namen der Boxen für Reihenfolge beim tab-Drücken
        QLabel * labelInitialValue;
        QDoubleSpinBox * input1InitialValue;

        QLabel * labelBottomBoundary;
        QDoubleSpinBox * input2BottomBoundary;

        QLabel * labelLeftBoundary;
        QDoubleSpinBox * input3LeftBoundary;

        QLabel * labelTopBoundary;
        QDoubleSpinBox * input4TopBoundary;

        QLabel * labelRightBoundary;
        QDoubleSpinBox * input5RightBoundary;

        QSpacerItem * spacerHorizontal;
        QSpacerItem * spacerVertical;
    };

}

#endif // IBVSWIDGET_H
