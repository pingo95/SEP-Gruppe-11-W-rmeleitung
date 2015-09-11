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
        QColor valueToColour(double const value, model::Model::AreaTyp type);

    private slots:
        void transformTabID(int targetTab);

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
        int resultT;
        int const tabMainCount;
        QMap<int,bool> visibilityHeatSources;
        QMap<int,bool> visibilityThermalConductivities;


        //Qt Elemente:
        //Widgets
        QWidget * widgetHelp;
        QWidget * widgetConfiguration;
        QWidget * widgetConfigurationHeatSources;
        QWidget * widgetConfigurationIBVs;
        QWidget * widgetConfigurationThermalConductivities;
        QWidget * widgetSimulation;
        QWidget * widgetVisualisation;

        //Layouts
        //subGridLayouts
        QGridLayout * subGridLayoutHelp;
        QGridLayout * subGridLayoutConfiguration;
        QGridLayout * subGridLayoutKonfigurationHeatSources;
        QGridLayout * subGridLayoutKonfigurationIBVs;
        QGridLayout * subGridLayoutKonfigurationThermalConductivities;
        QGridLayout * subGridLayoutSimulation;
        QGridLayout * subGridLayoutVisualisation;

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

        //Plate
        QCustomPlot * colorBarHeatSource;
        QCustomPlot * colorBarThermalConductivity;
        QCustomPlot * colorBarVisualization;
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
    };

}

#endif // UI_H
