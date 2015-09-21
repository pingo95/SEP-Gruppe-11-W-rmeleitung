#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>

#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>

namespace presentation {

    class HelpWidget : public QWidget
    {
        Q_OBJECT

    //Funktionen:
    public:
        explicit HelpWidget(QWidget *parent = 0);

    //Attribute:
    private:
        QGridLayout * layout;

        QLabel * topLabel;

        QLabel * configurationHelp;
        QLabel * thermalDiffusivitiesHelp;
        QLabel * heatSourcesHelp;
        QLabel * iBVsHelp;

        QLabel * simulationHelp;

        QLabel * visualizationHelp;

        QLabel * optimizationHelp;

        QSpacerItem * spacerItem;
    };

}

#endif // HELPWIDGET_H