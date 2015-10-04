#ifndef DOUBLESPINBOXDELEGATE_H
#define DOUBLESPINBOXDELEGATE_H

#include <QStyledItemDelegate>

namespace presentation {
    /*!
     * \brief Die Klasse DoubleSpinBoxDelegate implementiert analog zum Qt SpinBoxDelegete
     * Beispiel einen ItemDelegate, der eine QDoubleSpinBox als Editor benutzt. Dies
     * ermöglicht, in den Tabellen in den AreaWdiget - Objekten das Anpassen der Werte mit
     * QDoubleSpinBoxen durchzuführen.
     */
    class DoubleSpinBoxDelegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        /*!
         * \brief Konstruktor für die KLasse DoubleSpinBoxDelegate.
         * \param parent parent-Widget
         * \param minValue minimaler Wert für die DoubleSpinBox
         * \param maxValue maximaler Wert für die DoubleSpinBox
         */
        DoubleSpinBoxDelegate(QObject * parent, double const minValue,
                              double const maxValue);

        /*!
         * \brief createEditor erstellt den Editor der für das Ändern der Tabellenwerte
         * genutzt wird.
         * \param parent-Widget für den neuen editorEvent
         * \param option nicht benutzter Parameter
         * \param index nicht benutzter Parameter
         *
         * Hier eine QDoubleSpinBox mit den im Konstruktor übergeben minimalen und
         * maximalen Werten und ohne Nachkommastellen.
         */
        QWidget * createEditor(QWidget * parent, QStyleOptionViewItem const & option,
                               QModelIndex const & index) const Q_DECL_OVERRIDE;
        /*!
         * \brief setEditorData überträgt Daten aus dem Modell im Modell-Index in den
         * Editor.
         * \param editor Der Editor, d.h. die DoubleSpinBox
         * \param index Hier: enthält die Tabelle, die den Editor benutzt
         */
        void setEditorData(QWidget * editor,
                           QModelIndex const & index) const Q_DECL_OVERRIDE;
        /*!
         * \brief setModelData überträgt Daten aus dem Editor in das Modell.
         * \param editor Der Editor, d.h. die DoubleSpinBox
         * \param model Hier: die Tabelle, die den Editor benutzt
         * \param index Hier: der Index des Tabellenfeldes
         */
        void setModelData(QWidget * editor, QAbstractItemModel * model,
                          QModelIndex const & index) const Q_DECL_OVERRIDE;
        /*!
         * \brief updateEditorGeometry aktualisiert die Geometry des Editors.
         * \param editor Der Editor, d.h. die DoubleSpinBox
         * \param option Hier: enthält die neue Geometry
         * \param index nicht benutzter Parameter
         */
        void updateEditorGeometry(QWidget * editor, QStyleOptionViewItem const & option,
                                  QModelIndex const & index) const Q_DECL_OVERRIDE;

    private:
        DoubleSpinBoxDelegate();

        double const minValue;
        double const maxValue;
    };

}

#endif // DOUBLESPINBOXDELEGATE_H
