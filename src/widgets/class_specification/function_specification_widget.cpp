#include "function_specification_widget.h"

#include "widget_list_view.h"

#include "type_specification_widget.h"
#include "class_specification_item.h"
#include "class_specification_list.h"
#include "module_generator.h"
#include "module_generator_logger.h"
#include "module_generator_settings.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpacerItem>


FunctionSpecificationWidget::FunctionSpecificationWidget(FunctionDataList functions,
                                                         ModuleGeneratorSettings* settings,
                                                         QWidget* parent)
    : QScrollArea(parent),
      m_functions(std::move(functions)),
      m_settings(settings)
{
    auto* scrollWidget = new QWidget;
    m_baseLayout   = new QGridLayout;

    setContent();

    scrollWidget->setLayout(m_baseLayout);

    setFrameShape(QFrame::NoFrame);

    setWidgetResizable(true);
    setWidget(scrollWidget);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

int
FunctionSpecificationWidget::count() const
{
    return m_specificationWidgets.count();
}

bool
FunctionSpecificationWidget::isEmpty() const
{
    return this->count() == 0;
}



FunctionDataList
FunctionSpecificationWidget::implementedFunctions()
{
    QMapIterator<QString, AbstractTypeSpecifier*> iterator{
        m_specificationWidgets
    };

    while (iterator.hasNext())
    {
        auto item = iterator.next();

        if (!item.value())
        {
            LOG_INDENT_ERR("Null Interface");
            continue;
        }

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
                     function.returnType + ' ' +
                     function.name + "'...");

        auto* widget = getSpecificationWidget(function);
        auto* inputWidget = dynamic_cast<QWidget*>(widget);

        if (!inputWidget)
        {
            generateStdImplementation(function);
            LOG_INFO << "done!";
            continue;
        }

        inputWidget->setToolTip(function.tooltip);

        // shorten return type:
        //  QMap<A, B>
        // to
        //  QMap<A,
        //       B>
        auto returnType = function.returnType;
        auto tIdx = returnType.indexOf('<');
        auto cIdx = returnType.indexOf(',');
        if (cIdx > tIdx && tIdx > 0)
        {
            returnType.replace(',', ",\n" + QString{' '}.repeated(tIdx));
        }

        auto* returnTypeLabel = new QLabel{returnType};
        auto* functionLabel   = new QLabel{function.name};

        QPalette palette;
        palette.setColor(QPalette::WindowText, Qt::darkBlue);

        returnTypeLabel->setMinimumHeight(20);
        returnTypeLabel->setFont(ModuleGeneratorSettings::F_MONO_FONT);
        returnTypeLabel->setPalette(palette);

        functionLabel->setFont(ModuleGeneratorSettings::F_MONO_FONT);
        functionLabel->setMinimumHeight(20);

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

    auto* spacer = new QSpacerItem{1, 1, QSizePolicy::Minimum,
                                   QSizePolicy::MinimumExpanding};

    m_baseLayout->addItem(spacer, this->count() + 1, 0);

    LOG_INFO << "done!";
}

void
FunctionSpecificationWidget::generateStdImplementation(FunctionData& function)
{
    auto& returnValue = function.returnType;

    QStringList lines;

    LOG_INDENT("setting standard implementation: " + returnValue);

    // simple return types
    if (returnValue == QStringLiteral("void"))
    {
        lines << QStringLiteral("// TODO: add implementation");
    }
    // icons changed in 2.0
    else if (returnValue == QStringLiteral("QIcon"))
    {
        QString iconName = "list"; // defaults to list icon

        // hacky soloution to find icon name
        if (function.tooltip.contains("collection")) iconName = "collection";
        if (function.tooltip.contains("import")) iconName = "import";
        if (function.tooltip.contains("export")) iconName = "export";

        lines << ModuleGenerator::S_2_0_VERSION_CHECK;
        lines << "return gtApp->icon(QStringLiteral(\"" +
                 iconName + "Icon.png\"))";
        lines << "#else";
        lines << "return GtGUI::Icon::" + iconName + "16()";
        lines << "#endif";

        // includes for icons
        function.implementation.includes += ModuleGenerator::S_2_0_ICON_INCLUDES;
    }
    else if (returnValue == QStringLiteral("QStringList") ||
             returnValue.startsWith(QStringLiteral("QMap<")) ||
             returnValue.startsWith(QStringLiteral("QList<")))
    {
        lines << QString{returnValue + " retVal"};
        lines << QString{};
        lines << QStringLiteral("retVal");
    }
    else
    {
        lines << QStringLiteral("{}");
//        LOG_ERR << "no standard implementation set!";
    }

    function.implementation.lines = lines;

    LOG_INFO << "done!";
}

AbstractTypeSpecifier*
FunctionSpecificationWidget::getSpecificationWidget(FunctionData const& function)
{
    auto& returnValue = function.returnType;

    // simple return types
    if (returnValue == QStringLiteral("bool"))
    {
        return new BoolSpecificationWidget(function.implementation);
    }

    if (returnValue == QStringLiteral("QString"))
    {
        return new StringSpecificationWidget(function.implementation);
    }

//    if (returnValue == QStringLiteral("QIcon"))
//    {
//        QStringList values;
//        values << QStringLiteral("-");
//        values << QStringLiteral("exportIcon");
//        values << QStringLiteral("importIcon");
//        values << QStringLiteral("collectionIcon");

//        return new ComboboxSpecificationWidget(function.implementation,
//                                               values,
//                                               "gtApp->icon(\"", ".png\")");
//    }

    if (returnValue == QStringLiteral("Qt::DockWidgetArea"))
    {
        QStringList values;
        values << QStringLiteral("Qt::RightDockWidgetArea");
        values << QStringLiteral("Qt::LeftDockWidgetArea");
        values << QStringLiteral("Qt::TopDockWidgetArea");
        values << QStringLiteral("Qt::BottomDockWidgetArea");

        return new ComboboxSpecificationWidget(function.implementation, values);
    }

    // complex return types
    ClassData baseClass = function.baseClass;

    if (baseClass.className.isEmpty())
    {
        LOG_INDENT_WARN("null base class!");
        return {};
    }

    if (returnValue == QStringLiteral("QMetaObject"))
    {
        return new ClassSpecificationItem(function, m_settings, false);
    }

    if (returnValue == QStringLiteral("QList<QMetaObject>") ||
        returnValue == QStringLiteral("QList<GtAbstractProperty*>") ||
        returnValue == QStringLiteral("QList<GtCalculatorData>") ||
        returnValue == QStringLiteral("QList<GtTaskData>") ||
        returnValue == QStringLiteral("QMap<const char*, QMetaObject>"))
    {
        return new ClassSpecificationList(function, m_settings);
    }

    LOG_INDENT_WARN("no specification widget found!");

    return {};
}

