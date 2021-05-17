#include "simple_class_specification_widget.h"


BoolClassSpecificationWidget::BoolClassSpecificationWidget(ImplementationStruct& implementation,
                                                           QWidget* parent):
    QCheckBox(parent), AbstractClassSpecification(implementation)
{

}

StringClassSpecificationWidget::StringClassSpecificationWidget(ImplementationStruct& implementation,
                                                               QWidget *parent) :
    QLineEdit(parent), AbstractClassSpecification(implementation)
{

}

ComboClassSpecificationWidget::ComboClassSpecificationWidget(ImplementationStruct& implementation,
                                                             const QStringList& values,
                                                             const QString& prefix,
                                                             const QString& suffix,
                                                             QWidget* parent) :
    QComboBox(parent), AbstractClassSpecification(implementation),
    m_prefix(prefix), m_suffix(suffix)
{
    addItems(values);
}
