#ifndef UI_H
#define UI_H

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLCDNumber>
#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QSpinBox>
#include <QTableView>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <QGridLayout>
#include <QHBoxLayout>

#include "../model/Area.h"
#include "Qcustomplot.h"

namespace model{
    class Model;
}

namespace presentation {
class Controller;

    class UI : public QMainWindow
    {
        Q_OBJECT

    //Funktionen:
    public:
        UI(QWidget *parent = 0);
        ~UI();

        void drawPartialHeatSource(QVector<double> & partialAreaX, QVector<double> & partialAreaY);
        void drawPartialThermalConductivity(QVector<double> & partialAreaX, QVector<double> & partialAreaY);
        bool testFocusChange(QWidget * old);
        int getInitialFrame();
        double getNewHeatSourceValue(int row);
        double getNewThermalConductivityValue(int row);
        void heatSourcePixelToCoords(double mouseX, double mouseY, double & x, double & y);
        void revertFocusChange(QWidget * old, QWidget * now);
        void setActiveTab(int tab); //Enum?
        void setController(Controller * controller);
        void setModel(model::Model * model);
        void thermalConductivityPixelToCoords(double mouseX, double mouseY, double & x, double & y);
        void updateNotification();
        void visualizeState(int frame);

    private:
        void updateHeatSources();
        void updateIBVs();
        void updateSimulating();
        void updateThermalConductivties();
        void updateVisualization();
        void visualizeHeatSourceArea(model::Area * area);
        void visualizeThermalConductivityArea(model::Area * area);

    //Attribute:
    private:
        int activeTab;
        Controller * controller;
        model::Model * model;
        double *** result;
        int resultM;
        int resultN;
        int resultT;


        //Qt Elemente:
        //Widgets
        QWidget * widgetCentral;
        QWidget * widgetHelp;
        QWidget * widgetVisualisation;
        QWidget * widgetSimulation;
        QWidget * widgetKonfiguration;

        //Layouts
        QVBoxLayout * mainLayout;
        QGridLayout * subGridLayout;
        QHBoxLayout * subHBoxLayout;

        //Buttons
        QPushButton * buttonPlayVideo;
        QPushButton * buttonSimulate;
        QPushButton * buttonUndoHeatSource;
        QPushButton * buttonUndoThermalConductivity;

        //DoubleSpinBoxes
        QDoubleSpinBox * doubleSpinBoxBottomBoundary;
        QDoubleSpinBox * doubleSpinBoxInitialValue;
        QDoubleSpinBox * doubleSpinBoxLeftBoundary;
        QDoubleSpinBox * doubleSpinBoxRightBoundary;
        QDoubleSpinBox * doubleSpinBoxT;
        QDoubleSpinBox * doubleSpinBoxTopBoundary;

        //SpinBoxes
        QSpinBox * spinBoxM;
        QSpinBox * spinBoxN;

        //Labels
        QLabel * labelHelp;
        QLabel * labelTopHeatSource;
        QLabel * labelTopIBV;
        QLabel * labelTopSimulation;
        QLabel * labelTopThermalConductivity;
        QLabel * labelTopVisualization;

        //Plate
        QCustomPlot * plateHeatSource;
        QCustomPlot * plateThermalConductivity;
        QCustomPlot * plateVideo;

        //Slider
        QSlider * sliderVideo;

        //LCDNumber
        QLCDNumber * lcdNumberVideoTimestep;

        //ProgressBar
        QProgressBar * progressBarProgress;

        //ComboBoxes
        QComboBox * comboBoxIntMethod;
        QComboBox * comboBoxSolver;

        //TableViews
        QTableView * tableViewHeatSources;
        QTableView * tableViewThermalConductivities;

        //TabWidgets
        QTabWidget * tabWidgetMain;
        QTabWidget * tabWidgetSub;
    };

}

#endif // UI_H
