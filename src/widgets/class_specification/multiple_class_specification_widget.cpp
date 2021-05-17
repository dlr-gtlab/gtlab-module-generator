#include "multiple_class_specification_widget.h"

#include "single_class_specification_widget.h"

#include "module_generator_logger.h"

#include "widget_list_view.h"

#include <QPushButton>
#include <QVBoxLayout>

const QString S_ADD_CLASS_BTN = QStringLiteral("Add Class ...");

MultipleClassSpecificationWidget::MultipleClassSpecificationWidget(FunctionStruct& function,
                                                                   ModuleGeneratorSettings* settings,
                                                                   QWidget *parent) :
    QWidget(parent),
    AbstractClassSpecification(function.implementation),
    m_functionStruct(function),
    m_settings(settings)
{
    m_baseLayout = new QVBoxLayout();

    m_addButton = new QPushButton(S_ADD_CLASS_BTN);
    m_widgetListView = new WidgetListView();

    m_baseLayout->addWidget(m_widgetListView);
    m_baseLayout->addWidget(m_addButton);

    m_baseLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(m_baseLayout);

    connect(m_addButton, SIGNAL(pressed()), this, SLOT(onAddButtonPressed()));
}

QStringList
MultipleClassSpecificationWidget::implementationValues()
{
    QStringList values;

    // create variable -> QList<...> retVal
    values << QString(m_functionStruct.returnValue + " retVal");
    values << "";

    for (auto* w : m_widgetListView->widgets())
    {
        auto* widget = dynamic_cast<SingleClassSpecificationWidget*>(w);

        if (widget == Q_NULLPTR) continue;

        QStringList implementation = widget->implementation();

        if (implementation.isEmpty()) continue;

        // implementation contaisn multiple lines
        if (implementation.length() > 1)
        {
            if (values.length() < 2) values << ""; // for
            values << implementation.mid(0, implementation.length() - 1);
            values << "";
        }

        // append implementation to retVal
        if (!widget->editable())
        {
            // QList
            values << QString("retVal << " + implementation.last());
        }
        else
        {
            // QMap
            values << QString("retVal.insert(GT_CLASSNAME(" +
                              widget->linkedClassName() + "), " +
                              implementation.last() + ')');
        }
    }

    // return variable
    values << "";
    values << "retVal";

    return values;
}

ClassStructs
MultipleClassSpecificationWidget::derivedClasses()
{
    ClassStructs classes;

    for (auto* w : m_widgetListView->widgets())
    {
        auto* widget = dynamic_cast<SingleClassSpecificationWidget*>(w);

        if (widget == Q_NULLPTR) continue;

        classes << widget->classImplementation();
    }

    return classes;
}

void
MultipleClassSpecificationWidget::onAddButtonPressed()
{
    bool editable = m_functionStruct.returnValue.startsWith(QStringLiteral("QMap<"));

    auto* widget = new SingleClassSpecificationWidget(m_functionStruct, m_settings, true, editable);

    connect(widget, SIGNAL(deleted(SingleClassSpecificationWidget*)),
            this, SLOT(onWidgetDeleted(SingleClassSpecificationWidget*)));

    m_widgetListView->insertWidget(-1, widget);
}

void
MultipleClassSpecificationWidget::onWidgetDeleted(SingleClassSpecificationWidget* widget)
{
    auto* w = dynamic_cast<QWidget*>(widget);

    if (w == Q_NULLPTR) return;

    m_widgetListView->removeWidget(w);
}
