/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include "class_specification_item.h"

#include "class_specification_widget.h"
#include "module_generator_logger.h"
#include "module_generator_utils.h"
#include "module_generator.h"

#include <QPushButton>
#include <QHBoxLayout>

const QString
ClassSpecificationItem::S_DERIVED_CLASS_BTN_TEXT =
        QStringLiteral("<Edit class definition>");
const QString
ClassSpecificationItem::S_LINKED_CLASS_BTN_TEXT =
        QStringLiteral("<Edit linked class definition>");
const QString
S_CLASS_ALT_BTN_TEXT =
        QStringLiteral("<Set existing class>");
//        QStringLiteral("<Class definition to extend>");
const QString
ClassSpecificationItem::S_CLASS_BTN_TOOLTIP =
        QStringLiteral("Edit class definition");

ClassSpecificationItem::ClassSpecificationItem(FunctionData function,
                                               TypeInfo typeInfo,
                                               ModuleGeneratorSettings* settings,
                                               QWidget* parent) :
    QWidget(parent),
    AbstractTypeSpecificationItem(function.implementation, std::move(typeInfo)),
    m_functionData(std::move(function)),
    m_settings(settings)
{
    auto* baseLayout  = new QHBoxLayout;

    m_derivedClassButton = new QPushButton{S_DERIVED_CLASS_BTN_TEXT};
    m_linkedClassButton  = new QPushButton{S_LINKED_CLASS_BTN_TEXT};
    m_derivedClassButton->setToolTip(S_CLASS_BTN_TOOLTIP);
    m_linkedClassButton->setToolTip(S_CLASS_BTN_TOOLTIP);

    baseLayout->setContentsMargins(0, 0, 0, 0);

    baseLayout->addWidget(m_linkedClassButton);
    baseLayout->setStretchFactor(m_linkedClassButton, 1);
    baseLayout->addWidget(m_derivedClassButton);
    baseLayout->setStretchFactor(m_derivedClassButton, 1);

    setLayout(baseLayout);

    if (!m_functionData.linkedClass.isValid() &&
        !(this->typeInfo().widgetType == S_WIDGET_TYPE_CLASSLINKED))
    {
        m_linkedClassButton->setVisible(false);
    }

    m_derivedClassWidget = std::make_unique<ClassSpecificationWidget>(
                m_functionData.baseClass, m_settings);

    m_linkedClassWidget  = std::make_unique<ClassSpecificationWidget>(
                m_functionData.linkedClass, m_settings);

    setupSpecificationsWidget(*m_derivedClassWidget);
    setupSpecificationsWidget(*m_linkedClassWidget);

    connect(m_derivedClassButton, SIGNAL(pressed()),
            this, SLOT(onDerivedClassBtnPressed()));
    connect(m_derivedClassWidget.get(), SIGNAL(hidden()),
            this, SLOT(onHideDerivedClassWidget()));

    connect(m_linkedClassButton, SIGNAL(pressed()),
            this, SLOT(onLinkedClassBtnPressed()));
    connect(m_linkedClassWidget.get(), SIGNAL(hidden()),
            this, SLOT(onHideLinkedClassWidget()));

    // if object name is already set
    onHideDerivedClassWidget();
    onHideLinkedClassWidget();
}

void
ClassSpecificationItem::setupSpecificationsWidget(
        ClassSpecificationWidget& widget)
{
    widget.setWindowFlag(Qt::Dialog);
    widget.setWindowModality(Qt::ApplicationModal);
//    widget.setMinimumWidth(330);
}

void
ClassSpecificationItem::updateSpecificationButton(QPushButton& button,
                                                  ClassData const& implClass,
                                                  bool isLinkedBtn)
{
    QString text{};
    QPalette palette{};

    if (implClass.isValid())
    {
        text = implClass.className;

        if (!implClass.objectName.isEmpty())
        {
            text += " (" + implClass.objectName + ")";
        }
    }
    else
    {
        palette.setColor(QPalette::ButtonText, Qt::blue);
        if (isLinkedBtn)
        {
            text = m_functionData.linkedClass.isValid() ?
                        S_LINKED_CLASS_BTN_TEXT :
                        S_CLASS_ALT_BTN_TEXT;
        }
        else
        {
            text = m_functionData.baseClass.isValid() ?
                        S_DERIVED_CLASS_BTN_TEXT :
                        S_CLASS_ALT_BTN_TEXT;
        }
    }

    button.setText(text);
    button.setPalette(palette);
}

QStringList
ClassSpecificationItem::codeImplementation()
{
    ClassData implementation = m_derivedClassWidget->implementedClass();

    if (!implementation.isValid())
    {
        LOG_INDENT_WARN("invalid implementation!");
        return {};
    }

    QString returnType = m_functionData.returnType;

    if (!typeInfo().isValid())
    {
        LOG_INDENT_ERR("No type info found for '" + returnType + "'!");
        return {};
    }
    if (typeInfo().implementation.isEmpty())
    {
        LOG_INDENT_ERR("Invalid type info for '" + returnType + "'!");
        return {};
    }

    // we use identifiers to set certain data (e.g. class name to return etc)
    IdentifierPairs idents;

    idents.append({ModuleGenerator::S_ID_CLASS_NAME,
                   implementation.className});
    // mostly necessary for task and calculator data
    idents.append({ModuleGenerator::S_ID_OBJECT_NAME,
                   implementation.objectName});
    // create a variable name using the object name
    QString varName = implementation.objectName;
    varName.remove(' ');
    if (!varName.isEmpty())
    {
        auto c = varName.at(0).toLower();
        varName.replace(0, 1, c);
        idents.append({S_ID_VARIABLE_NAME, varName});
    }

    // linked class
    auto linkedClass = m_linkedClassWidget->implementedClass();
    if (m_functionData.linkedClass.isValid() && linkedClass.isValid())
    {
        idents.append({S_ID_LINKED_VALUE, linkedClass.className});
    }

    // replace all identifiers in the implementation
    QStringList lines = typeInfo().implementation;
    for (auto& line : lines)
    {
        utils::replaceIdentifier(line, idents);
    }

    return lines;
}


QStringList
ClassSpecificationItem::additionalIncludes()
{
    // includes from type info
    QStringList includes{typeInfo().includes};

    // generate include for extended classes
    auto derivedClasses = m_derivedClassWidget->implementedClass();
    if (!m_functionData.baseClass.isValid() && derivedClasses.isValid())
    {
        includes << derivedClasses.fileName;
    }

    // generate include for linked class
    auto linkedClass = m_linkedClassWidget->implementedClass();
    if (!m_functionData.linkedClass.isValid() && linkedClass.isValid())
    {
        includes << linkedClass.fileName;
    }

    return includes;
}


ClassDataList
ClassSpecificationItem::derivedClasses()
{
    return { m_derivedClassWidget->implementedClass() };
}

ClassDataList
ClassSpecificationItem::linkedClasses()
{
    return { m_linkedClassWidget->implementedClass() };
}

void
ClassSpecificationItem::onDerivedClassBtnPressed()
{
    m_derivedClassWidget->show();
}

void
ClassSpecificationItem::onHideDerivedClassWidget()
{
    updateSpecificationButton(*m_derivedClassButton,
                              m_derivedClassWidget->implementedClass(), false);
}

void
ClassSpecificationItem::onLinkedClassBtnPressed()
{
    m_linkedClassWidget->show();
}

void
ClassSpecificationItem::onHideLinkedClassWidget()
{
    updateSpecificationButton(*m_linkedClassButton,
                              m_linkedClassWidget->implementedClass(), true);
}
