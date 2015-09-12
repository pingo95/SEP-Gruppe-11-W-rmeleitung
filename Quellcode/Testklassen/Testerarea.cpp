#include "Testerarea.h"
#include "../model/Area.h"


TesterArea::TesterArea()
{

}
TesterArea::~TesterArea()
{
//    delete ptr;
}

QWidget *TesterArea::simpleRobustnessTesterInsidePoint()
{
    tabs = new QTabWidget();
    int numberTests = 5;
    ptr.resize(numberTests);
    grid.resize(numberTests);
    plot.resize(numberTests);
    log.resize(numberTests);
    for(int t = 0; t < numberTests; ++t)
    {
        ptr[t] = new QWidget();
        grid[t] = new QGridLayout(ptr[t]);
        plot[t] = new QCustomPlot(ptr[t],false);
        log[t] = new QTextEdit(ptr[t]);
        grid[t]->addWidget(plot[t],0,0);
        grid[t]->addWidget(log[t],1,0);

        plot[t]->setMinimumSize(400,400);
        log[t]->setMinimumSize(400,300);
        log[t]->setReadOnly(true);
        double tmp = (double) rand() / RAND_MAX;
        double tmp2 = (double) rand() /RAND_MAX;
        double max,min;
        if(t == 0)
        {
            max = 1;
            min = 0;
        }
        else
        {
            max = tmp > tmp2 ? tmp : tmp2;
            min = tmp < tmp2 ? tmp : tmp2;
        }
        double nom = (max+min)/2;
        double offset = (max - nom) / 8;

        QVector<double> x, y, teilWerte, xTest(49), yTest(49);
        QVector<bool> teilErgebnisse,sollErgebnisse(49);
        x << min << max << max << min << min;
        y << min << min << max << max << min;
        teilWerte       << min-offset << min  << min+offset << nom  << max-offset << max  << max+offset;
        teilErgebnisse  << false      << true << true       << true << true       << true << false;

//        xTest        << nom     << nom        << nom  << nom              << max-offset << max  << max+offset       << nom        << nom  << nom              << min+offset << min  << min-offset;
//        yTest        << nom     << min+offset << min  << min-offset       << nom        << nom  << nom              << max-offset << max  << max+offset       << nom        << nom  << nom;
//        wantedResult << true    << true       << true << false            << true       << true << false            << true       << true << false            << true       << true << false;

        for(int a = 0; a < 7; ++a)
            for(int b = 0; b < 7; ++b)
            {
                xTest[7*a + b] = teilWerte[a];
                yTest[7*a + b] = teilWerte[b];
                sollErgebnisse[7*a + b] = teilErgebnisse[a] && teilErgebnisse[b];
            }

        plot[t]->xAxis->setRange(min-2*offset,max+2*offset);
        plot[t]->yAxis->setRange(min-2*offset,max+2*offset);
        plot[t]->addGraph();
        plot[t]->graph(0)->setData(x,y);
        QPen myPen(Qt::red);
        myPen.setWidth(2*myPen.width());
        plot[t]->graph(0)->setPen(myPen);
        plot[t]->addGraph();
        plot[t]->graph(1)->setData(xTest,yTest);
        QCPScatterStyle myScatter;
        myScatter.setShape(QCPScatterStyle::ssCircle);
        QPen myPen2(Qt::green);
        myScatter.setPen(myPen2);
        myScatter.setBrush(Qt::green);
        myScatter.setSize(5);
        plot[t]->graph(1)->setScatterStyle(myScatter);
        plot[t]->graph(1)->setPen(Qt::NoPen);
        plot[t]->replot();

        QString logText = "Einfacher Robustness Test der InsidePoint-Funktionen der Area-Klasse:\n"
                          "Generierte Werte: \n\tmax = " + QString::number(max) + "\n\tmin = "
                          + QString::number(min) + "\n\tnom = " + QString::number(nom) +
                          "\n\toffset = " + QString::number(offset) +
                          "\nValidierung des Test-Gebietes: ";
        if(model::Area::validateArea(x,y))
        {
            logText += "Erfolgreich\n\nBeginn der Tests:\n";
            int count = 0;
            model::Area testArea(x,y,0,0);
            for(int i = 0; i < 49; ++i)
            {
                logText += "Test Nummer " + QString("%1").arg(i+1,2,10,QChar('0')) + ": x = "
                + QString::number(xTest[i],'f',5) + ", y = " + QString::number(yTest[i],'f',5);
                if(testArea.insidePoint(xTest[i],yTest[i]) == sollErgebnisse[i])
                {
                    logText += " Erfolgreich\n";
                    ++count;
                }
                else
                    logText += " Nicht erfolgreich\n";
            }
            logText += "Zusammenfassung: " + QString::number(count) + " von 49 Tests erfolgreich.";
        }
        else
        {
            logText += "Nicht erfolgreich\n\nAbbruch";
        }
        log[t]->setText(logText);
        tabs->addTab(ptr[t],"Testserie Nr. " + QString::number(t+1));
    }
    return tabs;
}
