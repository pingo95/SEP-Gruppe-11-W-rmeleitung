#include "Areawidget.h"

#include "Controller.h"

presentation::AreaWidget::AreaWidget(QWidget *parent, model::SimulationSetup::AreaType type,
                                     QString const name, QString const unit, double const valueShift)
    : QWidget(parent), controller(NULL), model(NULL), name(name), type(type), unit(unit), valueShift(valueShift)
{
        //Top-Label
    topLabel = new QLabel("Dies ist der Tab zur Eingabe der " + name + ". \n"
                          "Sie können hier die Gebiete der " + name + " markieren "
                          "und deren Wert eingeben. Für weitere Informationen wechseln"
                          " Sie bitte in den Hilfe-Tab.",this);
    topLabel->setWordWrap(true);

        //Tabelle initialisieren
    table = new QTableWidget(this);
    table->setShowGrid(true);
    table->setRowCount(1);
    table->setColumnCount(3);

    table->setMinimumWidth(152);
    table->setMaximumWidth(152);

    QString combinedUnit = "\n[";
    if(valueShift != 1)
        combinedUnit += QString::number(valueShift,'e',0);
    combinedUnit += " " + unit + "]";

    tableHeader <<"ID"<<"Wert"+combinedUnit<<"Sichtbar";
    table->setHorizontalHeaderLabels(tableHeader);
    table->horizontalHeader()->setSectionsClickable(false);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->horizontalHeader()->setMinimumHeight(45);

    table->setColumnWidth(0,20);
    table->setColumnWidth(1,72);
    table->setColumnWidth(2,60);

    QTableWidgetItem * tmpItemPtr = new QTableWidgetItem("0");
    tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    table->setItem(0,AreaWidget::ColumnID,tmpItemPtr);
    tmpItemPtr = new QTableWidgetItem("0"); //<-- in setModel updaten!
    tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    table->setItem(0,AreaWidget::ColumnValue,tmpItemPtr);
    tmpItemPtr = new QTableWidgetItem("-");
    tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    table->setItem(0,AreaWidget::ColumnVisibility,tmpItemPtr);

        //Reorder-Box
    allUpButton = new QPushButton(this);
    allUpButton->setIcon(QIcon(":/Icons/2up"));
    allUpButton->setIconSize(QSize(15,15));
    allUpButton->setEnabled(false);

    upButton = new QPushButton(this);
    upButton->setIcon(QIcon(":/Icons/up"));
    upButton->setIconSize(QSize(15,15));
    upButton->setEnabled(false);

    downButton = new QPushButton(this);
    downButton->setIcon(QIcon(":/Icons/down"));
    downButton->setIconSize(QSize(15,15));
    downButton->setEnabled(false);

    allDownButton = new QPushButton(this);
    allDownButton->setIcon(QIcon(":/Icons/2down"));
    allDownButton->setIconSize(QSize(15,15));
    allDownButton->setEnabled(false);

    boxReorderButtons = new QGroupBox(this);
    boxReorderButtonsLayout = new QVBoxLayout();
    boxReorderButtonsLayout->addWidget(allUpButton);
    boxReorderButtonsLayout->addWidget(upButton);
    boxReorderButtonsLayout->addWidget(downButton);
    boxReorderButtonsLayout->addWidget(allDownButton);
    boxReorderButtons->setLayout(boxReorderButtonsLayout);
    boxReorderButtons->setFixedHeight(200);

        //Für Platte
    QVector<double> ticks,ticksColorBar;
    QVector<QString> tickLabels,tickLabelsColorBar;
    double valueSpan = model::SimulationSetup::AreaMaxValue[type] - model::SimulationSetup::AreaMinValue[type];
    for(int i = 0; i < 11; ++i)
    {
        ticks << (double) i / 10;
        tickLabels << QString::number(ticks[i]);

        ticksColorBar << round((valueSpan * ticks[i] + model::SimulationSetup::AreaMinValue[type])/valueShift);
        tickLabelsColorBar << QString::number(ticksColorBar[i]);
    }

        //Platte
    plate = new QCustomPlot(this,false);
    plate->setMinimumWidth(350);

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
    QString scaleTitle = name + combinedUnit;
    colorScale->setLabel(scaleTitle);

    QCPRange range(model::SimulationSetup::AreaMinValue[type]/valueShift,
                   model::SimulationSetup::AreaMaxValue[type]/valueShift);
    QCPMarginGroup * group = new QCPMarginGroup(plate);
    colorScale->setMarginGroup(QCP::msTop|QCP::msBottom, group);
    colorScale->setGradient(QCPColorGradient::gpThermal);
    colorScale->setDataRange(range);
    colorScale->axis()->setAutoTicks(false);
    colorScale->axis()->setAutoTickLabels(false);
    colorScale->axis()->setTickVector(ticksColorBar);
    colorScale->axis()->setTickVectorLabels(tickLabelsColorBar);
    plate->axisRect()->setMarginGroup(QCP::msTop|QCP::msBottom, group);
    plate->axisRect()->setBackground(QBrush(colorScale->gradient().color(0,range,false))); //<-- in setModel updaten!

        //Click-Mode-Box
    selectionModeButton = new QRadioButton("Gebiet auswählen",this);

    newAreaModeButton = new QRadioButton("Neues Gebiet",this);
    newAreaModeButton->setChecked(true);

    boxClickMode = new QGroupBox(this);
    boxClickModeLayout = new QVBoxLayout;
    boxClickModeLayout->addWidget(selectionModeButton);
    boxClickModeLayout->addWidget(newAreaModeButton);
//    boxClickModeLayout->addStretch(1);
    boxClickMode->setLayout(boxClickModeLayout);

        //Point-per-keyboard
    labelKeyboardInput = new QLabel("Punkte per Tastatur eingeben",this);

    labelXValue = new QLabel("x-Koordinate",this);
    inputXValue = new QDoubleSpinBox(this);
    inputXValue->setMinimum(0);
    inputXValue->setMaximum(1);
    inputXValue->setSingleStep(0.1);

    labelYValue = new QLabel("y-Koordinate",this);
    inputYValue = new QDoubleSpinBox(this);
    inputYValue->setMinimum(0);
    inputYValue->setMaximum(1);
    inputYValue->setSingleStep(0.1);

    confirmButton = new QPushButton("Punkt hinzufügen",this);

        //Undo-Redo-Box
    areaModeButton = new QRadioButton("Gebietsweise",this);
    areaModeButton->setChecked(true);
    areaModeButton->setEnabled(false);

    pointModeButton = new QRadioButton("Punktweise",this);
    pointModeButton->setEnabled(false);

    undoButton = new QPushButton(this);
    undoButton->setIcon(QIcon(":/Icons/undo"));
    undoButton->setText("Rückgängig");
    undoButton->setIconSize(QSize(15,15));
    undoButton->setEnabled(false);

    redoButton = new QPushButton(this);
    redoButton->setIcon(QIcon(":/Icons/redo"));
    redoButton->setText("Wiederholen");
    redoButton->setIconSize(QSize(15,15));
    redoButton->setEnabled(false);

    boxUndoRedo = new QGroupBox(this);
    boxUndoRedoLayout = new QVBoxLayout;
    boxUndoRedoLayout->addWidget(areaModeButton);
    boxUndoRedoLayout->addWidget(pointModeButton);
    boxUndoRedoLayout->addWidget(undoButton);
    boxUndoRedoLayout->addWidget(redoButton);
//    boxUndoRedoLayout->addStretch(1);
    boxUndoRedo->setLayout(boxUndoRedoLayout);

        //Abbruch Knöpfe
    discardAreaButton = new QPushButton("Gebiet abbrechen",this);
    discardAreaButton->setEnabled(false);

    deleteAreaButton = new QPushButton("Gebiet löschen",this);
    deleteAreaButton->setEnabled(false);

    clearAreasButton = new QPushButton("Alle Gebiete löschen",this);
    clearAreasButton->setEnabled(false);

        //Platzhalter
    spacerItem1 = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);
    spacerItem2 = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);


        //Layouts initialisieren
    subLayout = new QGridLayout();

    subLayout->addItem(spacerItem1,0,0);
    subLayout->addWidget(boxClickMode,1,0,2,2);
    subLayout->addWidget(labelKeyboardInput,3,0,1,2);
    subLayout->addWidget(labelXValue,4,0);
    subLayout->addWidget(inputXValue,4,1);
    subLayout->addWidget(labelYValue,5,0);
    subLayout->addWidget(inputYValue,5,1);
    subLayout->addWidget(confirmButton,6,0,1,2);
    subLayout->addWidget(boxUndoRedo,8,0,4,2);
    subLayout->addWidget(discardAreaButton,12,0,1,2);
    subLayout->addWidget(deleteAreaButton,13,0,1,2);
    subLayout->addWidget(clearAreasButton,14,0,1,2);
    subLayout->addItem(spacerItem2,15,0,1,2);

    subLayout->setColumnStretch(0,120);
    subLayout->setColumnStretch(1,100);

    layout = new QGridLayout(this);

    layout->addWidget(topLabel,0,0,1,3);
    layout->addWidget(table,1,0,6,1);
    layout->addWidget(boxReorderButtons,1,1);
    layout->addWidget(plate,1,2,6,1);
    layout->addLayout(subLayout,1,4);

    layout->setColumnStretch(2,50);

    // interne slots & signal verbinden
    connect(plate,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(mouseClickOnPlateSlot(QMouseEvent*)));

    connect(table,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(tableItemChangeSlot(QTableWidgetItem*)));
    connect(table,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(tableItemClickSlot(QTableWidgetItem*)));

    connect(clearAreasButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
    connect(confirmButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
    connect(undoButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
}

void presentation::AreaWidget::drawPartialArea(QVector<double> const & partialAreaX,
                                          QVector<double> const & partialAreaY)
{
    if(plate->graphCount() <= table->rowCount()-1)
        plate->addGraph();
    plate->graph(plate->graphCount()-1)->setData(partialAreaX,partialAreaY);
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    QPen myPen(Qt::green);
    myScatter.setPen(myPen);
    myScatter.setBrush(Qt::green);
    myScatter.setSize(5);
    plate->graph(plate->graphCount()-1)->setScatterStyle(myScatter);
    myPen.setWidth(2* myPen.width());
    plate->graph(plate->graphCount()-1)->setPen(myPen);
    plate->graph(plate->graphCount()-1)->setVisible(true);
    plate->replot();
}

void presentation::AreaWidget::setController(Controller *controller)
{
    this->controller = controller;

    connect(this,SIGNAL(areaClicked(double,double,QSize,double,model::SimulationSetup::AreaType)),
            controller,SLOT(areaClickSlot(double,double,QSize,double,model::SimulationSetup::AreaType)));
    connect(this,SIGNAL(areaValueChanged(int,double,bool,model::SimulationSetup::AreaType)),
            controller,SLOT(areaValueChangedSlot(int,double,bool,model::SimulationSetup::AreaType)));
//    connect(this,SIGNAL(clearAreas(model::SimulationSetup::AreaType)),
//            controller,SLOT(clearAreasSlot(model::SimulationSetup::AreaType)));
    connect(this,SIGNAL(undoArea(model::SimulationSetup::AreaType)),controller,
            SLOT(undoAreaSlot(model::SimulationSetup::AreaType)));
}

void presentation::AreaWidget::setModel(model::Model *model)
{
    this->model = model;
    // colorScale Range
    QCPRange range(model::SimulationSetup::AreaMinValue[type]/valueShift,
                    model::SimulationSetup::AreaMaxValue[type]/valueShift);

    // Updaten des Background-Wertes
    double bgValue = model->getSimulationSetup()->getAreaBackgroundValue(type)/valueShift;
    table->item(0,AreaWidget::ColumnValue)->setText(QString::number(bgValue));
    plate->axisRect()->setBackground(QBrush(colorScale->gradient().color(bgValue,range,false)));
}

void presentation::AreaWidget::update()
{
    // colorScale Range
    QCPRange range(model::SimulationSetup::AreaMinValue[type]/valueShift,
                    model::SimulationSetup::AreaMaxValue[type]/valueShift);

    // Updaten des Background-Wertes
    double bgValue = model->getSimulationSetup()->getAreaBackgroundValue(type)
            /valueShift;
    table->item(0,AreaWidget::ColumnValue)->setText(QString::number(bgValue));
    plate->axisRect()->setBackground(QBrush(colorScale->gradient().color(bgValue,range,false)));

    int hSCount = model->getSimulationSetup()->getAreaCount(type);
    int rowCount = table->rowCount();
    int tmpBound = hSCount+1 <= rowCount ? hSCount+1 : rowCount;

    table->setRowCount(hSCount+1);

    QList<model::Area*> const & areas = model->getSimulationSetup()->getAreas(type);
    QList<model::Area*>::const_iterator it = areas.begin();
    int i = 1;
    double value;
    for(; i < tmpBound; ++i, ++it)
    {
        value = (*it)->getValue()/valueShift;

        table->item(i,AreaWidget::ColumnID)->setText(QString::number((*it)->getID()));

        table->item(i,AreaWidget::ColumnValue)->setText(QString::number(value));

        table->item(i,AreaWidget::ColumnVisibility)->setCheckState(
                    visibilities.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
    }
    if(hSCount+1 >= rowCount)
    {
        for(; i < hSCount+1; ++i, ++it)
        {
            QTableWidgetItem * tmpItemPtr = new QTableWidgetItem(QString::number((*it)->getID()));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            table->setItem(i,AreaWidget::ColumnID,tmpItemPtr);

            value = (*it)->getValue()/valueShift;
            tmpItemPtr = new QTableWidgetItem(QString::number(value));
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            table->setItem(i,AreaWidget::ColumnValue,tmpItemPtr);

            tmpItemPtr = new QTableWidgetItem();
            tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            tmpItemPtr->setCheckState(visibilities.value((*it)->getID(),true) ? Qt::Checked : Qt::Unchecked);
            table->setItem(i,AreaWidget::ColumnVisibility,tmpItemPtr);
        }
    }
    else
    {
        for(int j = rowCount-1; j >= i; --j)
        {
            delete table->takeItem(j,AreaWidget::ColumnID);
            delete table->takeItem(j,AreaWidget::ColumnValue);
            delete table->takeItem(j,AreaWidget::ColumnVisibility);
        }
    }

    for(int j = 0; j < rowCount-1; ++j)
        plate->graph(j)->setVisible(false);

    for(int j = plate->graphCount(); j < hSCount; ++j)
        plate->addGraph();

    it = areas.begin();
    for(int j = 0; j < hSCount; ++j,++it)
    {
        QVector<double> x,y;
        (*it)->getPoints(x,y);
        value = (*it)->getValue()/valueShift;
        QColor color(colorScale->gradient().color(value,range,false));
        plate->graph(j)->setData(x,y);
        plate->graph(j)->setPen(QPen(Qt::black));
        plate->graph(j)->setBrush(QBrush(color,Qt::SolidPattern));
        plate->graph(j)->setScatterStyle(QCPScatterStyle::ssNone);
        plate->graph(j)->setVisible(visibilities.value((*it)->getID(),true));
    }
    plate->replot();
    controller->testPartialArea(type);

    if(hSCount==0)
    {
        undoButton->setEnabled(false);
        clearAreasButton->setEnabled(false);
    }
    else
    {
        undoButton->setEnabled(true);
        clearAreasButton->setEnabled(false);
    }
}

void presentation::AreaWidget::buttonMapperSlot()
{
    QObject * _sender = sender();
    if(_sender == clearAreasButton)
    {
        emit clearAreas(type);
        return;
    }
    if(_sender == confirmButton)
    {
        emit areaClicked(inputXValue->value(), inputYValue->value(),
                         plate->size(),valueShift,type);
    }
    if(_sender == undoButton)
    {
        emit undoArea(type);
        return;
    }
}

void presentation::AreaWidget::mouseClickOnPlateSlot(QMouseEvent *event)
{
    double x = plate->xAxis->pixelToCoord(event->x()),
            y = plate->yAxis->pixelToCoord(event->y());
    emit areaClicked(x,y,plate->size(),valueShift,type);
}

void presentation::AreaWidget::tableItemChangeSlot(QTableWidgetItem *item)
{
    int column = table->column(item);
    if(column != AreaWidget::ColumnValue)
        return;
    int row = table->row(item);

    bool ok;
    double newValue = table->item(row,AreaWidget::ColumnValue)->text().toDouble(&ok)*valueShift;
    emit areaValueChanged(row,newValue,ok,type);

}

void presentation::AreaWidget::tableItemClickSlot(QTableWidgetItem *item)
{
    int column = table->column(item);
    if(column != AreaWidget::ColumnVisibility)
        return;
    int row = table->row(item);

    if(row == 0)
        return;

    bool visible = table->item(row,AreaWidget::ColumnVisibility)->
            checkState() == Qt::Checked ? true : false;
    bool ok;
    int id = table->item(row,AreaWidget::ColumnID)->text().toInt(&ok);
    assert(ok);
    visibilities[id] = visible;
    plate->graph(row-1)->setVisible(visible);
    plate->replot();
}
