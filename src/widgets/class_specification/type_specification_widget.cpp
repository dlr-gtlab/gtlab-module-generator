#include "type_specification_widget.h"


BoolSpecificationWidget::BoolSpecificationWidget(
        ImplementationData implementation,
        QWidget *parent) :
    QCheckBox(parent),
    AbstractTypeSpecifier{std::move(implementation)}
{
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged()));
    onStateChanged();
}

QStringList
BoolSpecificationWidget::implementationCode()
{
    return {stateToString()};
}

QString
BoolSpecificationWidget::stateToString()
{
    return isChecked() ? QStringLiteral("true") : QStringLiteral("false");
}

void
BoolSpecificationWidget::onStateChanged()
{
    setText(this->stateToString());
}


StringSpecificationWidget::StringSpecificationWidget(
        ImplementationData implementation,
        QWidget* parent) :
    QLineEdit{parent},
    AbstractTypeSpecifier{std::move(implementation)}
{

}

QStringList
StringSpecificationWidget::implementationCode()
{
    return {"QStringLiteral(\"" + text() + "\")"};
}

ComboboxSpecificationWidget::ComboboxSpecificationWidget(
        ImplementationData implementation,
        QStringList const& values,
        QString const& prefix,
        QString const& suffix,
        QWidget* parent) :
    QComboBox{parent},
    AbstractTypeSpecifier{std::move(implementation)},
    m_prefix{prefix},
    m_suffix{suffix}
{
    addItems(values);
}

QStringList
ComboboxSpecificationWidget::implementationCode()
{
    QString retVal = currentText();

    retVal.prepend(m_prefix);
    retVal.append(m_suffix);

    return {retVal};
}
