#include "Ibvswidget.h"

#include "Controller.h"

presentation::IBVsWidget::IBVsWidget(QWidget *parent) : QWidget(parent)
{
        //Top-Label
    topLabel = new QLabel("Dies ist der Tab zur Einstellung der Anfangs- und Randwerte.\n"
                               "Für weitere Informationen wechseln Sie bitte in den Hilfe-Tab.",
                               this);
    topLabel->setWordWrap(true);

        //Inputs
    labelInitialValue = new QLabel("Anfangswert eingeben",this);
    input1InitialValue = new QDoubleSpinBox(this);
    input1InitialValue->setKeyboardTracking(false);
    input1InitialValue->setMinimum(model::SimulationSetup::MinTemperature);
    input1InitialValue->setMaximum(model::SimulationSetup::MaxTemperature);
    input1InitialValue->setSuffix("K");

    labelBottomBoundary = new QLabel("unteren Randwert eingeben",this);
    input2BottomBoundary = new QDoubleSpinBox(this);
    input2BottomBoundary->setKeyboardTracking(false);
    input2BottomBoundary->setMinimum(model::SimulationSetup::MinTemperature);
    input2BottomBoundary->setMaximum(model::SimulationSetup::MaxTemperature);
    input2BottomBoundary->setSuffix("K");

    labelLeftBoundary = new QLabel("linken Randwert eingeben",this);
    input3LeftBoundary = new QDoubleSpinBox(this);
    input3LeftBoundary->setKeyboardTracking(false);
    input3LeftBoundary->setMinimum(model::SimulationSetup::MinTemperature);
    input3LeftBoundary->setMaximum(model::SimulationSetup::MaxTemperature);
    input3LeftBoundary->setSuffix("K");

    labelRightBoundary = new QLabel("rechten Randwert eingeben",this);
    input4RightBoundary = new QDoubleSpinBox(this);
    input4RightBoundary->setKeyboardTracking(false);
    input4RightBoundary->setMinimum(model::SimulationSetup::MinTemperature);
    input4RightBoundary->setMaximum(model::SimulationSetup::MaxTemperature);
    input4RightBoundary->setSuffix("K");

    labelTopBoundary = new QLabel("oberen Randwert eingeben",this);
    input5TopBoundary = new QDoubleSpinBox(this);
    input5TopBoundary->setKeyboardTracking(false);
    input5TopBoundary->setMinimum(model::SimulationSetup::MinTemperature);
    input5TopBoundary->setMaximum(model::SimulationSetup::MaxTemperature);
    input5TopBoundary->setSuffix("K");

        //Platzhalter
    spacerHorizontal = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);
    spacerVertical = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);

        //Layout
    layout = new QGridLayout(this);
    layout->addWidget(topLabel,0,0,1,2);
    layout->addWidget(labelInitialValue,1,0);
    layout->addWidget(input1InitialValue,1,1);
    layout->addItem(spacerVertical,1,2,5,1);
    layout->addWidget(labelBottomBoundary,2,0);
    layout->addWidget(input2BottomBoundary,2,1);
    layout->addWidget(labelLeftBoundary,3,0);
    layout->addWidget(input3LeftBoundary,3,1);
    layout->addWidget(labelRightBoundary,4,0);
    layout->addWidget(input4RightBoundary,4,1);
    layout->addWidget(labelTopBoundary,5,0);
    layout->addWidget(input5TopBoundary,5,1);
    layout->addItem(spacerHorizontal,6,0);

    layout->setColumnMinimumWidth(0,200);
    layout->setColumnMinimumWidth(1,150);

    connect(input1InitialValue,SIGNAL(valueChanged(double)),this,SLOT(iBVsMapperSlot(double)));
    connect(input2BottomBoundary,SIGNAL(valueChanged(double)),this,SLOT(iBVsMapperSlot(double)));
    connect(input3LeftBoundary,SIGNAL(valueChanged(double)),this,SLOT(iBVsMapperSlot(double)));
    connect(input4RightBoundary,SIGNAL(valueChanged(double)),this,SLOT(iBVsMapperSlot(double)));
    connect(input5TopBoundary,SIGNAL(valueChanged(double)),this,SLOT(iBVsMapperSlot(double)));
}

void presentation::IBVsWidget::setController(Controller *controller)
{
    this->controller = controller;

    connect(this,SIGNAL(iBVsValueChange(double,model::SimulationSetup::IBV)),
            controller,SLOT(newIBVValueSlot(double,model::SimulationSetup::IBV)));
}

void presentation::IBVsWidget::setModel(model::Model *model)
{
    this->model = model;
}

void presentation::IBVsWidget::update()
{
    input1InitialValue->setValue(model->getSimulationSetup()->getIBV(model::SimulationSetup::InitialValue));
    input2BottomBoundary->setValue(model->getSimulationSetup()->getIBV(model::SimulationSetup::BottomBoundary));
    input3LeftBoundary->setValue(model->getSimulationSetup()->getIBV(model::SimulationSetup::LeftBoundary));
    input4RightBoundary->setValue(model->getSimulationSetup()->getIBV(model::SimulationSetup::RightBoundary));
    input5TopBoundary->setValue(model->getSimulationSetup()->getIBV(model::SimulationSetup::TopBoundary));
}

void presentation::IBVsWidget::iBVsMapperSlot(double newValue)
{
    QObject * _sender = sender();
    if(_sender == input1InitialValue)
    {
        emit iBVsValueChange(newValue,model::SimulationSetup::InitialValue);
        return;
    }
    if(_sender == input2BottomBoundary)
    {
        emit iBVsValueChange(newValue,model::SimulationSetup::BottomBoundary);
        return;
    }
    if(_sender == input3LeftBoundary)
    {
        emit iBVsValueChange(newValue,model::SimulationSetup::LeftBoundary);
        return;
    }
    if(_sender == input4RightBoundary)
    {
        emit iBVsValueChange(newValue,model::SimulationSetup::RightBoundary);
        return;
    }
    //else
    emit iBVsValueChange(newValue,model::SimulationSetup::TopBoundary);
}