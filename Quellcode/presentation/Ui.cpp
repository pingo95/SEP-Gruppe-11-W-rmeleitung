#include "Ui.h"
#include "../model/Model.h"
#include "Controller.h"

presentation::UI::UI(QWidget *parent)
    : QMainWindow(parent), MaxConductivity(430), MaxTemperature(1000), // höchste Leitfähigkeit eines Metalls -> Silber
      activeTab(UI::TabThermalConductivity) , tabMainCount(5)
{
    //centrales Widget initialisieren
    widgetCentral = new QWidget(this);

    //TabBar initialisieren
    tabWidgetMain = new QTabWidget(widgetCentral);
    tabWidgetMain->setMinimumSize(350,250);

    mainLayout = new QVBoxLayout(widgetCentral);

    mainLayout->addWidget(tabWidgetMain);

    //Layout anwenden
    this->setMinimumSize(750,750);
    widgetCentral->setLayout(mainLayout);
    setCentralWidget(widgetCentral);

    //initFunktionen
    initConfiguration();

    initThermalConductivities();
    initHeatSources();
    initIBVs();

    initSimulating();
    initVisualization();
    initHelp();


    // Reste:
//    //Farben
//    QPalette Pal(palette());
//    Pal.setColor(QPalette::Background,Qt::lightGray);
    //Tabs erstellen
    //Tabs einsetzen
    //Layouts initialliesieren
    //mainLayouts
    //subGridLayouts

//    subGridLayout = new QGridLayout(); // <-- überflüssig?

    //subHBoxLayouts

//    subHBoxLayout = new QHBoxLayout(); // <-- überflüssig?

    //KonfigurationTab
    //ThermalConductivityTab
    //HeatSourceTab
    //IBVTab
    //SimulationTab
    //VisualisierungsTab
    //HelpTab
}

presentation::UI::~UI()
{

}


void presentation::UI::drawPartialHeatSource(QVector<double> const & partialAreaX,
                                             QVector<double> const & partialAreaY)
{
    if(plateHeatSource->graphCount() <= tableWidgetHeatSources->rowCount())
        plateHeatSource->addGraph();
    plateHeatSource->graph(plateHeatSource->graphCount()-1)->setData(partialAreaX,partialAreaY);
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    QPen myPen(Qt::gray);
    myScatter.setPen(myPen);
    myScatter.setBrush(Qt::gray);
    myScatter.setSize(5);
    plateHeatSource->graph(plateHeatSource->graphCount()-1)->setScatterStyle(myScatter);
    myPen.setWidth(2* myPen.width());
    plateHeatSource->graph(plateHeatSource->graphCount()-1)->setPen(myPen);
    plateHeatSource->replot();
}

void presentation::UI::drawPartialThermalConductivity(QVector<double> const & partialAreaX,
                                                      QVector<double> const & partialAreaY)
{
    if(plateThermalConductivity->graphCount() <= tableWidgetThermalConductivities->rowCount())
        plateThermalConductivity->addGraph();
    plateThermalConductivity->graph(plateThermalConductivity->graphCount()-1)->setData(partialAreaX,partialAreaY);
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    QPen myPen(Qt::gray);
    myScatter.setPen(myPen);
    myScatter.setBrush(Qt::gray);
    myScatter.setSize(5);
    plateThermalConductivity->graph(plateThermalConductivity->graphCount()-1)->setScatterStyle(myScatter);
    myPen.setWidth(2* myPen.width());
    plateThermalConductivity->graph(plateThermalConductivity->graphCount()-1)->setPen(myPen);
    plateThermalConductivity->replot();
}

int presentation::UI::getHeatSourceID(const int pos) const
{
    return tableWidgetHeatSources->item(pos,UI::ColumnID)->text().toInt();
}

QSize presentation::UI::getHeatSourcePlotSize() const
{
    return plateHeatSource->size();
}

int presentation::UI::getInitialFrame() const
{
    return sliderVideo->value();
}

QString presentation::UI::getNewHeatSourceValue(int row) const
{
    return tableWidgetHeatSources->item(row,UI::ColumnValue)->text();
}

QString presentation::UI::getNewThermalConductivityValue(int row) const
{
    return tableWidgetThermalConductivities->item(row,UI::ColumnValue)->text();
}

int presentation::UI::getThermalConductivityID(const int pos) const
{
    return tableWidgetThermalConductivities->item(pos,UI::ColumnID)->text().toInt();
}

QSize presentation::UI::getThermalConductivityPlotSize() const
{
    return plateThermalConductivity->size();
}


void presentation::UI::heatSourcePixelToCoords(double const mouseX, double const mouseY, double &x, double &y)
{
    x = plateHeatSource->xAxis->pixelToCoord(mouseX);
    y = plateHeatSource->yAxis->pixelToCoord(mouseY);
}

void presentation::UI::revertTabChange(UI::ActiveTab targetTab)
{
    if(targetTab == UI::TabHeatSources)
    {
        tabWidgetMain->setCurrentIndex(UI::TabConfiguration);
        tabWidgetSub->setCurrentIndex(UI::TabHeatSources-tabMainCount);
    }
    else
    {
        tabWidgetMain->setCurrentIndex(UI::TabConfiguration);
        tabWidgetSub->setCurrentIndex(UI::TabThermalConductivity-tabMainCount);
    }
}

void presentation::UI::setActiveTab(int targetTab)
{
    activeTab = targetTab;
    updateNotification();
}

void presentation::UI::setController(Controller *controller)
{
    this->controller = controller;
            //test Hinzufügen
    connect(plateHeatSource,SIGNAL(mousePress(QMouseEvent*)),controller,SLOT(heatSourcesClickSlot(QMouseEvent*)));
//    connect(tableWidgetHeatSources,SIGNAL(cellChanged(int,int)),controller,SLOT(heatSourceValueChangedSlot(int,int)));
    connect(doubleSpinBoxBottomBoundary,SIGNAL(valueChanged(double)),controller,SLOT(newBottomBoundarySlot(double)));
    connect(doubleSpinBoxInitialValue,SIGNAL(valueChanged(double)),controller,SLOT(newInitialValueSlot(double)));
    connect(doubleSpinBoxLeftBoundary,SIGNAL(valueChanged(double)),controller,SLOT(newLeftBoundarySlot(double)));
    connect(spinBoxM,SIGNAL(valueChanged(int)),controller,SLOT(newMSlot(int)));
    connect(spinBoxN,SIGNAL(valueChanged(int)),controller,SLOT(newNSlot(int)));
    connect(doubleSpinBoxRightBoundary,SIGNAL(valueChanged(double)),controller,SLOT(newRightBoundarySlot(double)));
    connect(doubleSpinBoxTopBoundary,SIGNAL(valueChanged(double)),controller,SLOT(newTopBoundarySlot(double)));
    connect(doubleSpinBoxT,SIGNAL(valueChanged(double)),controller,SLOT(newTSlot(double)));
    connect(buttonPlayVideo,SIGNAL(pressed()),controller,SLOT(playVideoSlot()));
    connect(comboBoxIntMethod,SIGNAL(activated(QString)),controller,SLOT(selectIntMethodSlot(QString)));
    connect(comboBoxIterativeSolver,SIGNAL(activated(QString)),controller,SLOT(selectIterativeSolverSlot(QString)));
    connect(buttonSimulate,SIGNAL(clicked(bool)),controller,SLOT(simulateSlot()));
    connect(tabWidgetMain,SIGNAL(currentChanged(int)),controller,SLOT(tabChangedSlot(int)));
    connect(this,SIGNAL(subTabChange(int)),controller,SLOT(tabChangedSlot(int)));
    connect(plateThermalConductivity,SIGNAL(mousePress(QMouseEvent*)),controller,SLOT(thermalConductivitiesClickSlot(QMouseEvent*)));
//    connect(tableWidgetThermalConductivities,SIGNAL(cellChanged(int,int)),controller,SLOT(thermalConductivityValueChangedSlot(int,int)));
    connect(buttonUndoHeatSource,SIGNAL(clicked(bool)),controller,SLOT(undoHeatSourceSlot()));
    connect(buttonUndoThermalConductivity,SIGNAL(clicked(bool)),controller,SLOT(undoThermalConductivitySlot()));
    connect(sliderVideo,SIGNAL(valueChanged(int)),controller,SLOT(visualizeStateSlot(int)));

//    tableWidgetThermalConductivities->setRowCount(1);
//        QTableWidgetItem * tmpItemPtr = new
//                QTableWidgetItem(QString().number(4));
//        tmpItemPtr->setFlags(Qt::ItemIsEnabled);
//        tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//        tableWidgetThermalConductivities->setItem(0,UI::ColumnID,tmpItemPtr);
//        tmpItemPtr = new
//                QTableWidgetItem(QString().number(1.0));
//        tmpItemPtr->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//        tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//        tableWidgetThermalConductivities->setItem(0,UI::ColumnValue,tmpItemPtr);
//        tmpItemPtr = new QTableWidgetItem();
//        tmpItemPtr->setFlags(Qt::ItemIsEnabled| Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
//        tmpItemPtr->setCheckState(Qt::Checked);
//        tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignRight);
//        tableWidgetThermalConductivities->setItem(0,UI::ColumnVisibility,tmpItemPtr);

////    connect(tableWidgetThermalConductivities,SIGNAL(cellChanged(int,int)),controller,SLOT(thermalConductivityValueChangedSlot(int,int)));

//    tableWidgetThermalConductivities->setRowCount(2);
//        tmpItemPtr = new
//                QTableWidgetItem(QString().number(4));
//        tmpItemPtr->setFlags(Qt::ItemIsEnabled);
//        tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//        tableWidgetThermalConductivities->setItem(1,UI::ColumnID,tmpItemPtr);
//        tmpItemPtr = new
//                QTableWidgetItem(QString().number(1.0));
//        tmpItemPtr->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//        tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//        tableWidgetThermalConductivities->setItem(1,UI::ColumnValue,tmpItemPtr);
//        tmpItemPtr = new QTableWidgetItem();
//        tmpItemPtr->setFlags(Qt::ItemIsEnabled| Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
//        tmpItemPtr->setCheckState(Qt::Checked);
//        tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignRight);
//        tableWidgetThermalConductivities->setItem(1,UI::ColumnVisibility,tmpItemPtr);
}

void presentation::UI::setModel(model::Model *model)
{
    this->model = model;
    QList<QString> tmpList = model->getIntMethodNames();
    QList<QString>::const_iterator it = tmpList.begin();
    for(; it < tmpList.end(); ++it)
        comboBoxIntMethod->addItem((*it));
    tmpList = model->getIterativeSolverNames();
    it = tmpList.begin();
    for(; it < tmpList.end(); ++it)
        comboBoxIterativeSolver->addItem((*it));
    // Initialen Tab laden/updaten
//    updateNotification();
}

void presentation::UI::updateVisibilityHeatSource(const int pos)
{
    bool visible = tableWidgetHeatSources->item(pos,UI::ColumnVisibility)->checkState() == Qt::Checked ? true : false;
    bool ok;
    int id = tableWidgetHeatSources->item(pos,UI::ColumnID)->text().toInt(&ok);
    if(ok)
    {
        visibilityHeatSources[id] = visible;
        plateHeatSource->graph(pos)->setVisible(visible);
    }
}

void presentation::UI::updateVisibilityThermalConductivity(const int pos)
{
    bool visible = tableWidgetThermalConductivities->item(pos,UI::ColumnVisibility)->checkState() == Qt::Checked ? true : false;
    bool ok;
    int id = tableWidgetThermalConductivities->item(pos,UI::ColumnID)->text().toInt(&ok);
    if(ok)
    {
        visibilityThermalConductivities[id] = visible;
        plateThermalConductivity->graph(pos)->setVisible(visible);
    }
}

void presentation::UI::thermalConductivityPixelToCoords(double const mouseX, double const mouseY, double &x, double &y)
{
    x = plateThermalConductivity->xAxis->pixelToCoord(mouseX);
    y = plateThermalConductivity->yAxis->pixelToCoord(mouseY);
}

void presentation::UI::updateNotification()
{
    switch(activeTab)
    {
    case TabHeatSources:
        updateHeatSources();
        break;
    case TabIBVs:
        updateIBVs();
        break;
    case TabParameterFitting:
        //updateParameterFitting;
        break;
    case TabSimulating:
        updateSimulating();
        break;
    case TabThermalConductivity:
        updateThermalConductivties();
        break;
    case TabVisualization:
        updateVisualization();
        break;
    default:
        break;
    }
}

void presentation::UI::visualizeState(int frame)
{

}

void presentation::UI::updateHeatSources()
{   
    int hSCount = model->getHeatSourcesCount();
    int rowCount = tableWidgetHeatSources->rowCount();
    tableWidgetHeatSources->setRowCount(hSCount);
    int tmpBound = hSCount <= rowCount ? hSCount : rowCount;
    QList<model::Area*> const & heatSources = model->getHeatSources();
    QList<model::Area*>::const_iterator it = heatSources.begin();
    int i = 0;
    for(; i < tmpBound; ++i, ++it)
    {
        tableWidgetHeatSources->item(i,UI::ColumnID)->
                setText(QString().number((*it)->getID()));
        tableWidgetHeatSources->item(i,UI::ColumnValue)->
                setText(QString().number((*it)->getValue()));
        tableWidgetHeatSources->item(i,UI::ColumnVisibility)->
                setCheckState(visibilityHeatSources.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
    }
    if(hSCount >= rowCount)
    {
        for(; i < hSCount; ++i, ++it)
        {
            QTableWidgetItem * tmpItemPtr = new
                    QTableWidgetItem(QString().number((*it)->getID()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetHeatSources->setItem(i,UI::ColumnID,tmpItemPtr);
            tmpItemPtr = new
                    QTableWidgetItem(QString().number((*it)->getValue()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetHeatSources->setItem(i,UI::ColumnValue,tmpItemPtr);
            tmpItemPtr = new QTableWidgetItem();
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            tmpItemPtr->setCheckState(visibilityHeatSources.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
            tableWidgetHeatSources->setItem(i,UI::ColumnVisibility,tmpItemPtr);
        }
    }
    else
    {
        for(int j = rowCount-1; j >= i; --j)
        {
            delete tableWidgetHeatSources->takeItem(j,UI::ColumnID);
            delete tableWidgetHeatSources->takeItem(j,UI::ColumnValue);
        }
    }


    for(int j = 0; j < rowCount; ++j)
        plateHeatSource->graph(j)->setVisible(false);
    for(int j = rowCount+1; j < hSCount; ++j)
        plateHeatSource->addGraph();
    it = heatSources.begin();
    for(int j = 0; j < hSCount; ++j,++it)
    {
        QVector<double> x,y;
        (*it)->getPoints(x,y);
        plateHeatSource->graph(j)->setData(x,y);
        plateHeatSource->graph(j)->setPen(QPen(Qt::black));
        plateHeatSource->graph(j)->setBrush(QBrush(valueToColour((*it)->getValue(),
                                  model::Model::HeatSourceArea),Qt::SolidPattern));
        plateHeatSource->graph(j)->setScatterStyle(QCPScatterStyle::ssNone);
        plateHeatSource->graph(j)->setVisible(visibilityHeatSources.value((*it)->getID(),true));
    }
    plateHeatSource->replot();
    controller->testPartialHeatSource();

    if(hSCount==0)
    {
        buttonUndoHeatSource->setEnabled(false);
    }
    else
    {
        buttonUndoHeatSource->setEnabled(true);
    }
}

void presentation::UI::updateIBVs()
{
    doubleSpinBoxBottomBoundary->setValue(model->getBottomBoundary());
    doubleSpinBoxInitialValue->setValue(model->getInitialValue());
    doubleSpinBoxLeftBoundary->setValue(model->getLeftBoundary());
    doubleSpinBoxRightBoundary->setValue(model->getRightBoundary());
    doubleSpinBoxTopBoundary->setValue(model->getTopBoundary());
}


void presentation::UI::updateSimulating()
{
    doubleSpinBoxT->setValue(model->getT());
    spinBoxM->setValue(model->getM());
    spinBoxN->setValue(model->getN());
    doubleSpinBoxT->setValue(model->getT());
    if(model->getSimulating())
    {
        buttonSimulate->setEnabled(false);
        labelTopSimulation->setText("Es kann nicht noch einmal simuliert werden, "
                                    "wenn die vorherige Simulation noch nicht abgeschlossen wurde");
    }
    else
    {
       buttonSimulate->setEnabled(true);
       labelTopSimulation->setText("Dies ist der Tab zur Einstellung der Simulation.\n"
                                   "Für weitere Informationen wechseln Sie in den Hilfe-Tab.");
    }

    comboBoxIntMethod->setCurrentText(model->getSelectedIntMethod());
    comboBoxIterativeSolver->setCurrentText(model->getSelectedIterativeSolver());
}

void presentation::UI::updateThermalConductivties()
{
    int tCCount = model->getThermalConductivitiesCount();
    int rowCount = tableWidgetThermalConductivities->rowCount();
    tableWidgetThermalConductivities->setRowCount(tCCount);
    int tmpBound = tCCount <= rowCount ? tCCount : rowCount;
    QList<model::Area*> const & thermalConductivities = model->getThermalConductivities();
    QList<model::Area*>::const_iterator it = thermalConductivities.begin();
    int i = 0;
    for (; i < tmpBound; ++i, ++it)
    {
        tableWidgetThermalConductivities->item(i,UI::ColumnID)->
                setText(QString().number((*it)->getID()));
        tableWidgetThermalConductivities->item(i,UI::ColumnValue)->
                setText(QString().number((*it)->getValue()));
        tableWidgetThermalConductivities->item(i,UI::ColumnVisibility)->
                setCheckState(visibilityThermalConductivities.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
    }
    if (tCCount >= rowCount)
    {
        for(; i < tCCount; ++i, ++it)
        {
            QTableWidgetItem * tmpItemPtr = new
                    QTableWidgetItem(QString().number((*it)->getID()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetThermalConductivities->setItem(i,UI::ColumnID,tmpItemPtr);
            tmpItemPtr = new
                    QTableWidgetItem(QString().number((*it)->getValue()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetThermalConductivities->setItem(i,UI::ColumnValue,tmpItemPtr);
            tmpItemPtr = new QTableWidgetItem();
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            tmpItemPtr->setCheckState(visibilityThermalConductivities.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
            tableWidgetThermalConductivities->setItem(i,UI::ColumnVisibility,tmpItemPtr);
        }
    }
    else
    {
        for(int j = rowCount-1; j >= i; --j)
        {
            delete tableWidgetThermalConductivities->takeItem(j,UI::ColumnID);
            delete tableWidgetThermalConductivities->takeItem(j,UI::ColumnValue);
        }
    }

    for(int j = 0; j < rowCount; ++j)
        plateThermalConductivity->graph(j)->setVisible(false);
    for(int j = rowCount+1; j < tCCount; ++j)
        plateThermalConductivity->addGraph();
    it = thermalConductivities.begin();
    for(int j = 0; j < tCCount; ++j,++it)
    {
        QVector<double> x,y;
        (*it)->getPoints(x,y);
        plateThermalConductivity->graph(j)->setData(x,y);
        plateThermalConductivity->graph(j)->setPen(QPen(Qt::black));
        plateThermalConductivity->graph(j)->setBrush(QBrush(valueToColour((*it)->getValue(),
                                  model::Model::ThermalConductivityArea),Qt::SolidPattern));
        plateThermalConductivity->graph(j)->setScatterStyle(QCPScatterStyle::ssNone);
        plateThermalConductivity->graph(j)->setVisible(visibilityThermalConductivities.value((*it)->getID(),true));
    }
    plateThermalConductivity->replot();
    controller->testPartialThermalConductivity();

    if(tCCount==0)
    {
        buttonUndoThermalConductivity->setEnabled(false);
    }
    else
    {
        buttonUndoThermalConductivity->setEnabled(true);
    }
}

void presentation::UI::updateVisualization()
{
    if (model->getSimulated())
    {
        sliderVideo->setEnabled(true);
        buttonPlayVideo->setEnabled(true);
        labelTopVisualization->setText("Dies ist der Tab zur Visualisierung der Simulationsergebnisse.\n"
                                       "Hier können Sie sich, mit Hilfe des Schiebereglers, Einzelbilder oder ein Video anzeigen lassen."
                                       "Für genauere Informationen wechseln Sie in den Hilfe-Tab.");
        resultM = model->getResultM();
        resultN = model->getResultN();
        resultT = model->getResultT();
        result = model->getResult();
    }
    else
    {
        labelTopVisualization->setText("Ergebnisse können erst angezeigt werden, "
                                       "nachdem eine Simulation durchgeführt wurde.");
    }
}

QColor presentation::UI::valueToColour(const double value, model::Model::AreaTyp type)
{
    double maxValue;
    switch(type)
    {
    case model::Model::HeatSourceArea:
        maxValue = MaxTemperature;
        break;
    case model::Model::ThermalConductivityArea:
        maxValue = MaxConductivity;
        break;
    default:
        return Qt::blue;
    }
    // value in Wellenlänge umwandeln
    double minValue = 0, minVisibleWaveLength = 380.0,
            maxVisibileWaveLength = 780.0;
    double wavelength = (value - minValue) / (maxValue - minValue) *
            (maxVisibileWaveLength - minVisibleWaveLength) + minVisibleWaveLength;

    // Wellenlänge in rgb umrechnen
    double factor = 1.0, red, green, blue;
    if(wavelength >= 380 && wavelength <= 439)
    {
        red = -(wavelength -440) / (440-380);
        green = 0.0;
        blue = 1.0;
    }
    else
    {
        if(wavelength >= 440 && wavelength <= 489)
        {
            red = 0.0;
            green = (wavelength - 440) / (490 - 440);
            blue = 1.0;
        }
        else
        {
            if(wavelength >= 490 && wavelength <= 509)
            {
                red = 0.0;
                green = 1.0;
                blue = -(wavelength - 510) / (510 - 490);
            }
            else
            {
                if(wavelength >= 510 && wavelength <= 579)
                {
                    red = (wavelength - 510) / (580 - 510);
                    green = 1.0;
                    blue = 0.0;
                }
                else
                {
                    if(wavelength >= 580 && wavelength <= 644)
                    {
                        red = 1.0;
                        green = -(wavelength - 645) / (645 - 580);
                        blue = 0.0;
                    }
                    else
                    {
                        if(wavelength >= 645 && wavelength <= 780)
                        {
                            red = 1.0;
                            green = 0.0;
                            blue = 0.0;
                        }
                        else
                        {
                            red = 0.0;
                            green = 0.0;
                            blue = 0.0;
                        }
                    }
                }
            }
        }
    }
    if(wavelength >= 380 && wavelength <= 419)
        factor = 0.3 + 0.7*(wavelength - 380) / (420 -380);
    else
        if(wavelength >= 701 && wavelength <= 780)
            factor = 0.3 + 0.7*(780 -  wavelength) / (780 - 700);
    double gamma = 0.8, intensityMax = 255;
    int r = round(intensityMax * pow(red*factor,gamma));
    int g = round(intensityMax * pow(green*factor,gamma));
    int b = round(intensityMax * pow(blue*factor,gamma));
    return QColor(r,g,b,255);
}

void presentation::UI::transformTabID(int targetTab)
{
    emit subTabChange(targetTab + tabMainCount);
}
