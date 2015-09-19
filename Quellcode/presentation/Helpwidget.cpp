#include "Helpwidget.h"

presentation::HelpWidget::HelpWidget(QWidget *parent) : QWidget(parent)
{
        //Labels
    topLabel = new QLabel("Dies ist der Hilfe-Tab. Hier finden Sie "
                                  "ausführliche Beschreibungen und Erklärungen"
                                  " zu den Funktionen der anderen Tabs.",this);
    topLabel->setWordWrap(true);

    configurationHelp = new QLabel("Info zum Tab Konfiguration",this);
    configurationHelp->setWordWrap(true);
    thermalDiffusivitiesHelp = new QLabel("Info zum Tab Wärmeleitkoeff",this);
    thermalDiffusivitiesHelp->setWordWrap(true);
    heatSourcesHelp = new QLabel("Info zum Tab Wärmequellen",this);
    heatSourcesHelp->setWordWrap(true);
    iBVsHelp = new QLabel("Info zum Tab IBV",this);
    iBVsHelp->setWordWrap(true);

    simulationHelp = new QLabel("Info zum Tab Simualtion",this);
    simulationHelp->setWordWrap(true);

    visualizationHelp = new QLabel("Info zum Tab Visualisierung",this);
    visualizationHelp->setWordWrap(true);

    optimizationHelp = new QLabel("Info zum Tab Optimierung",this);
    optimizationHelp->setWordWrap(true);

        //Platzhalter
    spacerItem = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);

        //Layout
    layout = new QGridLayout(this);

    layout->addWidget(topLabel,0,0);
    layout->addWidget(configurationHelp,1,0);
    layout->addWidget(thermalDiffusivitiesHelp,2,0);
    layout->addWidget(heatSourcesHelp,3,0);
    layout->addWidget(iBVsHelp,4,0);
    layout->addWidget(simulationHelp,5,0);
    layout->addWidget(visualizationHelp,6,0);
    layout->addWidget(optimizationHelp,7,0);
    layout->addItem(spacerItem,8,0);
}

