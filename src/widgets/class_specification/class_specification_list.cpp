#include "class_specification_list.h"

#include "class_specification_item.h"

#include "module_generator_logger.h"

#include "widget_list_view.h"

#include <QPushButton>
#include <QVBoxLayout>

const int
ClassSpecificationList::I_LIST_HEIGTH = 100;

const QString
ClassSpecificationList::S_ADD_CLASS_BTN =
        QStringLiteral("Add Class ...");

ClassSpecificationList::ClassSpecificationList(const FunctionData& function,
                                               ModuleGeneratorSettings* settings,
                                               QWidget *parent) :
    QWidget(parent),
    AbstractTypeSpecifier(function.implementation),
    m_functionData(function),
    m_settings(settings)
{
    auto* baseLayout = new QVBoxLayout;

    m_addButton = new QPushButton{S_ADD_CLASS_BTN};
    m_widgetListView = new WidgetListView;
    m_widgetListView->setMinimumHeight(I_LIST_HEIGTH);

    baseLayout->addWidget(m_widgetListView);
    baseLayout->addWidget(m_addButton);

    baseLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(baseLayout);

    connect(m_addButton, SIGNAL(pressed()), this, SLOT(onAddButtonPressed()));
}

QStringList
ClassSpecificationList::implementationCode()
{
    QStringList lines;

    // create variable -> QList<...> retVal
    lines << QString{m_functionData.returnType + " retVal"};
    lines << QString{};

    for (auto* w : m_widgetListView->widgets())
    {
        auto* widget = qobject_cast<ClassSpecificationItem*>(w);

        if (!widget) continue;

        QStringList implementation = widget->implementationCode();

        if (implementation.isEmpty()) continue;

        // implementation contains multiple lines
        if (implementation.length() > 1)
        {
            if (lines.length() > 2) lines << QString{}; // for nicer indent
            lines << implementation.mid(0, implementation.length() - 1);
            lines << QString{};
        }

        // should have one entry
        auto linkedClass = widget->linkedClasses().last();

        // append implementation to retVal
        if (linkedClass.className.isEmpty())
        {
            // QList
            lines << QString{"retVal << " + implementation.last()};
        }
        else
        {
            // QMap
            lines << QString{"retVal.insert(GT_CLASSNAME(" +
                             linkedClass.className + "), " +
                             implementation.last() + ')'};
        }
    }

    // return variable
    if (lines.length() > 2) lines << QString{}; // for nicer indent
    lines << "retVal";

    return lines;
}

QStringList
ClassSpecificationList::additionalIncludes()
{
    QStringList includes;

    for (auto* w : m_widgetListView->widgets())
    {
        auto* widget = qobject_cast<ClassSpecificationItem*>(w);

        if (!widget) continue;

        includes << widget->additionalIncludes();
    }

    return includes;
}

ClassDataList
ClassSpecificationList::derivedClasses()
{
    ClassDataList classes;

    for (auto* w : m_widgetListView->widgets())
    {
        auto* widget = dynamic_cast<ClassSpecificationItem*>(w);

        if (!widget) continue;

        classes << widget->derivedClasses()[0]; // must have one entry
    }

    return classes;
}

ClassDataList
ClassSpecificationList::linkedClasses()
{
    ClassDataList classes;

    for (auto* w : m_widgetListView->widgets())
    {
        auto* widget = dynamic_cast<ClassSpecificationItem*>(w);

        if (!widget) continue;

        classes << widget->linkedClasses(); // may have one entry
    }

    return classes;
}

void
ClassSpecificationList::onAddButtonPressed()
{
    LOG_INDENT("Adding new class for '" + m_functionData.name + "'...");

    auto* widget = new ClassSpecificationItem(m_functionData,
                                              m_settings, true);

    m_widgetListView->insertWidget(-1, widget);

    LOG_INFO << "done!";
}
