#ifndef AREAWIDGET_H
#define AREAWIDGET_H

#include <QWidget>

#include "../model/Model.h"
#include <QMap>

#include <QGridLayout>
#include <QLabel>
#include <QTableWidget>
#include <QStringList>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include "Qcustomplot.h"
#include <QRadioButton>
#include <QDoubleSpinBox>

namespace presentation {
class Controller;

    class AreaWidget : public QWidget
    {
        Q_OBJECT

    //Enum:
    public:
        enum AreaTableColumn
        {
            ColumnID = 0,
            ColumnValue = 1,
        };

    //Funktionen:
    public:
        explicit AreaWidget(QWidget *parent, model::SimulationSetup::AreaType type,
                            QString const name, QString const unit, double const valueShift = 1);
//        ~AreaWidget();

        void drawPartialArea(QVector<double> const & partialAreaX,
                            QVector<double> const & partialAreaY);

        void setController(Controller * controller);
        void setModel(model::Model * model);

        void update();

    signals:
        void areaClicked(double xKoord, double yKoord, QSize plateSize,
                         double valueShift, bool mouseClick,
                         model::SimulationSetup::AreaType type);
        void areaValueChanged(int pos, double newValue, bool ok,
                              model::SimulationSetup::AreaType type);
        void deleteArea(int pos, model::SimulationSetup::AreaType type);
        void clearAreas(model::SimulationSetup::AreaType type);
        void discardNewArea(model::SimulationSetup::AreaType type);
        void redo(model::SimulationSetup::AreaType type);
        void reorderArea(int pos, int dir, model::SimulationSetup::AreaType type);
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
