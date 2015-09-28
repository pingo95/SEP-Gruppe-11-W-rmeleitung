#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <QWidget>

#include "../model/Model.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include "Qcustomplot.h"
#include <QSlider>
#include <QLCDNumber>

namespace presentation {
class Controller;

    class VisualizationWidget : public QWidget
    {
        Q_OBJECT

    //Funktionen:
    public:
        explicit VisualizationWidget(QWidget *parent = 0);

        int getInitialFrame() const;

        void setController(Controller * controller);
        void setModel(model::Model * model);

        void update();

        void visualizeState(int const frame);

    signals:

    public slots:

    private slots:
        void updateLcdSlot(int value);

    private:
        explicit VisualizationWidget();
//        ~VisualizationWidget();

    //Attribute:
    private:
        Controller * controller;
        model::Model * model;
        double *** result;
        int resultM;
        int resultN;
        double resultT;
        bool updating;

        QGridLayout * layout;

        QLabel * topLabel;

        QPushButton * playButton;

        QCustomPlot * plate;
        QCPColorScale * colorScale;
        QCPColorMap * colorMap;

        QSlider * slider;
        QLCDNumber * lcdTimestep;
    };

}

#endif // VISUALIZATIONWIDGET_H
