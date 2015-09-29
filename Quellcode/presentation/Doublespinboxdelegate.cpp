#include "Doublespinboxdelegate.h"

#include <QDoubleSpinBox>

presentation::DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject * parent,
                                                           double const minValue,
                                                           double const maxValue)
    : QStyledItemDelegate(parent), minValue(minValue), maxValue(maxValue)
{

}

QWidget* presentation::DoubleSpinBoxDelegate::createEditor(QWidget * parent,
                                                           QStyleOptionViewItem const &/* option*/,
                                                           QModelIndex const & /*index*/) const
{
    QDoubleSpinBox * editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(minValue);
    editor->setMaximum(maxValue);
    editor->setDecimals(0);

    return editor;
}

void presentation::DoubleSpinBoxDelegate::setEditorData(QWidget * editor,
                                                        QModelIndex const & index) const
{
    double value = index.model()->data(index,Qt::EditRole).toDouble();

    QDoubleSpinBox * spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void presentation::DoubleSpinBoxDelegate::setModelData(QWidget * editor,
                                                       QAbstractItemModel * model,
                                                       QModelIndex const & index) const
{
    QDoubleSpinBox * spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index,value,Qt::EditRole);
}

void presentation::DoubleSpinBoxDelegate::updateEditorGeometry(QWidget * editor,
                                                               QStyleOptionViewItem const & option,
                                                               QModelIndex const & /*index*/) const
{
    editor->setGeometry(option.rect);
}
