#include "function_specification_widget.h"

#include "widget_list_view.h"

#include "simple_class_specification_widget.h"
#include "single_class_specification_widget.h"
#include "multiple_class_specification_widget.h"
#include "module_generator_logger.h"
#include "module_generator_settings.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpacerItem>


FunctionSpecificationWidget::FunctionSpecificationWidget(const FunctionStructs& functions,
                                                         ModuleGeneratorSettings* settings,
                                                         QWidget* parent)
    : QScrollArea(parent),
      m_functions(functions),
      m_settings(settings)
{
    auto* scrollWidget = new QWidget;
    m_baseLayout   = new QGridLayout;

    setContent();

    scrollWidget->setLayout(m_baseLayout);

    setFrameShape(QFrame::NoFrame);

    setWidgetResizable(true);
    setWidget(scrollWidget);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

int
FunctionSpecificationWidget::count() const
{
    return m_specificationWidgets.keys().length();
}

bool
FunctionSpecificationWidget::isEmpty() const
{
    return this->count() == 0;
}



FunctionStructs
FunctionSpecificationWidget::implementedFunctions()
{
    QMapIterator<QString, AbstractClassSpecification*> iterator{
        m_specificationWidgets
    };

    while (iterator.hasNext())
    {
        auto item = iterator.next();

        if (!item.value()) continue;

        for (auto& f : m_functions)
        {
            if (f.name != item.key()) continue;

            f.implementation = item.value()->functionImplementation();
            break;
        }
    }

    return m_functions;
}

void
FunctionSpecificationWidget::setContent()
{
    LOG_INDENT("setting content...");

    for (auto& function : m_functions)
    {
        LOG_INDENT("setting specifcation widget for '" +
                     function.returnValue + ' ' +
                     function.name + "'...");

        auto* widget = setSpecificationWidget(function);
        auto* inputWidget = dynamic_cast<QWidget*>(widget);

        if (!inputWidget)
        {
            setStandardImplementation(function);
            LOG_INFO << "done!";
            continue;
        }

        inputWidget->setToolTip(function.tooltip);

        auto* returnTypeLabel = new QLabel(function.returnValue);
        auto* functionLabel   = new QLabel(function.name);

        returnTypeLabel->setMinimumHeight(20);
        returnTypeLabel->setFont(ModuleGeneratorSettings::F_MONO_FONT);
        returnTypeLabel->setStyleSheet("QLabel { color : DarkBlue }");

        functionLabel->setFont(ModuleGeneratorSettings::F_MONO_FONT);
        functionLabel->setMinimumHeight(20);
        functionLabel->setStyleSheet("QLabel { color : black }");

        returnTypeLabel->setSizePolicy(QSizePolicy::Minimum,
                                       QSizePolicy::Minimum);
        functionLabel->setSizePolicy(QSizePolicy::Minimum,
                                     QSizePolicy::Minimum);

        int widgetCount = this->count();

        m_baseLayout->addWidget(functionLabel, widgetCount, 0, 1, 1,
                                Qt::AlignTop);
        m_baseLayout->addWidget(returnTypeLabel, widgetCount, 1, 1, 1,
                                Qt::AlignTop);
        m_baseLayout->addWidget(inputWidget, widgetCount, 2, 1, 1,
                                Qt::AlignTop);

        m_specificationWidgets.insert(function.name, widget);

        LOG_INFO << "done!";
    }

    auto* spacer = new QSpacerItem(1, 1, QSizePolicy::Minimum,
                                   QSizePolicy::MinimumExpanding);

    m_baseLayout->addItem(spacer, this->count() + 1, 0, 1, 1);

    LOG_INFO << "done!";
}

void
FunctionSpecificationWidget::setStandardImplementation(FunctionStruct& function)
{
    QString returnValue = function.returnValue;

    QStringList values;

    LOG_INDENT("setting standard implementation: " + returnValue);

    // simple return types
    if (returnValue == QStringLiteral("void"))
    {
        values << QStringLiteral("/* TODO: add implementation */");
    }
    else if (returnValue == QStringLiteral("QStringList") ||
             returnValue.startsWith(QStringLiteral("QMap<")) ||
             returnValue.startsWith(QStringLiteral("QList<")))
    {
        values << QString{returnValue + " retVal"};
        values << QString{};
        values << QStringLiteral("retVal");
    }
    else
    {
        values << QStringLiteral("{}");
//        LOG_ERR << "no standard implementation set!";
    }

    function.implementation.values = values;

    LOG_INFO << "done!";
}

AbstractClassSpecification*
FunctionSpecificationWidget::setSpecificationWidget(const FunctionStruct& function)
{
    QString returnValue = function.returnValue;

    // simple return types
    if (returnValue == QStringLiteral("bool"))
    {
        return new BoolClassSpecificationWidget(function.implementation);
    }
    if (returnValue == QStringLiteral("QString"))
    {
        return new StringClassSpecificationWidget(function.implementation);
    }
    if (returnValue == QStringLiteral("QIcon"))
    {
        QStringList values;
        values << QStringLiteral("-");
        values << QStringLiteral("exportIcon");
        values << QStringLiteral("importIcon");
        values << QStringLiteral("collectionIcon");

        return new ComboClassSpecificationWidget(function.implementation,
                                                 values,
                                                 "gtApp->icon(\"", ".png\")");
    }
    if (returnValue == QStringLiteral("Qt::DockWidgetArea"))
    {
        QStringList values;
        values << QStringLiteral("Qt::RightDockWidgetArea");
        values << QStringLiteral("Qt::LeftDockWidgetArea");
        values << QStringLiteral("Qt::TopDockWidgetArea");
        values << QStringLiteral("Qt::BottomDockWidgetArea");

        return new ComboClassSpecificationWidget(function.implementation,
                                                 values);
    }

    // complex return types
    ClassStruct baseClass = function.baseClass;

    if (baseClass.className.isEmpty())
    {
        LOG_INDENT("null base class!", ModuleGeneratorLogger::Type::Warning);
        return {};
    }

    if (returnValue == QStringLiteral("QMetaObject"))
    {
        return new SingleClassSpecificationWidget(function, m_settings, false);
    }

    if (returnValue == QStringLiteral("QList<QMetaObject>") ||
        returnValue == QStringLiteral("QList<GtAbstractProperty*>") ||
        returnValue == QStringLiteral("QList<GtCalculatorData>") ||
        returnValue == QStringLiteral("QList<GtTaskData>") ||
        returnValue == QStringLiteral("QMap<const char*, QMetaObject>"))
    {
        return new MultipleClassSpecificationWidget(function, m_settings);
    }

    LOG_INDENT("no specification widget found!",
               ModuleGeneratorLogger::Type::Warning);

    return {};
}

