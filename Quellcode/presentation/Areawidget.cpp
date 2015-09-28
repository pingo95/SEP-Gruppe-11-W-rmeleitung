#include "Areawidget.h"

#include "Controller.h"

presentation::AreaWidget::AreaWidget(QWidget *parent,
                                     model::SimulationSetup::AreaType type,
                                     QString const name, QString const unit,
                                     double const valueShift)
    : QWidget(parent), controller(NULL), model(NULL), name(name), partialArea(false),
      selectedAreaID(-1), type(type), unit(unit), updating(false), valueShift(valueShift)
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
    table->setColumnCount(2);
    table->setFixedWidth(117);
//    table->horizontalScrollBar()->setVisible(false);


    QString combinedUnit = "\n[";
    if(valueShift != 1)
        combinedUnit += QString::number(valueShift,'e',0);
    combinedUnit += " " + unit + "]";

    tableHeader <<"ID"<<"Wert"+combinedUnit;
    table->setHorizontalHeaderLabels(tableHeader);
    table->horizontalHeader()->setSectionsClickable(false);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->horizontalHeader()->setMinimumHeight(45);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    table->setColumnWidth(0,30);
    table->setColumnWidth(1,85);

    QTableWidgetItem * tmpItemPtr = new QTableWidgetItem("0");
    tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    table->setItem(0,AreaWidget::ColumnID,tmpItemPtr);
    tmpItemPtr = new QTableWidgetItem("0"); //<-- in setModel updaten!
    tmpItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    table->setItem(0,AreaWidget::ColumnValue,tmpItemPtr);

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

        //Point-per-keyboard
    labelKeyboardInput = new QLabel("Punkte per Tastatur eingeben",this);

    labelXValue = new QLabel("x-Koordinate",this);
    inputXValue = new QDoubleSpinBox(this);
    inputXValue->setMinimum(0);
    inputXValue->setMaximum(1);
    inputXValue->setSingleStep(0.1);
    inputXValue->setDecimals(3);

    labelYValue = new QLabel("y-Koordinate",this);
    inputYValue = new QDoubleSpinBox(this);
    inputYValue->setMinimum(0);
    inputYValue->setMaximum(1);
    inputYValue->setSingleStep(0.1);
    inputYValue->setDecimals(3);

    confirmButton = new QPushButton("Punkt hinzufügen",this);

        //Undo-Redo-Box
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
    boxUndoRedoLayout->addWidget(undoButton);
    boxUndoRedoLayout->addWidget(redoButton);
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
    subLayout->addWidget(labelKeyboardInput,1,0,1,2);
    subLayout->addWidget(labelXValue,2,0);
    subLayout->addWidget(inputXValue,2,1);
    subLayout->addWidget(labelYValue,3,0);
    subLayout->addWidget(inputYValue,3,1);
    subLayout->addWidget(confirmButton,4,0,1,2);
    subLayout->addWidget(boxUndoRedo,5,0,4,2);
    subLayout->addWidget(discardAreaButton,9,0,1,2);
    subLayout->addWidget(deleteAreaButton,10,0,1,2);
    subLayout->addWidget(clearAreasButton,11,0,1,2);
    subLayout->addItem(spacerItem2,12,0,1,2);

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
    connect(table,SIGNAL(itemSelectionChanged()),this,SLOT(tableSelectionChangeSlot()));

    connect(clearAreasButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
    connect(confirmButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
    connect(deleteAreaButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
    connect(discardAreaButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
    connect(redoButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
    connect(undoButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));

    connect(allUpButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
    connect(upButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
    connect(downButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
    connect(allDownButton,SIGNAL(clicked(bool)),this,SLOT(buttonMapperSlot()));
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

    if(partialAreaX.size() > 0)
    {
        partialArea = true;
        table->selectRow(0);
        table->setSelectionMode(QAbstractItemView::NoSelection);
        clearAreasButton->setEnabled(false);
        discardAreaButton->setEnabled(true);
        undoButton->setEnabled(true);
    }
    else
    {
        partialArea = false;
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        if(table->rowCount() > 1)
            clearAreasButton->setEnabled(true);
        discardAreaButton->setEnabled(false);
        undoButton->setEnabled(false);
    }
    redoButton->setEnabled(controller->getRedoPossible(type));
}

void presentation::AreaWidget::setController(Controller *controller)
{
    this->controller = controller;

    connect(this,SIGNAL(areaClicked(double,double,QSize,double,bool,model::SimulationSetup::AreaType)),
            controller,SLOT(areaClickSlot(double,double,QSize,double,bool,model::SimulationSetup::AreaType)));
    connect(this,SIGNAL(areaValueChanged(int,double,bool,model::SimulationSetup::AreaType)),
            controller,SLOT(areaValueChangedSlot(int,double,bool,model::SimulationSetup::AreaType)));
    connect(this,SIGNAL(clearAreas(model::SimulationSetup::AreaType)),
            controller,SLOT(clearAreasSlot(model::SimulationSetup::AreaType)));
    connect(this,SIGNAL(deleteArea(int,model::SimulationSetup::AreaType)),
            controller,SLOT(deleteAreaSlot(int,model::SimulationSetup::AreaType)));
    connect(this,SIGNAL(discardNewArea(model::SimulationSetup::AreaType)),
            controller,SLOT(discardAreaSlot(model::SimulationSetup::AreaType)));
    connect(this,SIGNAL(redo(model::SimulationSetup::AreaType)),
            controller,SLOT(redoSlot(model::SimulationSetup::AreaType)));
    connect(this,SIGNAL(reorderArea(int,int,model::SimulationSetup::AreaType)),
            controller,SLOT(reorderAreaSlot(int,int,model::SimulationSetup::AreaType)));
    connect(this,SIGNAL(undo(model::SimulationSetup::AreaType)),
            controller,SLOT(undoSlot(model::SimulationSetup::AreaType)));
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
    if(updating)
        return;
    updating = true;
    // colorScale Range
    QCPRange range(model::SimulationSetup::AreaMinValue[type]/valueShift,
                    model::SimulationSetup::AreaMaxValue[type]/valueShift);

    // Updaten des Background-Wertes
    double bgValue = model->getSimulationSetup()->getAreaBackgroundValue(type)
            /valueShift;
    table->item(0,AreaWidget::ColumnValue)->setText(QString::number(bgValue));
    plate->axisRect()->setBackground(QBrush(colorScale->gradient().color(bgValue,range,false)));

    int areaCount = model->getSimulationSetup()->getAreaCount(type);
    int rowCount = table->rowCount();
    int tmpBound = areaCount+1 <= rowCount ? areaCount+1 : rowCount;

    table->setRowCount(areaCount+1);

    QList<model::Area*> const & areas = model->getSimulationSetup()->getAreas(type);
    QList<model::Area*>::const_iterator it = areas.begin();
    int i = 1;
    double value;
    for(; i < tmpBound; ++i, ++it)
    {
        value = (*it)->getValue()/valueShift;

        table->item(i,AreaWidget::ColumnID)->setText(QString::number((*it)->getID()));

        table->item(i,AreaWidget::ColumnValue)->setText(QString::number(value));
    }
    if(areaCount+1 >= rowCount)
    {
        for(; i < areaCount+1; ++i, ++it)
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
        }
    }
    else
    {
        for(int j = rowCount-1; j >= i; --j)
        {
            delete table->takeItem(j,AreaWidget::ColumnID);
            delete table->takeItem(j,AreaWidget::ColumnValue);
        }
    }

    for(int j = 0; j < rowCount-1; ++j)
        plate->graph(j)->setVisible(false);

    for(int j = plate->graphCount(); j < areaCount; ++j)
        plate->addGraph();

    it = areas.begin();
    for(int j = 0; j < areaCount; ++j,++it)
    {
        QVector<double> x,y;
        (*it)->getPoints(x,y);
        value = (*it)->getValue()/valueShift;
        QColor color(colorScale->gradient().color(value,range,false));
        plate->graph(j)->setData(x,y);
        plate->graph(j)->setPen(QPen(Qt::black));
        plate->graph(j)->setBrush(QBrush(color,Qt::SolidPattern));
        plate->graph(j)->setScatterStyle(QCPScatterStyle::ssNone);
        plate->graph(j)->setVisible(true);
    }
    plate->replot();
    if(partialArea)
    {
        QVector<double> x,y;
        controller->getPartialArea(x,y);
        drawPartialArea(x,y);
    }
    else
    {
        table->selectRow(findRow(selectedAreaID));
        tableSelectionChangeSlot();
    }

    if(areaCount == 0)
        clearAreasButton->setEnabled(false);
    else
        clearAreasButton->setEnabled(true);
    updating = false;
}

void presentation::AreaWidget::buttonMapperSlot()
{
    QObject * _sender = sender();
    if(_sender == allUpButton)
    {
        emit reorderArea(findRow(selectedAreaID)-1,2,type);
        return;
    }
    if(_sender == upButton)
    {
        emit reorderArea(findRow(selectedAreaID)-1,1,type);
        return;
    }
    if(_sender == downButton)
    {
        emit reorderArea(findRow(selectedAreaID)-1,-1,type);
        return;
    }
    if(_sender == allDownButton)
    {
        emit reorderArea(findRow(selectedAreaID)-1,-2,type);
        return;
    }
    if(_sender == clearAreasButton)
    {
        emit clearAreas(type);
        return;
    }
    if(_sender == confirmButton)
    {
        emit areaClicked(inputXValue->value(), inputYValue->value(),
                         plate->size(),valueShift,false,type);
        return;
    }
    if(_sender == deleteAreaButton)
    {
        emit deleteArea(findRow(selectedAreaID)-1,type);
        table->selectRow(0);
        selectedAreaID = -1;
        return;
    }
    if(_sender == discardAreaButton)
    {
        emit discardNewArea(type);
        return;
    }
    if(_sender == redoButton)
    {
        emit redo(type);
        return;
    }
    else
    {
        emit undo(type);
        return;
    }
}

void presentation::AreaWidget::mouseClickOnPlateSlot(QMouseEvent *event)
{
    double x = plate->xAxis->pixelToCoord(event->x()),
            y = plate->yAxis->pixelToCoord(event->y());
    // QCustomPlots sind etwas neben dem Rand klickbar, dies wird hier abgefangen
    if(x < 0)
        x = 0;
    else
        if(x > 1)
            x = 1;
    if(y < 0)
        y = 0;
    else
        if(y > 1)
            y = 1;
    if(event->button() == Qt::LeftButton)
        emit areaClicked(x,y,plate->size(),valueShift,true,type);
    else
        if(!partialArea)
            table->selectRow(findRow(model->getSimulationSetup()->getContainingAreaID(x,y,type)));
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

void presentation::AreaWidget::tableSelectionChangeSlot()
{
    unhighlightGraph();
    int selectedRow;
    if(partialArea || table->selectedItems().isEmpty())
        selectedRow = 0;
    else
        selectedRow = table->row(table->selectedItems().first());
    if(selectedRow == 0)
    {
        selectedAreaID = -1;
        allUpButton->setEnabled(false);
        upButton->setEnabled(false);
        allDownButton->setEnabled(false);
        downButton->setEnabled(false);
        return;
    }

    bool ok;
    int id = table->item(selectedRow,AreaWidget::ColumnID)->text().toInt(&ok);
    assert(ok);
    selectedAreaID = id;
    highlightGraph();

    if(selectedRow > 1)
    {
        allUpButton->setEnabled(true);
        upButton->setEnabled(true);
    }
    else
    {
        allUpButton->setEnabled(false);
        upButton->setEnabled(false);
    }
    if(selectedRow < table->rowCount()-1)
    {
        allDownButton->setEnabled(true);
        downButton->setEnabled(true);
    }
    else
    {
        allDownButton->setEnabled(false);
        downButton->setEnabled(false);
    }
}

int presentation::AreaWidget::findRow(int id)
{
    for(int i = 1; i < table->rowCount(); ++i)
    {
        bool ok;
        int currentID = table->item(i,AreaWidget::ColumnID)->text().toInt(&ok);
        assert(ok);
        if(currentID == id)
            return i;
    }
    return 0;
}

void presentation::AreaWidget::highlightGraph()
{
    int index = findRow(selectedAreaID)-1;
    if(index == -1)
        return;
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    QPen myPen(Qt::cyan);
    myScatter.setPen(myPen);
    myScatter.setBrush(Qt::cyan);
    myScatter.setSize(5);
    plate->graph(index)->setScatterStyle(myScatter);
    myPen.setWidth(2* myPen.width());
    plate->graph(index)->setPen(myPen);
    plate->replot();
    deleteAreaButton->setEnabled(true);
}

void presentation::AreaWidget::unhighlightGraph()
{
    int index = findRow(selectedAreaID)-1;
    if(index == -1)
        return;
    plate->graph(index)->setPen(QPen(Qt::black));
    plate->graph(index)->setScatterStyle(QCPScatterStyle::ssNone);
    plate->replot();
    deleteAreaButton->setEnabled(false);
}
