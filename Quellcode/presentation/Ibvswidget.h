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

    class IBVsWidget : public QWidget
    {
        Q_OBJECT

    //Funktionen:
    public:
        explicit IBVsWidget(QWidget *parent = 0);
//        ~IBVsWidget();

        void setController(Controller * controller);
        void setModel(model::Model * model);

        void update();

    signals:
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
