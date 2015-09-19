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

        void testPartialArea(model::SimulationSetup::AreaType type);

    public slots:
        void areaClickSlot(double xKoord, double yKoord, QSize plateSize,
                           double valueShift, model::SimulationSetup::AreaType type);
        void areaValueChangedSlot(int pos, double newValue, bool ok,
                                  model::SimulationSetup::AreaType type);
        void newIBVValueSlot(double newValue, model::SimulationSetup::IBV side);
        void newMSlot(int newM);
        void newMaxErrorSlot(double newMaxErrorSlot);
        void newMaxItSlot(int newMaxItSlot);
        void newNSlot(int newN);
        void newTSlot(double newT);
        void playVideoSlot();
        void selectIntMethodSlot(QString newIntMethod);
        void selectSolverSlot(QString newIterativeSolver);
        void simulateSlot();
        void tabChangedSlot(int newTab);
        void undoAreaSlot(model::SimulationSetup::AreaType type);
        void visualizeStateSlot(int frame);

    //Attribute:
    private:
        bool loopBack;
        model::Model * model;
        QVector<double> partialAreaX;
        QVector<double> partialAreaY;
        bool started[2];
        UI * ui;
        QInputDialog * userInput;
        QMessageBox * errorMessages;
    };

}

#endif // CONTROLLER_H
