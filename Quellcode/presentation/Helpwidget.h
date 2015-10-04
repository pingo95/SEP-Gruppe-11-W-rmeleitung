#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>

#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>

namespace presentation {
    /*!
     * \brief Die Klasse HelpWidget implementiert den Hilfe-Tab. Dieser bietet lediglich
     * Text und keinerlei Funktionaliät und wurde nur zwecks Leserlichkeit und parallelen
     * Arbeitens ausgelagert.
     */
    class HelpWidget : public QWidget
    {
        Q_OBJECT

    //Funktionen:
    public:
        /*!
         * \brief Konstruktor für die Klasse HelpWidget.
         * \param parent parent-Widget
         */
        explicit HelpWidget(QWidget *parent = 0);

    //Attribute:
    private:
        QGridLayout * layout;

        QLabel * topLabel;

        QLabel * configurationHelp;
        QLabel * thermalDiffusivitiesHelp;
        QLabel * iBVsHelp;

        QLabel * simulationHelp;

        QLabel * visualizationHelp;

        QLabel * optimizationKonfigurationHelp;
        QLabel * optimizationResultsHelp;

        QSpacerItem * spacerItem;
    };

}

#endif // HELPWIDGET_H
