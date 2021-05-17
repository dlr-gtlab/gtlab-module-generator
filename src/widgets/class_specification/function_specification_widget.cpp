#include "function_specification_widget.h"

#include "widget_list_view.h"

#include "simple_class_specification_widget.h"
#include "single_class_specification_widget.h"
#include "multiple_class_specification_widget.h"
#include "module_generator_logger.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpacerItem>

const QFont F_LABEL_FONT = QFont(QStringLiteral("Consolas"), 9);


FunctionSpecificationWidget::FunctionSpecificationWidget(FunctionStructsPtr functions,
                                                         ModuleGeneratorSettings* settings,
                                                         QWidget* parent)
    : QScrollArea(parent), m_functions(functions), m_settings(settings)
{
    m_scrollWidget = new QWidget();
    m_baseLayout   = new QGridLayout();

    setContent();

    m_scrollWidget->setLayout(m_baseLayout);

    setFrameShape(QFrame::NoFrame);

    setWidgetResizable(true);
    setWidget(m_scrollWidget);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

int
FunctionSpecificationWidget::count() const
{
    return m_widgets.length();
}

bool
FunctionSpecificationWidget::isEmpty() const
{
    return this->count() == 0;
}

void
FunctionSpecificationWidget::setImplementation()
{
    for (AbstractClassSpecification* widget : m_widgets)
    {
        if (widget == Q_NULLPTR) continue;

        widget->setImplementation();
    }
}

void
FunctionSpecificationWidget::setContent()
{
    LOG_INSTANCE("setting content...");

    for (auto* function : m_functions)
    {
        if (function == Q_NULLPTR)
        {
            LOG_ERR << "null fucntion!";
            continue;
        }

        LOG_INSTANCE("setting specifcation widget for '" +
                     function->returnValue + ' ' +
                     function->name + "'...");

        AbstractClassSpecification* widget = setSpecificationWidget(*function);
        QWidget* inputWidget = dynamic_cast<QWidget*>(widget);

        if (inputWidget == Q_NULLPTR)
        {
            setStandardImplementation(*function);
            continue;
        }

        QLabel* returnType   = new QLabel(function->returnValue);
        QLabel* functionName = new QLabel(function->name);

        returnType->setMinimumHeight(20);
        returnType->setFont(F_LABEL_FONT);
        returnType->setStyleSheet("color : DarkBlue;");

        functionName->setFont(F_LABEL_FONT);
        functionName->setMinimumHeight(20);
        functionName->setStyleSheet("color : black;");

        returnType->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
        functionName->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

        int widgetCount = this->count();

        m_baseLayout->addWidget(functionName, widgetCount, 0, 1, 1, Qt::AlignTop);
        m_baseLayout->addWidget(returnType, widgetCount, 1, 1, 1, Qt::AlignTop);
        m_baseLayout->addWidget(inputWidget, widgetCount, 2, 1, 1, Qt::AlignTop);

        m_widgets << widget;

        LOG_INFO << "done!";
    }

    auto* spacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    m_baseLayout->addItem(spacer, this->count() + 1, 0, 1, 1);

    LOG_INFO << "done!";
}

void
FunctionSpecificationWidget::setStandardImplementation(FunctionStruct& function)
{

    QString returnValue = function.returnValue;

    QStringList values;

    LOG_INSTANCE("setting standard implementation: " + returnValue);

    // simple return types
    if (returnValue == QStringLiteral("void"))
    {
        values << QStringLiteral("/* TODO: add implementation */");
    }
    else if (returnValue == QStringLiteral("QVariant"))
    {
        values << QStringLiteral("QVariant()");
    }
    else if (returnValue.endsWith('*'))
    {
        values << QStringLiteral("Q_NULLPTR");
    }
    else if (returnValue == QStringLiteral("QStringList") ||
             returnValue.startsWith(QStringLiteral("QMap<")) ||
             returnValue.startsWith(QStringLiteral("QList<")))
    {
        values << QString(returnValue + " retVal");
        values << "";
        values << QStringLiteral("retVal");
    }
    else
    {
        LOG_ERR << "no standard implementation set!";
    }

    function.implementation.values = values;
}

AbstractClassSpecification*
FunctionSpecificationWidget::setSpecificationWidget(FunctionStruct& function)
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
        values << QStringLiteral("");
        values << QStringLiteral("notificationIcon.png");

        return new ComboClassSpecificationWidget(function.implementation,
                                                 values, "QIcon(\"", "\")");
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
    ClassStruct* baseClass = function.baseClass;

    if (baseClass == Q_NULLPTR)
    {
        LOG_INSTANCE("null base class!", ModuleGeneratorLogger::Type::Warning);
        return Q_NULLPTR;
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

    LOG_INSTANCE("no specification widget found!", ModuleGeneratorLogger::Type::Warning);

    return Q_NULLPTR;
}

