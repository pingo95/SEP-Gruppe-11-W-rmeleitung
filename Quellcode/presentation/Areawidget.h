#ifndef AREAWIDGET_H
#define AREAWIDGET_H

#include <QWidget>

#include "../model/Model.h"
#include <QMap>

#include <QGridLayout>
#include <QLabel>
#include <QTableWidget>
#include <QStringList>
#include "Doublespinboxdelegate.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include "Qcustomplot.h"
#include <QRadioButton>
#include <QDoubleSpinBox>

/*!
 * \brief Enthält den Controller und den View des MVC und alle dazugehörigen Klassen.
 */
namespace presentation {
class Controller;
    /*!
     * \brief Die Klasse AreaWidget implementiert den Tab zur Eingabe von Gebieten.
     *
     * Der Tab enthält insbesondere eine Tabelle zum Anzeigen und Bearbeiten der Werte
     * der Gebiete und einen QCustomPlot mit QCPColorScale zum Anzeigen, Erstellen und
     * Bearbeiten von Gebieten. Außerdem hat der Tab noch weitere Knöpfe und Eingabefelder,
     * sodass insgesamt folgende Funktionalitäten zur Verfügung stehen:
     *          - Erstellen eines neuen Gebietes durch
     *                   - Hinzufügen einzelner Punkte per linkem Mausklick oder per
     *                     Tastatur und zugehörigem Knopf
     *                   - Eingabe des Wertes des Gebietes in einem Pop-Up-Fenster
     *          - Abbrechen des Erstellen eines neuen Gebietes
     *          - Löschen und Wiederherstellen von Punkten während des Erstellen eines
     *            neuen Gebietes
     *          - Löschen aller erstellten Gebiete
     *          - Markieren eines Gebietes per rechtem Mausklick
     *          - Löschen eines markierten Gebietes
     *          - Verändern der Position eines markierten Gebietes
     *          - Ändern des Wertes eines Gebietes über die Tabelle
     */
    class AreaWidget : public QWidget
    {
        Q_OBJECT

    //Enum:
    public:
        /*!
         * \brief Der Enum AreaTableColumn dient zur Orientierung, in welcher Spalte der
         * Tabelle die IDs und in welcher Spalte die Werte der Gebiete stehen
         */
        enum AreaTableColumn
        {
            /*! Spaltennummer der Gebiet-IDs*/
            ColumnID = 0,
            /*! Spaltennummer der Gebiet-Werte*/
            ColumnValue = 1,
        };

    //Funktionen:
    public:
        /*!
         * \brief Konstruktor der Klasse AreaWidget.
         * \param parent parent-Widget
         * \param type Typ der Gebiete des Tabs
         * \param name Name des Gebietstyps
         * \param unit Einheit des Gebietstyps
         * \param valueShift Werte-Shift zur Visualisierung der Gebietswerte
         */
        explicit AreaWidget(QWidget *parent, model::SimulationSetup::AreaType type,
                            QString const name, QString const unit,
                            double const valueShift = 1);
//        ~AreaWidget();

        /*!
         * \brief drawPartialArea zeichnet ein neues, angefangenes, unfertiges Gebiet.
         * \param partialAreaX x-Koordinaten der Punkte des Gebietes
         * \param partialAreaY y-Koordinaten der Punkte des Gebietes
         * \see Controller::areaClickSlot
         */
        void drawPartialArea(QVector<double> const & partialAreaX,
                            QVector<double> const & partialAreaY);

        /*!
         * \brief setController setzt die Referenz auf den Controller und verbindet
         * Signale und Slots.
         * \param controller der Controller
         * \see Controller
         */
        void setController(Controller * controller);
        /*!
         * \brief setModel setzt die Referenz auf das Modell.
         * \param model das Modell
         * \see model::Model
         */
        void setModel(model::Model * model);

        /*!
         * \brief update aktualisiert den Tab mit den aktuellen Werten aus dem Modell.
         *
         * U.a. werden die aktuell im Modell enthaltenen Gebiete gezeichnet und deren Werte
         * in der Tabelle angezeigt.
         * \see UI::updateNotification
         */
        void update();

    signals:
        /*!
         * \brief areaClicked wird ausgesendet, wenn auf die Gebiet-Platte mit der linken
         * Maustaste geklickt wurde oder der "Punkt hinzufügen"-Knopf gedrückt wurde.
         * \param xKoord x-Koordinate des Klicks
         * \param yKoord y-Koordinate des Klicks
         * \param plateSize aktuelle Größe der Gebiet-Platte
         * \param valueShift Werte-Shift zur Visualisierung der Gebietswerte
         * \param mouseClick true/false für Mausklick oder per Tastatur
         * \param type Gebietstyp zum Entscheiden für den Controller
         * \see Controller::areaClickSlot
         */
        void areaClicked(double xKoord, double yKoord, QSize plateSize,
                         double valueShift, bool mouseClick,
                         model::SimulationSetup::AreaType type);
        /*!
         * \brief areaValueChanged wird ausgesendet, wenn der Wert eines Gebietes geändert
         * werden soll.
         * \param pos Position des Gebietes, dessen Wert geändert werden soll
         * \param newValue neuer Wert
         * \param type Gebietstyp zum Entscheiden für den Controller
         * \see Controller::areaValueChangedSlot
         */
        void areaValueChanged(int pos, double newValue,
                              model::SimulationSetup::AreaType type);
        /*!
         * \brief deleteArea wird ausgesendet, wenn ein Gebiet gelöscht werden soll
         * \param pos Position des Gebietes, das gelöscht werden soll
         * \param type Gebietstyp zum Entscheiden für den Controller
         * \see Controller::deleteAreaSlot
         */
        void deleteArea(int pos, model::SimulationSetup::AreaType type);
        /*!
         * \brief clearAreas wird ausgesendet, wenn alle Gebiete gelöscht werden sollen
         * \param type Gebietstyp zum Entscheiden für den Controller
         * \see Controller::clearAreasSlot
         */
        void clearAreas(model::SimulationSetup::AreaType type);
        /*!
         * \brief discardNewArea wird ausgesendet, wenn ein neues, angefangenes Gebiet
         * abgebrochen werden soll
         * \param type Gebietstyp zum Entscheiden für den Controller
         * \see Controller::discardAreaSlot
         */
        void discardNewArea(model::SimulationSetup::AreaType type);
        /*!
         * \brief redo wird ausgesendet, wenn ein gelöschter Punkt von einem neuen Gebiet
         * wiederhergestellt werden soll
         * \param type Gebietstyp zum Entscheiden für den Controller
         * \see Controller::redoSlot
         */
        void redo(model::SimulationSetup::AreaType type);
        /*!
         * \brief reorderArea wird aus gesendet, wenn die Reihenfolge der Gebiete geändert
         * werden soll
         * \param pos Position des Gebietes, das verschoben werden soll
         * \param dir Richtung, in die das Gebiet verschoben werden soll
         * \param type Gebietstyp zum Entscheiden für den Controller
         * \see Controller::reorderAreaSlot
         */
        void reorderArea(int pos, int dir, model::SimulationSetup::AreaType type);
        /*!
         * \brief undo wird ausgesendet, wenn der letzte Punkt eines neuen, angefangenen
         * Gebietes gelöscht werden soll
         * \param type Gebietstyp zum Entscheiden für den Controller
         * \see Controller::undoSlot
         */
        void undo(model::SimulationSetup::AreaType type);

    public slots:

    private slots:
        void buttonMapperSlot();
        void mouseClickOnPlateSlot(QMouseEvent * event);
        void tableItemChangeSlot(QTableWidgetItem * item);
        void tableSelectionChangeSlot();

    private:
        int findRow(int id);
        void highlightGraph();
        void unhighlightGraph();

    //Attribute:
    private:
        Controller * controller;
        model::Model * model;
        QString const name;
        bool partialArea;
        int selectedAreaID;
        model::SimulationSetup::AreaType type;
        QString const unit;
        bool updating;
        double const valueShift;

        //Qt Widgets:
        QGridLayout * layout;
        QGridLayout * subLayout;

        QLabel * topLabel;

        QTableWidget * table;
        QStringList tableHeader;
        DoubleSpinBoxDelegate * delegate;

        QGroupBox * boxReorderButtons;
        QVBoxLayout * boxReorderButtonsLayout;
        QPushButton * allUpButton;
        QPushButton * upButton;
        QPushButton * downButton;
        QPushButton * allDownButton;

        QCustomPlot * plate;
        QCPColorScale * colorScale;

        QLabel * labelKeyboardInput;
        QLabel * labelXValue;
        QDoubleSpinBox * inputXValue;
        QLabel * labelYValue;
        QDoubleSpinBox * inputYValue;
        QPushButton * confirmButton;

        QGroupBox * boxUndoRedo;
        QVBoxLayout * boxUndoRedoLayout;
        QPushButton * undoButton;
        QPushButton * redoButton;


        QPushButton * discardAreaButton;
        QPushButton * deleteAreaButton;
        QPushButton * clearAreasButton;

        QSpacerItem * spacerItem1;
        QSpacerItem * spacerItem2;
    };

}

#endif // AREAWIDGET_H
