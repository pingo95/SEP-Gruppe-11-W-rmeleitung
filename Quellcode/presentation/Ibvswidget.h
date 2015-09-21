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

        QGridLayout * layout;

        QLabel * topLabel;

        // Zahlen im Namen der Boxen für Reihenfolge beim tab-Drücken
        QLabel * labelInitialValue;
        QDoubleSpinBox * input1InitialValue;

        QLabel * labelBottomBoundary;
        QDoubleSpinBox * input2BottomBoundary;

        QLabel * labelLeftBoundary;
        QDoubleSpinBox * input3LeftBoundary;

        QLabel * labelRightBoundary;
        QDoubleSpinBox * input4RightBoundary;

        QLabel * labelTopBoundary;
        QDoubleSpinBox * input5TopBoundary;

        QSpacerItem * spacerHorizontal;
        QSpacerItem * spacerVertical;
    };

}

#endif // IBVSWIDGET_H