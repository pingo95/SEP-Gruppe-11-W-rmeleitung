#ifndef SIMULATIONWIDGET_H
#define SIMULATIONWIDGET_H

#include <QWidget>

#include "../model/Model.h"

#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressBar>

namespace presentation {
class Controller;

    class SimulationWidget : public QWidget
    {
        Q_OBJECT

    //Funktionen:
    public:
        explicit SimulationWidget(QWidget *parent = 0);
//        ~SimulationWidget();

        void setController(Controller * controller);
        void setModel(model::Model * model);

        void update();

        // Slots in spe
        void appendToSimulationLog(QString text);
        void nextStage(QString stage, int maximum);
        void updateProgress(int step);

    signals:
        void newMaxErrorValue(double newValue);

    private slots:
        void transformMaxError(int value);

    private:
        explicit SimulationWidget();

    //Attribute:
    private:
        Controller * controller;
        model::Model * model;

        QGridLayout * layout;

        QLabel * topLabel;

        QGroupBox * boxIntMethod;
        QGridLayout * boxIntMethodLayout;
        QLabel * labelIntMethod;
        QComboBox * inputIntMethod;
        QLabel * labelM;
        QSpinBox * inputM;
        QLabel * labelT;
        QDoubleSpinBox * inputT;

        QGroupBox * boxSolver;
        QGridLayout * boxSolverLayout;
        QLabel * labelSolver;
        QComboBox * inputSolver;
        QLabel * labelMaxError;
        QSpinBox * inputMaxError;
        QLabel * labelMaxIt;
        QSpinBox * inputMaxIt;

        QGroupBox * boxSimulate;
        QGridLayout * boxSimulateLayout;
        QLabel * labelN;
        QSpinBox * inputN;
        QPushButton * simulateButton;

        QTextEdit * simulationLog;

        QLabel * labelProgressBar;
        QProgressBar * progressBar;
    };

}

#endif // SIMULATIONWIDGET_H
