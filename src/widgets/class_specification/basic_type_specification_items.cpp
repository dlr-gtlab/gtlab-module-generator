/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include "basic_type_specification_items.h"

#include "module_generator_utils.h"
#include "module_generator.h"

BoolSpecificationItem::BoolSpecificationItem(
        ImplementationData implementation,
        TypeInfo typeInfo,
        QWidget* parent) :
    QCheckBox(parent),
    AbstractTypeSpecificationItem{std::move(implementation),
                                  std::move(typeInfo)}
{
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged()));
    onStateChanged();
}

QStringList
BoolSpecificationItem::codeImplementation()
{
    return {stateToString()};
}

QString
BoolSpecificationItem::stateToString()
{
    return isChecked() ? QStringLiteral("true") : QStringLiteral("false");
}

void
BoolSpecificationItem::onStateChanged()
{
    setText(this->stateToString());
}


StringSpecificationItem::StringSpecificationItem(
        ImplementationData implementation,
        TypeInfo typeInfo,
        QWidget* parent) :
    QLineEdit{parent},
    AbstractTypeSpecificationItem{std::move(implementation),
                                  std::move(typeInfo)}
{

}

QStringList
StringSpecificationItem::codeImplementation()
{
    return {"QStringLiteral(\"" + text() + "\")"};
}

ComboboxSpecificationItem::ComboboxSpecificationItem(
        ImplementationData implementation,
        TypeInfo typeInfo,
        QWidget* parent) :
    QComboBox{parent},
    AbstractTypeSpecificationItem{std::move(implementation),
                                  std::move(typeInfo)}
{
    addItems(this->typeInfo().values);
}

QStringList
ComboboxSpecificationItem::codeImplementation()
{
    IdentifierPair ident{S_ID_VALUE, currentText()};

    auto implementation = typeInfo().implementation;
    for (auto& line : implementation)
    {
        utils::replaceIdentifier(line, ident);
    }

    return implementation;
}
