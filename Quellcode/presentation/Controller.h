#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../model/Model.h"
#include "Ui.h"

namespace presentation {

    class Controller
    {
    //Funktionen:
    public:
        Controller(QApplication & application);

        void focusChangedSlot(QWidget * old, QWidget * now);
        void heatSourcesClickSlot(QMouseEvent * event);
        void heatSourceValueChangedSlot(int row, int column); //Column->Modularität?
        void newBottomBoundarySlot(double newBB);
        void newInitialValueSlot(double newIV);
        void newLeftBoundarySlot(double newLB);
        void newMSlot(int newM);
        void newNSlot(int newN);
        void newRightBoundarySlot(double newRB);
        void newTopBoundarySlot(double newTB);
        void newTSlot(double newT);
        void playVideoSlot();
        void selectIntMethodSlot(QString newIntMethod);
        void selectIterativeSolverSlot(QString newIterativeSolver);
        void setModel(model::Model * model);
        void setUI(UI * ui);
        void simulateSlot();
        void testPartialHeatSource();
        void testPartialThermalConductivity();
        void thermalConductivitiesClickSlot(QMouseEvent * event);
        void thermalConductivityValueChangedSlot(int row, int column); //Column->Modularität?
        void undoHeatSourceSlot();
        void undoThermalConductivitySlot();
        void visualizeStateSlot(int frame);

    private:
        Controller();

    //Attribute:
    private:
        QApplication & application;
        model::Model * model;
        QVector<double> partialAreaX;
        QVector<double> partialAreaY;
        bool startedNewHeatSource;
        bool startedNewThermalConductivity;
        UI * ui;
    };

}

#endif // CONTROLLER_H
