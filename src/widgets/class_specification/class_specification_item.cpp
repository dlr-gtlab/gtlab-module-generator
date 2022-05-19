#include "class_specification_item.h"

#include "class_specification_widget.h"
#include "module_generator_logger.h"
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
S_LINKED_CLASS_ALT_BTN_TEXT =
        QStringLiteral("<Edit class to extend>");
const QString
ClassSpecificationItem::S_CLASS_BTN_TOOLTIP =
        QStringLiteral("Edit class definition");
const QString
ClassSpecificationItem::S_DELETE_BTN_ICON_PATH =
        QStringLiteral(":/images/cross.png");

ClassSpecificationItem::ClassSpecificationItem(FunctionData function,
                                               ModuleGeneratorSettings* settings,
                                               bool deletable,
                                               QWidget* parent) :
    QWidget(parent),
    AbstractTypeSpecifier(function.implementation),
    m_functionData(std::move(function)),
    m_deletable(deletable),
    m_settings(settings)
{
    auto* baseLayout  = new QHBoxLayout;
    auto* deleteButton = new QPushButton;

    m_derivedClassButton = new QPushButton{S_DERIVED_CLASS_BTN_TEXT};
    m_linkedClassButton  = new QPushButton{S_LINKED_CLASS_BTN_TEXT};
    m_derivedClassButton->setToolTip(S_CLASS_BTN_TOOLTIP);
    m_linkedClassButton->setToolTip(S_CLASS_BTN_TOOLTIP);

    deleteButton->setIconSize({16, 16});
    deleteButton->setFixedSize(16, 16);
    deleteButton->setIcon(QIcon{S_DELETE_BTN_ICON_PATH});

    if (!deletable)
    {
        deleteButton->hide();
        baseLayout->setContentsMargins(0, 0, 0, 0);
    }
    else
    {
        baseLayout->setContentsMargins(2, 2, 2, 2);
    }

    baseLayout->addWidget(deleteButton);
    baseLayout->addWidget(m_linkedClassButton);
    baseLayout->setStretchFactor(m_linkedClassButton, 1);
    baseLayout->addWidget(m_derivedClassButton);
    baseLayout->setStretchFactor(m_derivedClassButton, 1);

    setLayout(baseLayout);

    if (!m_functionData.linkedClass.isValid() &&
        !m_functionData.returnType.startsWith("QMap<"))
    {
        m_linkedClassButton->setVisible(false);
    }

    m_derivedClassWidget = std::make_unique<ClassSpecificationWidget>(
                m_functionData.baseClass, m_settings);

    m_linkedClassWidget  = std::make_unique<ClassSpecificationWidget>(
                m_functionData.linkedClass, m_settings);

    setupSpecificationsWidget(*m_derivedClassWidget);
    setupSpecificationsWidget(*m_linkedClassWidget);

    connect(deleteButton, SIGNAL(pressed()),
            this, SLOT(onDeleteBtnPressed()));
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
ClassSpecificationItem::setupSpecificationsWidget(ClassSpecificationWidget& widget)
{
    widget.setWindowFlag(Qt::Dialog);
    widget.setWindowModality(Qt::ApplicationModal);
    widget.setMinimumWidth(300);
}

void
ClassSpecificationItem::updateSpecificationButton(QPushButton& button,
                                                  ClassData const& implClass,
                                                  bool isLinkedBtn)
{
    auto text = isLinkedBtn ?
                    m_functionData.linkedClass.isValid() ?
                        S_LINKED_CLASS_BTN_TEXT :
                        S_LINKED_CLASS_ALT_BTN_TEXT :
                    S_DERIVED_CLASS_BTN_TEXT;

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
    }

    button.setText(text);
    button.setPalette(palette);
}

QStringList
ClassSpecificationItem::implementationCode()
{
//    LOG_INSTANCE("retrieving single class implemenation values...");

    ClassData implementation = m_derivedClassWidget->implementedClass();

    if (!implementation.isValid())
    {
        LOG_INDENT_ERR("invalid implementation!");
        return {};
    }

    QString returnValue = m_functionData.returnType;

    // QMap<const char*, QMetaObject> -> QMetaObject
    QRegExp regExp{QStringLiteral("QMap<.+,\\s?")};

    returnValue.remove("QList");
    returnValue.remove(regExp);
    returnValue.remove("<");
    returnValue.remove(">");

    QStringList lines;

    // simple return types
    if (returnValue == QStringLiteral("QMetaObject"))
    {
        lines << QString{"GT_METADATA(" + implementation.className + ")"};
    }
    else if (returnValue.endsWith('*'))
    {
        lines << QString{"new " + implementation.className + "{}"};
    }
    else if (returnValue == QStringLiteral("GtCalculatorData") ||
             returnValue == QStringLiteral("GtTaskData"))
    {
        bool isTask = returnValue.contains(QStringLiteral("Task"));
        // GT_EXTENDED_TASK_DATA or GT_EXTENDED_CALC_DATA
        QString makroName = isTask ?
                    QStringLiteral("GT_EXTENDED_TASK_DATA") :
                    QStringLiteral("GT_EXTENDED_CALC_DATA");

        // generate valid variable name
        QString varName = implementation.objectName;
        varName.remove(' ');
        varName.replace(0, 1, varName.at(0).toLower());

        // create variable
        lines << QString{"auto " + varName + " = " +
                          makroName + "(" + implementation.className + ")\t"};
        lines << QString{};
        // proces id
        lines << QString{varName + "->id = " + "QStringLiteral(\""+
                          implementation.objectName + "\")"};
        // process category
        lines << QString{varName + "->category = " + "QStringLiteral(\""+
                        ModuleGenerator::S_ID_MODULE_NAME + "\")"};
        // author
        lines << QString{varName + "->author = " + "QStringLiteral(\""+
                        ModuleGenerator::S_ID_AUTHOR + "\")"};
        // contact
        lines << QString{varName + "->contact = " + "QStringLiteral(\""+
                        ModuleGenerator::S_ID_AUTHOR_EMAIL + "\")"};
        // icon and version
        if (isTask)
        {
            lines << ModuleGenerator::S_2_0_VERSION_CHECK;
            lines << varName + "->icon = gtApp->icon(QStringLiteral(\""
                               "processIcon.png\"))";
            lines << varName + "->version = 1";
            lines << "#else";
            lines << varName + "->icon = GtGUI::Icon::process16()";
            lines << varName + "->version = GtVersionNumber(0, 0, 1)";
            lines << "#endif";
        }
        else
        {
            lines << ModuleGenerator::S_2_0_VERSION_CHECK;
            lines << varName + "->icon = gtApp->icon(QStringLiteral(\""
                               "calculatorIcon.png\"))";
            lines << "#else";
            lines << varName + "->icon = GtGUI::Icon::process16()";
            lines << "#endif";
            lines << varName + "->version = GtVersionNumber(0, 0, 1)";
        }
        // status
        lines << QString{varName + "->status = GtAbstractProcessData::PROTOTYPE"};
        // return variable
        lines << varName;
    }
    else
    {
        LOG_INDENT_ERR("return value not implemented: " + returnValue);
        return  {};
    }

    return lines;
}


QStringList
ClassSpecificationItem::additionalIncludes()
{
    // for version check
    if (m_functionData.returnType.contains(QStringLiteral("Task")) ||
        m_functionData.returnType.contains(QStringLiteral("Calc")))
    {
        return ModuleGenerator::S_2_0_ICON_INCLUDES;
    }
    auto linkedClass = m_linkedClassWidget->implementedClass();

    // generate include if linked class was used for specifing external class
    if (!m_functionData.linkedClass.isValid() && linkedClass.isValid())
    {
        return { linkedClass.fileName };
    }

    return {};
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
ClassSpecificationItem::onDeleteBtnPressed()
{
    emit deleted(this);

    disconnect(this);

    this->deleteLater();
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
                              m_derivedClassWidget->implementedClass(),
                              false);
}

void
ClassSpecificationItem::onLinkedClassBtnPressed()
{
    m_linkedClassWidget->show();
}

void
ClassSpecificationItem::onHideLinkedClassWidget()
{
    auto impl = m_linkedClassWidget->implementedClass();

    updateSpecificationButton(*m_linkedClassButton, impl, true);
}
