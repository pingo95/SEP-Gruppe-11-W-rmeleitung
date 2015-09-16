#include "Ui.h"
#include "../model/Model.h"
#include "Controller.h"

presentation::UI::UI(QWidget *parent)
    : QMainWindow(parent), MaxConductivity(430), MaxTemperature(1000), // höchste Leitfähigkeit eines Metalls -> Silber
      activeTab(UI::TabThermalConductivity) , resultM(1) , resultN(0),
      resultT(0.), tabMainCount(5)
{

    //TabBar initialisieren
    tabWidgetMain = new QTabWidget(this);
//    tabWidgetMain->setMinimumSize(350,250);

    //Layout anwenden
    this->setMinimumSize(1360,765);
    setCentralWidget(tabWidgetMain);

    //initFunktionen
    initConfiguration();

    initThermalConductivities();
    initHeatSources();
    initIBVs();

    initSimulating();
    initVisualization();
    initHelp();


}

presentation::UI::~UI()
{
}


void presentation::UI::drawPartialHeatSource(QVector<double> const & partialAreaX,
                                             QVector<double> const & partialAreaY)
{
    if(plateHeatSource->graphCount() <= tableWidgetHeatSources->rowCount()-1)
        plateHeatSource->addGraph();
    plateHeatSource->graph(plateHeatSource->graphCount()-1)->setData(partialAreaX,partialAreaY);
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    QPen myPen(Qt::green);
    myScatter.setPen(myPen);
    myScatter.setBrush(Qt::green);
    myScatter.setSize(5);
    plateHeatSource->graph(plateHeatSource->graphCount()-1)->setScatterStyle(myScatter);
    myPen.setWidth(2* myPen.width());
    plateHeatSource->graph(plateHeatSource->graphCount()-1)->setPen(myPen);
    plateHeatSource->graph(plateHeatSource->graphCount()-1)->setVisible(true);
    plateHeatSource->replot();
}

void presentation::UI::drawPartialThermalConductivity(QVector<double> const & partialAreaX,
                                                      QVector<double> const & partialAreaY)
{
    if(plateThermalConductivity->graphCount() <= tableWidgetThermalConductivities->rowCount()-1)
        plateThermalConductivity->addGraph();
    plateThermalConductivity->graph(plateThermalConductivity->graphCount()-1)->setData(partialAreaX,partialAreaY);
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    QPen myPen(Qt::green);
    myScatter.setPen(myPen);
    myScatter.setBrush(Qt::green);
    myScatter.setSize(5);
    plateThermalConductivity->graph(plateThermalConductivity->graphCount()-1)->setScatterStyle(myScatter);
    myPen.setWidth(2* myPen.width());
    plateThermalConductivity->graph(plateThermalConductivity->graphCount()-1)->setPen(myPen);
    plateThermalConductivity->graph(plateThermalConductivity->graphCount()-1)->setVisible(true);
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
    setActiveTab(targetTab);
}

void presentation::UI::setActiveTab(int targetTab)
{
    if(targetTab == UI::TabConfiguration)
        activeTab = tabWidgetSub->currentIndex() + tabMainCount;
    else
        activeTab = targetTab;
    updateNotification();
}

void presentation::UI::setController(Controller *controller)
{
    this->controller = controller;
            //test Hinzufügen
    connect(plateHeatSource,SIGNAL(mousePress(QMouseEvent*)),controller,SLOT(heatSourcesClickSlot(QMouseEvent*)));
    connect(tableWidgetHeatSources,SIGNAL(cellChanged(int,int)),controller,SLOT(heatSourceValueChangedSlot(int,int)));
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
    connect(tableWidgetThermalConductivities,SIGNAL(cellChanged(int,int)),controller,SLOT(thermalConductivityValueChangedSlot(int,int)));
    connect(buttonUndoHeatSource,SIGNAL(clicked(bool)),controller,SLOT(undoHeatSourceSlot()));
    connect(buttonUndoThermalConductivity,SIGNAL(clicked(bool)),controller,SLOT(undoThermalConductivitySlot()));
    connect(sliderVideo,SIGNAL(valueChanged(int)),controller,SLOT(visualizeStateSlot(int)));
}

void presentation::UI::setModel(model::Model *model)
{
    this->model = model;
    QList<QString> tmpList = model->getIntMethodNames();
    QList<QString>::const_iterator it = tmpList.begin();
    for(; it < tmpList.end(); ++it)
        comboBoxIntMethod->addItem((*it));
    tmpList = model->getSolverNames();
    it = tmpList.begin();
    for(; it < tmpList.end(); ++it)
        comboBoxIterativeSolver->addItem((*it));

    // Initialen Tab laden/updaten
    updateNotification();
}

void presentation::UI::updateVisibilityHeatSource(const int pos)
{
    bool visible = tableWidgetHeatSources->item(pos,UI::ColumnVisibility)->checkState() == Qt::Checked ? true : false;
    bool ok;
    int id = tableWidgetHeatSources->item(pos,UI::ColumnID)->text().toInt(&ok);
    if(ok)
    {
        visibilityHeatSources[id] = visible;
        plateHeatSource->graph(pos-1)->setVisible(visible);
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
        plateThermalConductivity->graph(pos-1)->setVisible(visible);
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
    for(int i = 0; i < resultN; ++i)
        for(int j = 0; j < resultN; ++j)
            colorMapVideo->data()->setCell(i,j,result[frame][i][j]);
    plateVideo->replot();
    updateLcdSlot(frame);

}

void presentation::UI::updateHeatSources()
{   
    double bgValue = model->getAreaBackgroundValue(model::Model::AreaHeatSource);
    tableWidgetHeatSources->item(0,UI::ColumnValue)->setText(QString::number(bgValue));
    plateHeatSource->axisRect()->setBackground(
        QBrush(colorScaleHeatSource->gradient().color(bgValue,QCPRange(0,MaxTemperature),false)));

    int hSCount = model->getAreaCount(model::Model::AreaHeatSource);
    int rowCount = tableWidgetHeatSources->rowCount();
    tableWidgetHeatSources->setRowCount(hSCount+1);
    int tmpBound = hSCount+1 <= rowCount ? hSCount+1 : rowCount;
    QList<model::Area*> const & heatSources = model->getAreas(
                model::Model::AreaHeatSource);
    QList<model::Area*>::const_iterator it = heatSources.begin();
    int i = 1;
    for(; i < tmpBound; ++i, ++it)
    {
        tableWidgetHeatSources->item(i,UI::ColumnID)->
                setText(QString::number((*it)->getID()));
        tableWidgetHeatSources->item(i,UI::ColumnValue)->
                setText(QString::number((*it)->getValue()));
        tableWidgetHeatSources->item(i,UI::ColumnVisibility)->
                setCheckState(visibilityHeatSources.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
    }
    if(hSCount+1 >= rowCount)
    {
        for(; i < hSCount+1; ++i, ++it)
        {
            QTableWidgetItem * tmpItemPtr = new
                    QTableWidgetItem(QString::number((*it)->getID()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetHeatSources->setItem(i,UI::ColumnID,tmpItemPtr);
            tmpItemPtr = new
                    QTableWidgetItem(QString::number((*it)->getValue()));
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

    QCPRange range(0,MaxTemperature);

    for(int j = 0; j < rowCount-1; ++j)
        plateHeatSource->graph(j)->setVisible(false);
    for(int j = plateHeatSource->graphCount(); j < hSCount; ++j)
        plateHeatSource->addGraph();
    it = heatSources.begin();
    for(int j = 0; j < hSCount; ++j,++it)
    {
        QVector<double> x,y;
        (*it)->getPoints(x,y);
        QColor color(colorScaleHeatSource->gradient().color((*it)->getValue(),range,false));
        plateHeatSource->graph(j)->setData(x,y);
        plateHeatSource->graph(j)->setPen(QPen(Qt::black));
        plateHeatSource->graph(j)->setBrush(QBrush(color,Qt::SolidPattern));
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
    doubleSpinBoxBottomBoundary->setValue(model->getBoundaryBottom());
    doubleSpinBoxInitialValue->setValue(model->getInitialValue());
    doubleSpinBoxLeftBoundary->setValue(model->getBoundaryLeft());
    doubleSpinBoxRightBoundary->setValue(model->getBoundaryRight());
    doubleSpinBoxTopBoundary->setValue(model->getBoundaryTop());
}


void presentation::UI::updateSimulating()
{
    doubleSpinBoxT->setValue(model->getT());
    spinBoxM->setValue(model->getM());
    spinBoxN->setValue(model->getN());
    doubleSpinBoxT->setValue(model->getT());
    doubleSpinBoxEpsilon->setValue(model->getSolverMaxError());
    spinBoxMaxIt->setValue(model->getSolverMaxIt());
    if(model->getSimulating())
    {
        buttonSimulate->setEnabled(false);
        labelTopSimulation->setText("Es wird zur Zeit simuliert.");
    }
    else
    {
       buttonSimulate->setEnabled(true);
       labelTopSimulation->setText("Dies ist der Tab zur Einstellung der Simulation.\n"
                                   "Für weitere Informationen wechseln Sie in den Hilfe-Tab.");
       if(model->getSimulated())
           labelProgressBar->setText("Simulation beendet");

    }
    comboBoxIntMethod->setCurrentText(model->getSelectedIntMethod());
    comboBoxIterativeSolver->setCurrentText(model->getSelectedSolver());
}

void presentation::UI::updateThermalConductivties()
{
    double bgValue = model->getAreaBackgroundValue(model::Model::AreaThermalConductivity);
    tableWidgetThermalConductivities->item(0,UI::ColumnValue)->setText(QString::number(bgValue));
    plateThermalConductivity->axisRect()->setBackground(
        QBrush(colorScaleThermalConductivity->gradient().color(bgValue,QCPRange(0,MaxConductivity),false)));
    int tCCount = model->getAreaCount(model::Model::AreaThermalConductivity);
    int rowCount = tableWidgetThermalConductivities->rowCount();
    tableWidgetThermalConductivities->setRowCount(tCCount+1);
    int tmpBound = tCCount+1 <= rowCount ? tCCount+1 : rowCount;
    QList<model::Area*> const & thermalConductivities = model->getAreas(
                model::Model::AreaThermalConductivity);
    QList<model::Area*>::const_iterator it = thermalConductivities.begin();
    int i = 1;
    for (; i < tmpBound; ++i, ++it)
    {
        double value = (*it)->getValue();
        tableWidgetThermalConductivities->item(i,UI::ColumnID)->
                setText(QString::number((*it)->getID()));
        tableWidgetThermalConductivities->item(i,UI::ColumnValue)->
                setText(QString::number(value));
        tableWidgetThermalConductivities->item(i,UI::ColumnVisibility)->
                setCheckState(visibilityThermalConductivities.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
    }
    if (tCCount+1 >= rowCount)
    {
        for(; i < tCCount+1; ++i, ++it)
        {
            QTableWidgetItem * tmpItemPtr = new
                    QTableWidgetItem(QString::number((*it)->getID()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetThermalConductivities->setItem(i,UI::ColumnID,tmpItemPtr);
            double value = (*it)->getValue();
            tmpItemPtr = new QTableWidgetItem(QString::number(value));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidgetThermalConductivities->setItem(i,UI::ColumnValue,tmpItemPtr);
            tmpItemPtr = new QTableWidgetItem(" ");
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
    QCPRange range(0,MaxConductivity);
    colorScaleThermalConductivity->setDataRange(range);

    for(int j = 0; j < rowCount-1; ++j)
        plateThermalConductivity->graph(j)->setVisible(false);
    for(int j = plateThermalConductivity->graphCount(); j < tCCount; ++j)
        plateThermalConductivity->addGraph();

    it = thermalConductivities.begin();
    for(int j = 0; j < tCCount; ++j,++it)
    {
        QVector<double> x,y;
        model::Area* thermalConductivity = *it;
        QColor color(colorScaleThermalConductivity->gradient().color((*it)->getValue(),range,false));
        thermalConductivity->getPoints(x,y);
        plateThermalConductivity->graph(j)->setData(x,y);
        plateThermalConductivity->graph(j)->setPen(QPen(Qt::black));
        plateThermalConductivity->graph(j)->setBrush(QBrush(color,Qt::SolidPattern));
        plateThermalConductivity->graph(j)->setScatterStyle(QCPScatterStyle::ssNone);
        plateThermalConductivity->graph(j)->setVisible(
                    visibilityThermalConductivities.value(thermalConductivity->getID(),true));
         //thermalConductivity.getValue(i*deltaX,j*deltaX);
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
    // TODO: Regler und LCD einstellen
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
        sliderVideo->setRange(0,resultM-1);
        sliderVideo->setValue(0);

        colorMapVideo->data()->setSize(resultN,resultN);

        visualizeState(0);
    }
    else
    {
        labelTopVisualization->setText("Ergebnisse können erst angezeigt werden, "
                                       "nachdem eine Simulation durchgeführt wurde.");
    }
}

void presentation::UI::appendToSimulationLogSlot(QString text)
{
    simulationLog->append(text);
}

void presentation::UI::nextProgresseStageSlot(QString stage, int maximum)
{
    progressBarProgress->setMaximum(maximum);
    progressBarProgress->setMinimum(0);
    progressBarProgress->setValue(0);
    labelProgressBar->setText(stage);
}

void presentation::UI::updateSimulationProgressSlot(int step)
{
    progressBarProgress->setValue(step);
}

void presentation::UI::transformTabIDSlot(int targetTab)
{
    emit subTabChange(targetTab + tabMainCount);
}

void presentation::UI::updateLcdSlot(int value)
{
    lcdNumberVideoTimestep->display((double) value * resultT/(double)(resultM));
}
