/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include "class_specification_list.h"

#include "class_specification_item.h"
#include "module_generator_logger.h"
#include "module_generator_settings.h"
#include "module_generator_utils.h"
#include "module_generator.h"

#include "widget_list_view.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QRegularExpression>

const int
ClassSpecificationList::I_LIST_HEIGTH = 100;

const QString
ClassSpecificationList::S_ADD_BTN =
        QStringLiteral("Add ...");

const QString
DeletableSpecificationItem::S_DELETE_BTN_ICON_PATH =
        QStringLiteral(":/images/crossIcon_16.png");

ClassSpecificationList::ClassSpecificationList(const FunctionData& function,
                                               TypeInfo typeInfo,
                                               ModuleGeneratorSettings* settings,
                                               QWidget *parent) :
    QWidget(parent),
    AbstractTypeSpecificationItem(function.implementation, std::move(typeInfo)),
    m_functionData(function),
    m_settings(settings)
{
    auto* baseLayout = new QVBoxLayout;

    m_addButton = new QPushButton{S_ADD_BTN};
    m_widgetListView = new WidgetListView;
    m_widgetListView->setMinimumHeight(I_LIST_HEIGTH);

    baseLayout->addWidget(m_widgetListView);
    baseLayout->addWidget(m_addButton);

    baseLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(baseLayout);

    connect(m_addButton, SIGNAL(pressed()), this, SLOT(onAddButtonPressed()));
}

QStringList
ClassSpecificationList::codeImplementation()
{
    QStringList lines;

    QString const& returnType = m_functionData.returnType;

    // validity check
    if (!typeInfo().isValid())
    {
        LOG_INDENT_ERR("No type info found for '" + returnType + "'!");
        return {};
    }
    // implementation.size() == 3:
    // 1. Variable declaration (e.g. QList<...> list)
    // 2. Return value (e.g. list)
    if (typeInfo().implementation.size() != 2 || typeInfo().values.size() != 1)
    {
        LOG_INDENT_ERR("Invalid type info for '" + returnType + "'!");
        return {};
    }

    // typeInfo.implementation is intended as the variable Name
    QString retValDecl = typeInfo().implementation.first();
    utils::replaceIdentifier(retValDecl, {S_ID_RETURN_TYPE, returnType});

    lines << retValDecl;
    lines << QString{};

    for (auto* w : m_widgetListView->widgets())
    {
        auto* item = qobject_cast<DeletableSpecificationItem*>(w);
        if (!item) continue;
        auto* spec = dynamic_cast<AbstractTypeSpecificationItem*>(item->widget());
        if (!spec) continue;

        QStringList implementation = spec->codeImplementation();
        if (implementation.isEmpty()) continue;

        // implementation contains multiple lines
        // -> only last line is the one we will add to our list
        // -> mostly relevant for tasks and calculators
        if (implementation.length() > 1)
        {
            // for nicer indent
            if (lines.length() > 2) lines << QString{};

            lines << implementation.mid(0, implementation.length() - 1);
            lines << QString{};
        }

        IdentifierPairs idents;
        // value is last entry of implementation
        idents.append({S_ID_VALUE, implementation.last()});

        // only necessary for linked classes
//        if (typeInfo().widgetType == S_WIDGET_TYPE_MULTILINKED)
//        {
        ClassDataList linkedClasses = spec->linkedClasses();

        if (!linkedClasses.isEmpty() && linkedClasses.first().isValid())
        {
            idents.append({S_ID_LINKED_VALUE,
                           linkedClasses.first().className});
        }
//        }

        // will add a new entry to our list (i.e. list << "entry")
        QString value = typeInfo().values.first();
        utils::replaceIdentifier(value, idents);

        lines << value;
    }

    // for nicer indent
    if (lines.length() > 2) lines << QString{};

    // last line is return variable
    lines << typeInfo().implementation.last();

    return lines;
}

QStringList
ClassSpecificationList::additionalIncludes()
{
    QStringList includes;

    for (auto* w : m_widgetListView->widgets())
    {
        auto* item = qobject_cast<DeletableSpecificationItem*>(w);
        if (!item) continue;
        auto* spec = dynamic_cast<AbstractTypeSpecificationItem*>(item->widget());
        if (!spec) continue;

        includes << spec->additionalIncludes(); // may have multiple entries
    }

    return includes;
}

ClassDataList
ClassSpecificationList::derivedClasses()
{
    ClassDataList classes;

    for (auto* w : m_widgetListView->widgets())
    {
        auto* item = qobject_cast<DeletableSpecificationItem*>(w);
        if (!item) continue;
        auto* spec = dynamic_cast<AbstractTypeSpecificationItem*>(item->widget());
        if (!spec) continue;

        classes << spec->derivedClasses(); // may have one entry
    }

    return classes;
}

ClassDataList
ClassSpecificationList::linkedClasses()
{
    ClassDataList classes;

    for (auto* w : m_widgetListView->widgets())
    {
        auto* item = qobject_cast<DeletableSpecificationItem*>(w);
        if (!item) continue;
        auto* spec = dynamic_cast<AbstractTypeSpecificationItem*>(item->widget());
        if (!spec) continue;

        classes << spec->linkedClasses(); // may have one entry
    }

    return classes;
}

void
ClassSpecificationList::onAddButtonPressed()
{
    LOG_INDENT("Adding new class for '" + m_functionData.name + "'...");

    auto typeInfo = this->typeInfo().innerType(m_functionData.inputType);

    auto* widget = generateFunctionSpecification(m_settings,
                                                 m_functionData,
                                                 typeInfo);
    auto* inputWidget = dynamic_cast<QWidget*>(widget);

    if (inputWidget)
    {
        auto* wrapperWidget = new DeletableSpecificationItem(inputWidget);
        m_widgetListView->appendWidget(wrapperWidget);
    }

    LOG_INFO << "done!";
}

DeletableSpecificationItem::DeletableSpecificationItem(QWidget* inputWidget,
                                                       QWidget*parent) :
    QWidget(parent),
    m_inputWidget(inputWidget)
{
    auto* deleteButton = new QPushButton;
    QIcon deleteIcon{S_DELETE_BTN_ICON_PATH};
    deleteButton->setIcon(deleteIcon);
    deleteButton->setFixedSize({16, 16});
    deleteButton->setIconSize({16, 16});

    auto* lay = new QHBoxLayout;
    lay->setContentsMargins(4, 2, 2, 2);
    lay->setSpacing(2);
    lay->addWidget(deleteButton);
    lay->addWidget(inputWidget);

    setLayout(lay);

    connect(deleteButton, &QPushButton::clicked, this, [&](){
        this->deleteLater();
    });
}
