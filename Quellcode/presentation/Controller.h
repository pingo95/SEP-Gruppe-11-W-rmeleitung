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

        void getPartialArea(QVector<double> & xKoords,
                            QVector<double> & yKoords) const;
        bool getRedoPossible(model::SimulationSetup::AreaType type) const;

        void setModel(model::Model * model);
        void setUI(UI * ui);


    public slots:
        void abortWorkSlot();
        void areaClickSlot(double xKoord, double yKoord, QSize plateSize,
                           double valueShift, bool mouseClick,
                           model::SimulationSetup::AreaType type);
        void areaValueChangedSlot(int pos, double newValue, bool ok,
                                  model::SimulationSetup::AreaType type);
        void clearAreasSlot(model::SimulationSetup::AreaType type);
        void deleteAreaSlot(int pos, model::SimulationSetup::AreaType type);
        void discardAreaSlot(model::SimulationSetup::AreaType type);
        void loadObservationsSlot();
        void loadSimulationSetupSlot();
        void newIBVValueSlot(double newValue, model::SimulationSetup::IBV ibv);
        void newMaxErrorSlot(double newMaxError);
        void newMaxItSlot(int newMaxIt);
        void newMSlot(int newM);
        void newNSlot(int newN);
        void newOverrideValueSlot(double newValue);
        void newTSlot(double newT);
        void optimizationSlot();
        void overrideThermalDiffusivities(bool override);
        void playVideoSlot();
        void redoSlot(model::SimulationSetup::AreaType type);
        void reorderAreaSlot(int pos, int dir, model::SimulationSetup::AreaType type);
        void resetSimulationSetupSlot();
        void saveSimulationSetupSlot();
        void selectIntMethodSlot(QString newIntMethod);
        void selectSolverSlot(QString newIterativeSolver);
        void simulateSlot();
        void tabChangedSlot(int newTab);
        void undoSlot(model::SimulationSetup::AreaType type);
        void useHeatSourcesSlot(bool use);
        void visualizeStateSlot();

    private:
        void clearRedo();

    //Attribute:
    private:
        bool loopBack;
        model::Model * model;
        QVector<double> partialAreaX;
        QVector<double> partialAreaY;
        bool redoPossible[2];
        QList<double> redoXStack;
        QList<double> redoYStack;
        bool started[2];
        UI * ui;
        QInputDialog * userInput;
        QMessageBox * errorMessages;
    };

}

#endif // CONTROLLER_H
