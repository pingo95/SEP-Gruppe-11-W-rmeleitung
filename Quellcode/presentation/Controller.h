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

        bool getRedoPossible(model::SimulationSetup::AreaType type) const;

        void setModel(model::Model * model);
        void setUI(UI * ui);

        void testPartialArea(model::SimulationSetup::AreaType type) const; //<--Wann wirklich?

    public slots:
        void areaClickSlot(double xKoord, double yKoord, QSize plateSize,
                           double valueShift, model::SimulationSetup::AreaType type);
        void areaValueChangedSlot(int pos, double newValue, bool ok,
                                  model::SimulationSetup::AreaType type);
        void clearAreasSlot(model::SimulationSetup::AreaType type);
        void deleteAreaSlot(int pos, model::SimulationSetup::AreaType type);
        void discardAreaSlot(model::SimulationSetup::AreaType type);
        void loadObservationsSlot();
        void newIBVValueSlot(double newValue, model::SimulationSetup::IBV ibv);
        void newMSlot(int newM);
        void newMaxErrorSlot(double newMaxErrorSlot);
        void newMaxItSlot(int newMaxItSlot);
        void newNSlot(int newN);
        void newTSlot(double newT);
        void playVideoSlot();
        void redoAreaSlot(model::SimulationSetup::AreaType type);
        void reorderAreaSlot(int pos, int dir, model::SimulationSetup::AreaType type);
        void selectIntMethodSlot(QString newIntMethod);
        void selectSolverSlot(QString newIterativeSolver);
        void simulateSlot();
        void tabChangedSlot(int newTab);
        void undoAreaSlot(model::SimulationSetup::AreaType type);
        void visualizeStateSlot(int frame);

    private:
        void clearRedo();

    //Attribute:
    private:
        bool loopBack;
        model::Model * model;
        QVector<double> partialAreaX;
        QVector<double> partialAreaY;
        QList<model::Area *> redoAreaStack;
        QList<double> redoPointXStack;
        QList<double> redoPointYStack;
        bool redoPossible[2];
        bool started[2];
        UI * ui;
        QInputDialog * userInput;
        QMessageBox * errorMessages;
    };

}

#endif // CONTROLLER_H
