#include "Visualizationwidget.h"

#include "Controller.h"

presentation::VisualizationWidget::VisualizationWidget(QWidget *parent)
    : QWidget(parent), controller(NULL), model(NULL), result(NULL), resultM(1),
      resultN(3), resultT(1.), updating(false)
{
        //Top-Label
    topLabel = new QLabel("",this);
    topLabel->setWordWrap(true);

        //Buttons
    playButton = new QPushButton(this);
    playButton->setIcon(QIcon(":/Icons/play"));
    playButton->setText("Play");
    playButton->setIconSize(QSize(15,15));
    playButton->setEnabled(false);
    playButton->setMaximumWidth(100);

        //Für Platte
    QVector<double> ticks,ticksColorBar;
    QVector<QString> tickLabels,tickLabelsColorBar;
    double temperatureSpan = model::SimulationSetup::MaxTemperature - model::SimulationSetup::MinTemperature;
    for(int i = 0; i < 11; ++i)
    {
        ticks << (double) i / 10;
        tickLabels << QString::number(ticks[i]);

        ticksColorBar << temperatureSpan * ticks[i] + model::SimulationSetup::MinTemperature;
        tickLabelsColorBar << QString::number(ticksColorBar[i]);
    }

        //Video anzeige
    plate = new QCustomPlot(this,false);
    plate->setMinimumWidth(350);
    plate->setMinimumHeight(650);
        //Platte xAchse unten
    plate->xAxis->setAutoTicks(false);
    plate->xAxis->setAutoTickLabels(false);
    plate->xAxis->setTickVector(ticks);
    plate->xAxis->setTickVectorLabels(tickLabels);
    plate->xAxis->setRange(0,1);
    plate->xAxis->grid()->setSubGridVisible(true);
        //Platte yAchse links
    plate->yAxis->setAutoTicks(false);
    plate->yAxis->setAutoTickLabels(false);
    plate->yAxis->setTickVector(ticks);
    plate->yAxis->setTickVectorLabels(tickLabels);
    plate->yAxis->setRange(0,1);
    plate->yAxis->grid()->setSubGridVisible(true);
        //Platte xAchse oben
    plate->xAxis2->setVisible(true);
    plate->xAxis2->setAutoTicks(false);
    plate->xAxis2->setAutoTickLabels(false);
    plate->xAxis2->setTickVector(ticks);
    plate->xAxis2->setTickVectorLabels(tickLabels);
    plate->xAxis2->setRange(0,1);
    plate->xAxis2->grid()->setSubGridVisible(true);
        //Platte yAchse rechts
    plate->yAxis2->setVisible(true);
    plate->yAxis2->setAutoTicks(false);
    plate->yAxis2->setAutoTickLabels(false);
    plate->yAxis2->setTickVector(ticks);
    plate->yAxis2->setTickVectorLabels(tickLabels);
    plate->yAxis2->setRange(0,1);
    plate->yAxis2->grid()->setSubGridVisible(true);

        // Color Scale
    colorScale = new QCPColorScale(plate);
    plate->plotLayout()->addElement(0,1,colorScale);
    colorScale->setLabel("Temperatur [K]");
    QCPMarginGroup * group = new QCPMarginGroup(plate);
    plate->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, group);
    colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, group);
    colorScale->setGradient(QCPColorGradient::gpThermal);
    colorScale->setDataRange(QCPRange(model::SimulationSetup::MinTemperature,
                                      model::SimulationSetup::MaxTemperature));
    colorScale->axis()->setAutoTicks(false);
    colorScale->axis()->setAutoTickLabels(false);
    colorScale->axis()->setTickVector(ticksColorBar);
    colorScale->axis()->setTickVectorLabels(tickLabelsColorBar);
    colorScale->axis()->setRange(QCPRange(model::SimulationSetup::MinTemperature,
                                          model::SimulationSetup::MaxTemperature));

    // Color Map
    colorMap = new QCPColorMap(plate->yAxis,plate->xAxis);
    colorMap->data()->setRange(QCPRange(0,1),QCPRange(0,1));
    colorMap->setColorScale(colorScale);
    plate->addPlottable(colorMap);

        //Schieberegler
    slider = new QSlider(this);
    slider->setEnabled(false);
    slider->setMinimumHeight(20);
    //sliderVideo->setMaximumHeight(200);
    slider->setOrientation(Qt::Horizontal);
    slider->setSingleStep(1);
    connect(slider,SIGNAL(sliderMoved(int)),this,SLOT(updateLcdSlot(int)));

        //LcdNumber
    lcdTimestep = new QLCDNumber(this);

        //Layout
    layout = new QGridLayout(this);
    layout->addWidget(topLabel,0,0,1,2);
    layout->addWidget(plate,1,0,2,1);
    layout->addWidget(playButton,1,1,2,1);
    layout->addWidget(slider,3,0);
    layout->addWidget(lcdTimestep,3,1);

    layout->setRowStretch(0,0);
    layout->setRowStretch(1,0);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,0);
}

void presentation::VisualizationWidget::playVideo()
{
#ifndef _WIN32
    if(resultM > 600 && resultN*resultM >= 250000)
    {
        int frame = slider->value();
        for(; frame < resultM+1; ++++frame)
        {
            for(int i = 0; i < resultN; ++i)
                for(int j = 0; j < resultN; ++j)
                    colorMap->data()->setCell(i,j,result[frame][i][j]);
            plate->replot();
            updateLcdSlot(frame);
        }
        if(frame != resultM)
        {
            for(int i = 0; i < resultN; ++i)
                for(int j = 0; j < resultN; ++j)
                    colorMap->data()->setCell(i,j,result[resultM][i][j]);
            plate->replot();
            updateLcdSlot(resultM);
        }
    }
    else
        for(int frame = slider->value(); frame < resultM+1; ++frame)
        {
            for(int i = 0; i < resultN; ++i)
                for(int j = 0; j < resultN; ++j)
                    colorMap->data()->setCell(i,j,result[frame][i][j]);
            plate->replot();
            updateLcdSlot(frame);
        }

#else
    for(int frame = slider->value(); frame < resultM+1; ++frame)
    {
        for(int i = 0; i < resultN; ++i)
            for(int j = 0; j < resultN; ++j)
                colorMap->data()->setCell(i,j,result[frame][i][j]);
        plate->replot();
        updateLcdSlot(frame);
    }
#endif
}

void presentation::VisualizationWidget::setController(Controller *controller)
{
    this->controller = controller;

    connect(playButton,SIGNAL(pressed()),controller,SLOT(playVideoSlot()));
    connect(slider,SIGNAL(sliderReleased()),controller,SLOT(visualizeStateSlot()));
}

void presentation::VisualizationWidget::setModel(model::Model *model)
{
    this->model = model;
}

void presentation::VisualizationWidget::update()
{
    if(updating)
        return;
    updating = true;
    if(model->isWorking())
    {
        slider->setEnabled(false);
        playButton->setEnabled(false);
        topLabel->setText("Es wird zurzeit simuliert, "
                          "Ergebnisse können erst nach Ende "
                          "der Simulation angezeigt werden.");
        colorMap->data()->setSize(1,1);
        colorMap->data()->setCell(0,0,0);
    }
    else
    {
        if (model->getSimulated())
        {
            slider->setEnabled(true);
            playButton->setEnabled(true);
            topLabel->setText("Dies ist der Tab zur Visualisierung der Simulationsergebnisse.\n"
                              "Hier können Sie sich, mit Hilfe des Schiebereglers, Einzelbilder oder ein Video anzeigen lassen."
                              "Für genauere Informationen wechseln Sie in den Hilfe-Tab.");
            resultM = model->getResultM();
            resultN = model->getResultN();
            resultT = model->getResultT();
            result = model->getResult();
            slider->setRange(0,resultM);
            slider->setValue(0);

            colorMap->data()->setSize(resultN,resultN);

            visualizeState();
        }
        else
        {
            topLabel->setText("Ergebnisse können erst angezeigt werden, "
                              "nachdem eine Simulation durchgeführt wurde.");
        }
    }
    updating = false;
}

void presentation::VisualizationWidget::visualizeState()
{
    int frame = slider->value();
    for(int i = 0; i < resultN; ++i)
        for(int j = 0; j < resultN; ++j)
            colorMap->data()->setCell(i,j,result[frame][i][j]);
    plate->replot();
    updateLcdSlot(frame);
}

void presentation::VisualizationWidget::updateLcdSlot(int value)
{
    lcdTimestep->display((double) value * resultT/(double)(resultM));
}
