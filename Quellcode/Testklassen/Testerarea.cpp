#include "Testerarea.h"
#include "../model/Area.h"

model::TesterArea::TesterArea()
{

}

model::TesterArea::~TesterArea()
{
//    delete ptr;
}

void model::TesterArea::rotateKoords(QVector<double> &x, QVector<double> &y, int angle)
{
    double s,c;
    switch(angle % 360)
    {
    case 0:
        s = 0;
        c = 1;
        break;
    case 90:
        s = 1;
        c = 0;
        break;
    case 180:
        s = 0;
        c = -1;
        break;
    case 270:
        s = -1;
        c = 0;
        break;
    default:
        s = sin((double)angle);
        c = cos((double)angle);
    }
    double xOld, yOld;
    for(int i = 0; i < x.size(); ++i)
    {
        xOld = x[i];
        yOld = y[i];
        x[i] = c*xOld + s*yOld;
        y[i] = -s*xOld + c*yOld;
    }
}

QWidget * model::TesterArea::robustnessWorstCaseTesterInsidePoint()
{
    tabs = new QTabWidget();
    int numberTests = 10;
    ptrs.resize(numberTests);
    grids.resize(numberTests);
    plots.resize(numberTests);
    logs.resize(numberTests);
    for(int t = 0; t < numberTests; ++t)
    {
        ptrs[t] = new QWidget();
        grids[t] = new QGridLayout(ptrs[t]);
        plots[t] = new QCustomPlot(ptrs[t],false);
        logs[t] = new QTextEdit(ptrs[t]);
        grids[t]->addWidget(plots[t],0,0);
        grids[t]->addWidget(logs[t],1,0);

        plots[t]->setMinimumSize(500,500);
        logs[t]->setMinimumSize(500,200);
        logs[t]->setReadOnly(true);
        double tmp = (double) qrand() / RAND_MAX;
        double tmp2 = (double) qrand() /RAND_MAX;
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
        QVector<QString> teilNamen, namen(49);
        x << min << max << max << min << min;
        y << min << min << max << max << min;
        teilWerte       << min-offset << min   << min+offset  << nom   << max-offset  << max   << max+offset;
        teilErgebnisse  << false      << true  << true        << true  << true        << true  << false;
        teilNamen       << "Min- "    << "Min "<< "Min+ "     << "Nom "<< "Max- "     << "Max "<< "Max+ ";

//        xTest        << nom     << nom        << nom  << nom              << max-offset << max  << max+offset       << nom        << nom  << nom              << min+offset << min  << min-offset;
//        yTest        << nom     << min+offset << min  << min-offset       << nom        << nom  << nom              << max-offset << max  << max+offset       << nom        << nom  << nom;
//        wantedResult << true    << true       << true << false            << true       << true << false            << true       << true << false            << true       << true << false;

        for(int a = 0; a < 7; ++a)
            for(int b = 0; b < 7; ++b)
            {
                xTest[7*a + b] = teilWerte[a];
                yTest[7*a + b] = teilWerte[b];
                sollErgebnisse[7*a + b] = teilErgebnisse[a] && teilErgebnisse[b];
                namen[7*a + b] = teilNamen[a] + teilNamen[b];
            }

//        int angle = 90;
//        rotateKoords(x,y,angle);
//        rotateKoords(xTest,yTest,angle);

        plots[t]->xAxis->setRange(min-2*offset,max+2*offset);
        plots[t]->yAxis->setRange(min-2*offset,max+2*offset);
        plots[t]->addGraph();
        plots[t]->graph(0)->setData(x,y);
        QPen myPen(Qt::red);
        myPen.setWidth(2*myPen.width());
        plots[t]->graph(0)->setPen(myPen);
        plots[t]->addGraph();
        plots[t]->graph(1)->setData(xTest,yTest);
        QCPScatterStyle myScatter;
        myScatter.setShape(QCPScatterStyle::ssCircle);
        QPen myPen2(Qt::green);
        myScatter.setPen(myPen2);
        myScatter.setBrush(Qt::green);
        myScatter.setSize(5);
        plots[t]->graph(1)->setScatterStyle(myScatter);
        plots[t]->graph(1)->setPen(Qt::NoPen);
        plots[t]->replot();

        QString logText = "Robustness Worse Case Test der InsidePoint-Funktion der Area-Klasse:\n"
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
                    logText += " Erfolgreich";
                    ++count;
                }
                else
                    logText += " Nicht erfolgreich";
                QString str = sollErgebnisse[i] ? ", TRUE" : ", FALSE";
                logText += " (" + namen[i] + str + ")\n";
            }
            logText += "Zusammenfassung: " + QString::number(count) + " von 49 Tests erfolgreich.";
        }
        else
        {
            logText += "Nicht erfolgreich\n\nAbbruch";
        }
        logs[t]->setText(logText);
        tabs->addTab(ptrs[t],"Testserie Nr. " + QString::number(t+1));
    }
    return tabs;
}

QWidget* model::TesterArea::robustnessWorstCaseTesterOnLine()
{
    tabs = new QTabWidget();
    int numberTests = 10;
    ptrs.resize(numberTests);
    grids.resize(numberTests);
    plots.resize(numberTests);
    logs.resize(numberTests);
    for(int t = 0; t < numberTests; ++t)
    {
        ptrs[t] = new QWidget();
        grids[t] = new QGridLayout(ptrs[t]);
        plots[t] = new QCustomPlot(ptrs[t],false);
        logs[t] = new QTextEdit(ptrs[t]);
        grids[t]->addWidget(plots[t],0,0);
        grids[t]->addWidget(logs[t],1,0);

        plots[t]->setMinimumSize(500,500);
        logs[t]->setMinimumSize(500,200);
        logs[t]->setReadOnly(true);


        QVector<double> x, y, xTest, yTest;
        QVector<bool> sollErgebnisse;
        QVector<QString> namen;
        x << (double) qrand() / RAND_MAX << (double) qrand() / RAND_MAX;
        y << (double) qrand() / RAND_MAX << (double) qrand() / RAND_MAX;

        double deltaX = x[1] - x[0];
        double deltaY = y[1] - y[0];

        xTest << x[0] - deltaX/8 << x[0] << x[0] + deltaX/8 << (x[0] + x[1])/2 << x[1] - deltaX/8 << x[1] << x[1] + deltaX/8;
        yTest << y[0] - deltaY/8 << y[0] << y[0] + deltaY/8 << (y[0] + y[1])/2 << y[1] - deltaY/8 << y[1] << y[1] + deltaY/8;
        sollErgebnisse << false  << true << true            << true            << true            << true << false;
        namen       << "Min- "    << "Min "<< "Min+ "     << "Nom "<< "Max- "     << "Max "<< "Max+ ";

//        int angle = 90;
//        rotateKoords(x,y,angle);
//        rotateKoords(xTest,yTest,angle);

        plots[t]->xAxis->setRange(x[0] - deltaX/4,x[1] + deltaX/4);
        plots[t]->yAxis->setRange(y[0] - deltaY/4,y[1] + deltaY/4);
        plots[t]->addGraph();
        plots[t]->graph(0)->setData(x,y);
        QPen myPen(Qt::red);
        myPen.setWidth(2*myPen.width());
        plots[t]->graph(0)->setPen(myPen);
        plots[t]->addGraph();
        plots[t]->graph(1)->setData(xTest,yTest);
        QCPScatterStyle myScatter;
        myScatter.setShape(QCPScatterStyle::ssCircle);
        QPen myPen2(Qt::green);
        myScatter.setPen(myPen2);
        myScatter.setBrush(Qt::green);
        myScatter.setSize(5);
        plots[t]->graph(1)->setScatterStyle(myScatter);
        plots[t]->graph(1)->setPen(Qt::NoPen);
        plots[t]->replot();

        QString logText = "Robustness Worse Case Test der OnLine-Funktion der Area-Klasse:\n"
                          "Generierte Werte: \n\tx[0] = " + QString::number(x[0]) + "\n\tx[1] = "
                          + QString::number(x[1]) + "\n\ty[0] = " + QString::number(y[0])
                          + "\n\ty[1] = " + QString::number(y[1]) + "\n\nBeginn der Tests:\n";

        int count = 0;
        for(int i = 0; i < 7; ++i)
        {
            logText += "Test Nummer " + QString("%1").arg(i+1,2,10,QChar('0')) + ": x = "
            + QString::number(xTest[i],'f',5) + ", y = " + QString::number(yTest[i],'f',5);
            if(model::Area::onLine(x[0],y[0],x[1],y[1],xTest[i],yTest[i]) == sollErgebnisse[i])
            {
                logText += " Erfolgreich";
                ++count;
            }
            else
                logText += " Nicht erfolgreich";
            QString str = sollErgebnisse[i] ? ", TRUE" : ", FALSE";
            logText += " (" + namen[i] + str + ")\n";
        }
        logText += "Zusammenfassung: " + QString::number(count) + " von 7 Tests erfolgreich.";

        logs[t]->setText(logText);
        tabs->addTab(ptrs[t],"Testserie Nr. " + QString::number(t+1));
    }
    return tabs;
}

QWidget * model::TesterArea::specialSegIntersectTester()
{
    tabs = new QTabWidget();
    int numberTests = 10;
    ptrs.resize(numberTests);
    grids.resize(numberTests);
    plots.resize(numberTests);
    logs.resize(numberTests);
    for(int t = 0; t < numberTests; ++t)
    {
        ptrs[t] = new QWidget();
        grids[t] = new QGridLayout(ptrs[t]);
        plots[t] = new QCustomPlot(ptrs[t],false);
        logs[t] = new QTextEdit(ptrs[t]);
        grids[t]->addWidget(plots[t],0,0);
        grids[t]->addWidget(logs[t],1,0);

        plots[t]->setMinimumSize(500,500);
        logs[t]->setMinimumSize(500,200);
        logs[t]->setReadOnly(true);

        double x1,x2,y1,y2,pct1,pct2;
        if(t==0)
        {
            x1 = 0.3; y1 = 0.3;
            x2 = 0.8; y2 = 0.8;

            pct1 = 0.2;
            pct2 = 0.6;
        }
        else
        {
            x1 = (double) qrand() / RAND_MAX;
            x2 = (double) qrand() / RAND_MAX;
            y1 = (double) qrand() / RAND_MAX;
            y2 = (double) qrand() / RAND_MAX;

            pct1 = (double) qrand() / RAND_MAX;
            pct2 = (double) qrand() / RAND_MAX;
        }

        double deltaX = x2-x1;
        double deltaY = y2-y1;

        plots[t]->xAxis->setRange(qMin(x1,x2) - qAbs(deltaX/4),qMax(x1,x2) + qAbs(deltaX/4));
        plots[t]->yAxis->setRange(qMin(y1,y2) - qAbs(deltaY/4),qMax(y1,y2) + qAbs(deltaY/4));

        QVector<double> xV1,yV1;
        xV1 << x1 << x1+deltaX*pct1;
        yV1 << y1 << y1+deltaY*pct1;
        plots[t]->addGraph();
        plots[t]->graph(0)->setData(xV1,yV1);
        QCPScatterStyle myScatter;
        myScatter.setShape(QCPScatterStyle::ssCircle);
        QPen myPen2(Qt::green);
        myScatter.setPen(myPen2);
        myScatter.setBrush(Qt::green);
        myScatter.setSize(5);
        plots[t]->graph(0)->setScatterStyle(myScatter);

        QVector<double> xV2,yV2;
        xV2 << x1+deltaX*pct2 << x2;
        yV2 << y1+deltaY*pct2 << y2;
        plots[t]->addGraph();
        plots[t]->graph(1)->setData(xV2,yV2);
        plots[t]->graph(1)->setScatterStyle(myScatter);
        plots[t]->replot();

        QString logText = "Spezieller Einzel-Test der SegIntersect-Funktion der Area-Klasse:\n"
                          "Generierte Werte: \n\tx[0] = " + QString::number(xV1[0]) + "\n\tx[1] = "
                          + QString::number(xV2[1]) + "\n\ty[0] = " + QString::number(yV1[0])
                          + "\n\ty[1] = " + QString::number(yV2[1]) + "\n\nBeginn des Tests:\n";
        bool test = Area::segIntersect(xV1[0],yV1[0],xV1[1],yV1[1],
                                       xV2[0],yV2[0],xV2[1],yV2[1]);
        if(test == (pct1 >= pct2))
            logText += "Test erfolgreich.";
        else
            logText += "Test nicht erfolgreich.";

        logs[t]->setText(logText);
        tabs->addTab(ptrs[t],"Testserie Nr. " + QString::number(t+1));
    }
    return tabs;
}
