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
#include <QTableWidget>
#include <QTabWidget>
#include <QWidget>

#include <QGridLayout>
#include <QVBoxLayout>

#include "../model/Area.h"
#include "Qcustomplot.h"
#include "../model/Model.h"

namespace presentation {
class Controller;

    class UI : public QMainWindow
    {
        Q_OBJECT

    //Enums + public Konstanten:
    public:
        // TODO: IDs setzten
        enum ActiveTab
        {
            TabConfiguration = 0,
            TabHeatSources = 6,
            TabHelp = 4,
            TabIBVs = 7,
            TabParameterFitting = 3,
            TabSimulating = 1,
            TabThermalConductivity = 5,
            TabVisualization = 2
        };
        enum AreaTableColumn
        {
            ColumnID = 0,
            ColumnValue = 1,
            ColumnVisibility = 2
        };
        double const MaxConductivity;
        double const MaxTemperature;

    //Funktionen:
    public:
        UI(QWidget *parent = 0);
        ~UI();

        void drawPartialHeatSource(QVector<double> const & partialAreaX, QVector<double> const & partialAreaY);
        void drawPartialThermalConductivity(QVector<double> const & partialAreaX, QVector<double> const & partialAreaY);
        int getHeatSourceID(int const pos) const;
        QSize getHeatSourcePlotSize() const;
        int getInitialFrame() const;
        QString getNewHeatSourceValue(int row) const;
        QString getNewThermalConductivityValue(int row) const;
        int getThermalConductivityID(int const pos) const;
        QSize getThermalConductivityPlotSize() const;
        void heatSourcePixelToCoords(double const mouseX, double const mouseY, double & x, double & y);
        void revertTabChange(UI::ActiveTab targetTab);
        void setActiveTab(int targetTab);
        void setController(Controller * controller);
        void setModel(model::Model * model);
        void thermalConductivityPixelToCoords(double const mouseX, double const mouseY, double & x, double & y);
        void updateNotification();
        void updateVisibilityHeatSource(int const pos);
        void updateVisibilityThermalConductivity(int const pos);
        void visualizeState(int const frame);


    private:
        void initConfiguration();
        void initHeatSources();
        void initHelp();
        void initIBVs();
        void initSimulating();
        void initThermalConductivities();
        void initVisualization();
        void updateHeatSources();
        void updateIBVs();
        void updateSimulating();
        void updateThermalConductivties();
        void updateVisualization();

    private slots:
        void transformTabIDSlot(int targetTab);
        void updateLcdSlot(int value);

    signals:
        void subTabChange(int targetTab);



    //Attribute:
    private:
        int activeTab;
        Controller * controller;
        model::Model * model;
        double *** result;
        int resultM;
        int resultN;
        double resultT;
        int const tabMainCount;
        QMap<int,bool> visibilityHeatSources;
        QMap<int,bool> visibilityThermalConductivities;


        //Qt Elemente:
        //Widgets
        QWidget * widgetHelp;
        QWidget * widgetConfigurationHeatSources;
        QWidget * widgetConfigurationIBVs;
        QWidget * widgetConfigurationThermalConductivities;
        QWidget * widgetSimulation;
        QWidget * widgetVisualisation;

        //Layouts
        //subGridLayouts
        QGridLayout * subGridLayoutHelp;
        QGridLayout * subGridLayoutKonfigurationHeatSources;
        QGridLayout * subGridLayoutKonfigurationIBVs;
        QGridLayout * subGridLayoutKonfigurationThermalConductivities;
        QGridLayout * subGridLayoutSimulation;
        QGridLayout * subGridLayoutVisualisation;
        QGridLayout * subsubGridLayoutHeatSource;

        //Buttons
        QPushButton * buttonPlayVideo;
        QPushButton * buttonSimulate;
        QPushButton * buttonUndoHeatSource;
        QPushButton * buttonUndoThermalConductivity;
        QPushButton * buttonRedoHeatSource;
        QPushButton * buttonRedoThermalConductivity;
        QPushButton * buttonClearHeatSource;
        QPushButton * buttonClearThermalConductivity;
        QPushButton * buttonConfirmHeatSource;
        QPushButton * buttonUpHeatSource;
        QPushButton * buttonUpAllHeatSource;
        QPushButton * buttonDownHeatSource;
        QPushButton * buttonDownAllHeatSource;
        QPushButton * buttonDiscardHeatSource;
        QPushButton * buttonDeleteHeatSource;
        QPushButton * buttonSaveHeatSource;
        QPushButton * buttonLoadHeatSource;

        //DoubleSpinBoxes
        QDoubleSpinBox * doubleSpinBoxBottomBoundary;
        QDoubleSpinBox * doubleSpinBoxInitialValue;
        QDoubleSpinBox * doubleSpinBoxLeftBoundary;
        QDoubleSpinBox * doubleSpinBoxRightBoundary;
        QDoubleSpinBox * doubleSpinBoxT;
        QDoubleSpinBox * doubleSpinBoxTopBoundary;
        QDoubleSpinBox * doubleSpinBoxXValueHeatSource;
        QDoubleSpinBox * doubleSpinBoxYValueHeatSource;
        QDoubleSpinBox * doubleSpinBoxXValueThermalConductivity;
        QDoubleSpinBox * doubleSpinBoxYValueThermalConductivity;

        //SpinBoxes
        QSpinBox * spinBoxM;
        QSpinBox * spinBoxN;

        //RadioButtons
        QGroupBox * groupBoxAuswaehlenHeatSource;
        QGroupBox * groupBoxHeatSource;
        QRadioButton * radioButtonAuswahl;
        QRadioButton * radioButtonNeuesGebiet;
        QRadioButton * radioButtonPunktweise;
        QRadioButton * radioButtonGebietsweise;


        //Labels
        QLabel * labelBottomBoundary;
        QLabel * labelHelpTabHeatSource;
        QLabel * labelHelpTabIBVs;
        QLabel * labelHelpTabKonfiguration;
        QLabel * labelHelpTabSimuation;
        QLabel * labelHelpTabThermalConductivity;
        QLabel * labelHelpTabVisualization;
//        QLabel * labelHelpTabOptinal;
        QLabel * labelInitialValue;
        QLabel * labelLeftBoundary;
        QLabel * labelM;
        QLabel * labelN;
        QLabel * labelProgressBar;
        QLabel * labelRightBoundary;
        QLabel * labelSelectIntMethod;
        QLabel * labelSelectSolver;
        QLabel * labelT;
        QLabel * labelTopBoundary;
        QLabel * labelTopHeatSource;
        QLabel * labelTopIBV;
        QLabel * labelTopSimulation;
        QLabel * labelTopThermalConductivity;
        QLabel * labelTopVisualization;
        QLabel * labelKeyboardHeatSource;
        QLabel * labelKeyboardThermalConductivity;
        QLabel * labelKeyboardHeatSourceXValue;
        QLabel * labelKeyboardHeatSourceYValue;

        //Plots + Plottables
        QCustomPlot * plateHeatSource;
        QCustomPlot * plateThermalConductivity;
        QCustomPlot * plateVideo;

        QCPColorMap * colorMapVideo;

        QCPColorScale * colorScaleHeatSource;
        QCPColorScale * colorScaleThermalConductivity;
        QCPColorScale * colorScaleVideo;


        //Slider
        QSlider * sliderVideo;

        //LCDNumber
        QLCDNumber * lcdNumberVideoTimestep;

        //ProgressBar
        QProgressBar * progressBarProgress;

        //ComboBoxes
        QComboBox * comboBoxIntMethod;
        QComboBox * comboBoxIterativeSolver;

        //TableWidgets
        QTableWidget * tableWidgetHeatSources;
        QTableWidget * tableWidgetThermalConductivities;
        QStringList listHeaderThermalConductivity;
        QStringList listHeaderHeatSource;

        //TabWidgets
        QTabWidget * tabWidgetMain;
        QTabWidget * tabWidgetSub;

        //Spaceritem
        QSpacerItem * spacerItemTabIBVHorizontal;
        QSpacerItem * spacerItemTabIBVVertical;
        QSpacerItem * spacerItemTabSimulationHorizontal;
        QSpacerItem * spacerItemTabSimulationVertical;
        QSpacerItem * spacerItemTabVisualisation;
        QSpacerItem * spacerItemTabHelp;
        QSpacerItem * spaceritemHeatSource;
        QSpacerItem * spaceritem2HeatSource;
        QSpacerItem * spaceritem3HeatSource;
        QSpacerItem * spaceritem4HeatSource;
    };

}

#endif // UI_H
