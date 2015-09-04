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

#include "Qcustomplot.h"

namespace presentation {

    class UI : public QMainWindow
    {
        Q_OBJECT

    public:
        UI(QWidget *parent = 0);
        ~UI();

    private:

       QWidget * widgetCentral;
       QWidget * widgetHelp;
       QWidget * widgetVisualisation;
       QWidget * widgetSimulation;
       QWidget * widgetKonfiguration;

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
