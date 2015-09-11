#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../model/Model.h"
#include "Ui.h"
#include <QObject>
#include <QInputDialog>
#include <QMessageBox>

namespace presentation {

    class Controller : public QObject
    {
        Q_OBJECT

    //Funktionen:
    public:
        Controller(QObject * parent = 0);
        ~Controller();

        void setModel(model::Model * model);
        void setUI(UI * ui);
        void testPartialHeatSource();
        void testPartialThermalConductivity();

    public slots:
        void heatSourcesClickSlot(QMouseEvent * event);
        void heatSourceValueChangedSlot(int pos, int column);
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
        void simulateSlot();
        void tabChangedSlot(int newTab);
        void thermalConductivitiesClickSlot(QMouseEvent * event);
        void thermalConductivityValueChangedSlot(int pos, int column);
        void undoHeatSourceSlot();
        void undoThermalConductivitySlot();
        void visualizeStateSlot(int frame);

    //Attribute:
    private:
        bool loopBack;
        model::Model * model;
        QVector<double> partialAreaX;
        QVector<double> partialAreaY;
        bool startedNewHeatSource;
        bool startedNewThermalConductivity;
        UI * ui;
        QInputDialog * userInput;
        QMessageBox * errorMessages;
    };

}

#endif // CONTROLLER_H
