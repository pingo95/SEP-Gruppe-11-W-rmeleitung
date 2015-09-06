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

        void heatSourcesClickSlot(QMouseEvent * event);
        void heatSourceValueChangedSlot(int pos, UI::AreaTableColumn column); //Column->Modularität?
        void newBottomBoundarySlot(double newBottomBoundary);
        void newInitialValueSlot(double newInitialValue);
        void newLeftBoundarySlot(double newLeftBoundary);
        void newMSlot(int newM);
        void newNSlot(int newN);
        void newRightBoundarySlot(double newRightBoundary);
        void newTopBoundarySlot(double newTopBoundary);
        void newTSlot(double newT);
        void playVideoSlot();
        void selectIntMethodSlot(QString newIntMethod);
        void selectIterativeSolverSlot(QString newIterativeSolver);
        void setModel(model::Model * model);
        void setUI(UI * ui);
        void simulateSlot();
        void tabChangedSlot(UI::ActiveTab newTab);
        void testPartialHeatSource();
        void testPartialThermalConductivity();
        void thermalConductivitiesClickSlot(QMouseEvent * event);
        void thermalConductivityValueChangedSlot(int pos, UI::AreaTableColumn column); //Column->Modularität?
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
