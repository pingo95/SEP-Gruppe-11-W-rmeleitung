#include "Testergrid.h"
#include "../model/Simulationsetup.h"

model::TesterGrid::TesterGrid()
{

}

model::TesterGrid::~TesterGrid()
{

}

QWidget * model::TesterGrid::areasToGridTest1()
{
    tabs = new QTabWidget();
    int numberTests = 5;
    ptrs.resize(numberTests);
    grids.resize(numberTests);
    plots.resize(numberTests);
    tables.resize(numberTests);
    logs.resize(numberTests);
    scales.resize(numberTests);
    for(int t = 0; t < numberTests; ++t)
    {
        ptrs[t] = new QWidget();
        grids[t] = new QGridLayout(ptrs[t]);
        plots[t] = new QCustomPlot(ptrs[t],false);
        tables[t] = new QTableWidget(ptrs[t]);
        logs[t] = new QTextEdit(ptrs[t]);
        grids[t]->addWidget(plots[t],0,0,1,2);
        grids[t]->addWidget(tables[t],1,0);
        grids[t]->addWidget(logs[t],1,1);

        plots[t]->setMinimumSize(500,500);
        tables[t]->setMinimumSize(250,200);
        logs[t]->setMinimumSize(250,200);
        logs[t]->setReadOnly(true);

        double min = 0, max = 500;

            //Für Platte
        QVector<double> ticks,ticksColorBar;
        QVector<QString> tickLabels,tickLabelsColorBar;
        double temperatureSpan = max - min;
        for(int i = 0; i < 11; ++i)
        {
            ticks << (double) i / 10;
            tickLabels << QString::number(ticks[i]);

            ticksColorBar << temperatureSpan * ticks[i] + min;
            tickLabelsColorBar << QString::number(ticksColorBar[i]);
        }

            //Platte xAchse unten
        plots[t]->xAxis->setAutoTicks(false);
        plots[t]->xAxis->setAutoTickLabels(false);
        plots[t]->xAxis->setTickVector(ticks);
        plots[t]->xAxis->setTickVectorLabels(tickLabels);
        plots[t]->xAxis->setRange(0,1);
        plots[t]->xAxis->grid()->setSubGridVisible(true);
            //Platte yAchse links
        plots[t]->yAxis->setAutoTicks(false);
        plots[t]->yAxis->setAutoTickLabels(false);
        plots[t]->yAxis->setTickVector(ticks);
        plots[t]->yAxis->setTickVectorLabels(tickLabels);
        plots[t]->yAxis->setRange(0,1);
        plots[t]->yAxis->grid()->setSubGridVisible(true);
            //Platte xAchse oben
        plots[t]->xAxis2->setVisible(true);
        plots[t]->xAxis2->setAutoTicks(false);
        plots[t]->xAxis2->setAutoTickLabels(false);
        plots[t]->xAxis2->setTickVector(ticks);
        plots[t]->xAxis2->setTickVectorLabels(tickLabels);
        plots[t]->xAxis2->setRange(0,1);
        plots[t]->xAxis2->grid()->setSubGridVisible(true);
            //Platte yAchse rechts
        plots[t]->yAxis2->setVisible(true);
        plots[t]->yAxis2->setAutoTicks(false);
        plots[t]->yAxis2->setAutoTickLabels(false);
        plots[t]->yAxis2->setTickVector(ticks);
        plots[t]->yAxis2->setTickVectorLabels(tickLabels);
        plots[t]->yAxis2->setRange(0,1);
        plots[t]->yAxis2->grid()->setSubGridVisible(true);

            // Color Scale
        scales[t] = new QCPColorScale(plots[t]);
        plots[t]->plotLayout()->addElement(0,1,scales[t]);
        scales[t]->setLabel("Temperatur [K]");
        QCPMarginGroup * group = new QCPMarginGroup(plots[t]);
        plots[t]->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, group);
        scales[t]->setMarginGroup(QCP::msBottom | QCP::msTop, group);
        scales[t]->setGradient(QCPColorGradient::gpThermal);
        scales[t]->setDataRange(QCPRange(min,max));
        scales[t]->axis()->setAutoTicks(false);
        scales[t]->axis()->setAutoTickLabels(false);
        scales[t]->axis()->setTickVector(ticksColorBar);
        scales[t]->axis()->setTickVectorLabels(tickLabelsColorBar);
        scales[t]->axis()->setRange(QCPRange(min,max));

        QList<Area*> areas;
        int n = 5 * t + 10;
        double deltaX = 1./((double)n-1);
        QVector<double> x(5),y(5);
        for(int i = 1; i < n-1; ++i)
            for(int j = 1; j < n-1; ++j)
            {
                x[0] = ((double)j-0.5) * deltaX; y[0] = ((double)i-0.5) * deltaX;
                x[1] = ((double)j+0.5) * deltaX; y[1] = ((double)i-0.5) * deltaX;
                x[2] = ((double)j+0.5) * deltaX; y[2] = ((double)i+0.5) * deltaX;
                x[3] = ((double)j-0.5) * deltaX; y[3] = ((double)i+0.5) * deltaX;
                x[4] = ((double)j-0.5) * deltaX; y[4] = ((double)i-0.5) * deltaX;
                double value = qMin((int)max,(int)(qAbs(i-n/2)*qAbs(j-n/2))*5);
                areas.append(new Area(x,y,value,SimulationSetup::AreaHeatSource));
                QColor color(scales[t]->gradient().color(value,QCPRange(min,max),false));
                plots[t]->addGraph();
                plots[t]->graph(plots[t]->graphCount()-1)->setData(x,y);
                plots[t]->graph(plots[t]->graphCount()-1)->setPen(QPen(Qt::black));
                plots[t]->graph(plots[t]->graphCount()-1)->setBrush(QBrush(color,Qt::SolidPattern));
                plots[t]->graph(plots[t]->graphCount()-1)->setScatterStyle(QCPScatterStyle::ssNone);
                plots[t]->graph(plots[t]->graphCount()-1)->setVisible(true);
            }
        plots[t]->replot();

        QVector<double> thermalDiffusivitiesGrid(n*n,0);
        QList<Area*>::const_iterator it = areas.begin();
        for(; it != areas.end(); ++it)
        {
            Area* thermalDiffusivity = *it;
            double diffusivity = thermalDiffusivity->getValue();
            double xMin, xMax, yMin, yMax;
            thermalDiffusivity->getTransitiveRectangle(xMin,xMax,yMin,yMax);
            long xLBound = ceil(xMin/deltaX),
                    xUBound = floor(xMax/deltaX),
                    yLBound = ceil(yMin/deltaX),
                    yUBound = floor(yMax/deltaX);
            for(long i = xLBound; i <= xUBound; ++i)
                for(long j = yLBound; j <= yUBound; ++j)
                    if(thermalDiffusivity->insidePoint(i*deltaX,j*deltaX))
                        thermalDiffusivitiesGrid[i+j*n] = diffusivity;
        }

        QString logText = "Test des Mapping von Gebieten auf Grid 1\n"
                          "Gridgröße: " + QString::number(n) +
                          "\nVergleich der Werte:\n";
        tables[t]->setColumnCount(n);
        tables[t]->setRowCount(n);
        for(int i = n-1; i >= 0; --i)
        {
            tables[t]->setColumnWidth(n-1-i,70);
            for(int j = 0; j < n; ++j)
            {
                QTableWidgetItem * tmpItemPtr = new QTableWidgetItem(QString::number(thermalDiffusivitiesGrid[i+j*n]));
                tmpItemPtr->setFlags(Qt::ItemIsEnabled);
                tmpItemPtr->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tables[t]->setItem(n-1-i,j,tmpItemPtr);
            }
        }
        tables[t]->horizontalHeader()->setSectionsClickable(false);
        tables[t]->verticalHeader()->setSectionsClickable(false);
        int count = 0;
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
            {
                logText += "Aktuelle Koordinaten:\nx: " + QString::number(i*deltaX) +
                        "\ny: " + QString::number(j*deltaX) + "\nSoll-Wert: ";
                double value = thermalDiffusivitiesGrid[i+j*n];
                if(i == 0 || i == n-1 || j == 0 || j == n-1)
                {
                    logText += QString::number(0) + "\nIst-Wert: " +
                            QString::number(value) + "\n-->Test " +
                            ((value == 0) ? "erfolgreich\n\n" : "nicht erfolgreich\n\n");
                    if(value == 0)
                        ++count;
                }
                else
                {
                    double sValue = qMin((int)max,(int)(qAbs(i-n/2)*qAbs(j-n/2))*5);
                    logText += QString::number(sValue) + "\nIst-Wert: " +
                            QString::number(value) + "\n-->Test " +
                            ((value == sValue) ? "erfolgreich\n\n" : "nicht erfolgreich\n\n");
                    if(value == sValue)
                        ++count;
                }
            }
        logText += QString::number(count) + " von " + QString::number(n*n) +
                " Tests erfolgreich.";
        logs[t]->setText(logText);
        tabs->addTab(ptrs[t],"Testserie Nr. " + QString::number(t+1));
    }
    return tabs;
}
