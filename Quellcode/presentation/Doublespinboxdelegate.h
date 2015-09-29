#ifndef DOUBLESPINBOXDELEGATE_H
#define DOUBLESPINBOXDELEGATE_H

#include <QStyledItemDelegate>

namespace presentation {

    class DoubleSpinBoxDelegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        DoubleSpinBoxDelegate(QObject * parent, double const minValue,
                              double const maxValue);

        QWidget * createEditor(QWidget * parent, QStyleOptionViewItem const & option,
                               QModelIndex const & index) const Q_DECL_OVERRIDE;
        void setEditorData(QWidget * editor,
                           QModelIndex const & index) const Q_DECL_OVERRIDE;
        void setModelData(QWidget * editor, QAbstractItemModel * model,
                          QModelIndex const & index) const Q_DECL_OVERRIDE;
        void updateEditorGeometry(QWidget * editor, QStyleOptionViewItem const & option,
                                  QModelIndex const & index) const Q_DECL_OVERRIDE;

    private:
        DoubleSpinBoxDelegate();

        double const minValue;
        double const maxValue;
    };

}

#endif // DOUBLESPINBOXDELEGATE_H
