#include "single_class_specification_widget.h"

#include "class_specification_widget.h"
#include "module_generator_logger.h"
#include "module_generator.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>

const int I_SPECIFICATIONS_PUSHBTN_WIDTH = 40;

const QString S_CLASS_NAME_PLACEHOLDER =
        QStringLiteral("Class name");
const QString S_LINKED_CLASS_EDIT_PLACEHOLDER =
        QStringLiteral("Linked class name");
const QString S_LINKED_CLASS_BTN_TEXT =
        QStringLiteral("-Generate linked class-");

const QString S_SPECIFICATIONS_BTN_TOOLTIP =
        QStringLiteral("Edit class definition");
const QString S_LINKED_SPECIFICATIONS_BTN_TOOLTIP =
        QStringLiteral("Edit linked class definition");

const QString S_SPECIFICATIONS_BTN_TEXT = QStringLiteral("...");
const QString S_DELETE_BTN_ICON_PATH = QStringLiteral(":/images/cross.png");

SingleClassSpecificationWidget::SingleClassSpecificationWidget(const FunctionStruct& function,
                                                               ModuleGeneratorSettings* settings,
                                                               bool deletable,
                                                               bool editable,
                                                               QWidget* parent) :
    QWidget(parent),
    AbstractClassSpecification(function.implementation),
    m_functionStruct(function),
    m_deletable(deletable),
    m_editable(editable)
{
    m_baseLayout  = new QHBoxLayout();

    m_linkedNameLineEdit = new QLineEdit();

    m_deleteButton = new QPushButton();
    m_specificationsButton = new QPushButton(S_SPECIFICATIONS_BTN_TEXT);
    m_linkedClassButton    = new QPushButton(S_LINKED_CLASS_BTN_TEXT);



    m_deleteButton->setIconSize(QSize(16, 16));
    m_deleteButton->setFixedSize(16, 16);
    m_deleteButton->setIcon(QIcon(S_DELETE_BTN_ICON_PATH));

    m_specificationsButton->setFixedWidth(I_SPECIFICATIONS_PUSHBTN_WIDTH);
    m_specificationsButton->setToolTip(S_SPECIFICATIONS_BTN_TOOLTIP);

    m_linkedClassButton->setToolTip(S_LINKED_SPECIFICATIONS_BTN_TOOLTIP);

    if (!deletable)
    {
        m_deleteButton->hide();

        m_baseLayout->setContentsMargins(0, 0, 0, 0);
    }
    else
    {
        m_baseLayout->setContentsMargins(2, 2, 2, 2);
    }

    if (!editable)
    {
        m_linkedNameLineEdit->setEnabled(false);
        m_linkedNameLineEdit->setPlaceholderText(S_CLASS_NAME_PLACEHOLDER);
    }
    else
    {
        m_linkedNameLineEdit->setPlaceholderText(S_LINKED_CLASS_EDIT_PLACEHOLDER);
    }

    if (function.linkedClass.isValid())
    {
        m_linkedNameLineEdit->setVisible(false);
    }
    else
    {
        m_linkedClassButton->setVisible(false);
    }

    m_baseLayout->addWidget(m_deleteButton);
    m_baseLayout->addWidget(m_linkedNameLineEdit);
    m_baseLayout->addWidget(m_linkedClassButton);
    m_baseLayout->addWidget(m_specificationsButton);

    setLayout(m_baseLayout);

    m_specificationsWidget       = new ClassSpecificationWidget(m_functionStruct, settings);
    m_linkedSpecificationsWidget = new ClassSpecificationWidget(m_functionStruct, settings, true);

    setupSpecificationsWidget(m_specificationsWidget);
    setupSpecificationsWidget(m_linkedSpecificationsWidget);

    m_specificationsWidget->setWindowTitle(m_functionStruct.baseClass.className);
    m_linkedSpecificationsWidget->setWindowTitle(m_functionStruct.linkedClass.className);

    connect(m_deleteButton, SIGNAL(pressed()),
            this, SLOT(onDeleteBtnPressed()));
    connect(m_specificationsButton, SIGNAL(pressed()),
            this, SLOT(onSpecificationsBtnPressed()));
    connect(m_specificationsWidget, SIGNAL(hidden()),
            this, SLOT(onHideSpecficationsWidget()));

    connect(m_linkedClassButton, SIGNAL(pressed()),
            this, SLOT(onLinkedSpecificationsBtnPressed()));
    connect(m_linkedSpecificationsWidget, SIGNAL(hidden()),
            this, SLOT(onHideLinkedSpecficationsWidget()));
}

SingleClassSpecificationWidget::~SingleClassSpecificationWidget()
{
    delete m_specificationsWidget;
    delete m_linkedSpecificationsWidget;
}

void
SingleClassSpecificationWidget::setupSpecificationsWidget(ClassSpecificationWidget* widget)
{
    widget->setWindowFlag(Qt::Dialog);
    widget->setWindowModality(Qt::ApplicationModal);
    widget->setMinimumWidth(300);
    widget->setMinimumHeight(150);
}

QString
SingleClassSpecificationWidget::linkedClassName() const
{
    return m_linkedNameLineEdit->text();
}

QStringList
SingleClassSpecificationWidget::implementationValues()
{
//    LOG_INSTANCE("retrieving single class implemenation values...");

    ClassStruct implementation = m_specificationsWidget->implementedClass();

    if (!implementation.isValid())
    {
        LOG_INSTANCE("invalid implementation!", ModuleGeneratorLogger::Type::Error);
        return QStringList();
    }

    QString returnValue = m_functionStruct.returnValue;

    // QMap<const char*, QMetaObject> -> QMetaObject
    QString pattern(QStringLiteral("QMap<.+,\\s?"));
    QRegExp regExp(pattern);

    returnValue.remove("QList");
    returnValue.remove(regExp);
    returnValue.remove("<");
    returnValue.remove(">");

    QStringList values;

    // simple return types
    if (returnValue == QStringLiteral("QMetaObject"))
    {
        values << QString("GT_METADATA(" + implementation.className + ")");
    }
    else if (returnValue.endsWith('*'))
    {
        values << QString ("new " + implementation.className + "()");
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
        QString variableName = implementation.objectName;
        variableName.remove(' ');
        variableName.replace(0, variableName.at(0).toLower());

        // create variable
        returnValue.insert(2, "Extended");
        values << QString(returnValue + ' ' + variableName + " = " +
                          makroName + '(' + implementation.className + ')');
        values << "";
        // proces id
        values << QString(variableName + "->id = " + "QStringLiteral(\""+
                          implementation.objectName + "\")");
        // process category
        values << QString(variableName + "->category = " + "QStringLiteral(\""+
                          ModuleGenerator::S_ID_MODULE_NAME + "\")");
        // author
        values << QString(variableName + "->author = " + "QStringLiteral(\""+
                          ModuleGenerator::S_ID_AUTHOR + "\")");
        // icon
        values << QString(variableName + "->icon = " + "gtApp->icon(QStringLiteral(\"" +
                          QString(isTask ? "taskIcon":"calculatorIcon") + ".png\"))");
        // version
        if (isTask) values << QString(variableName + "->version = 1");
        else values << QString(variableName + "->version = GtVersionNumber(0, 0, 1)");
        // status
        values << QString(variableName + "->status = GtAbstractProcessData::PROTOTYPE");
        // return variable
        values << variableName;
    }
    else
    {
        LOG_INSTANCE("return value not implemented: " + returnValue, ModuleGeneratorLogger::Type::Error);
        return QStringList();
    }

    return values;
}

ClassStructs
SingleClassSpecificationWidget::derivedClasses()
{
    ClassStructs list;

    list << m_specificationsWidget->implementedClass();

    return list;
}

ClassStructs
SingleClassSpecificationWidget::linkedClasses()
{
    ClassStructs list;

    list << m_linkedSpecificationsWidget->implementedClass();

    return list;
}

void
SingleClassSpecificationWidget::onDeleteBtnPressed()
{
    emit deleted(this);

    disconnect(this);

    this->deleteLater();
}

void
SingleClassSpecificationWidget::onSpecificationsBtnPressed()
{
    m_specificationsWidget->show();
}

void
SingleClassSpecificationWidget::onHideSpecficationsWidget()
{
    if (!m_editable && m_linkedNameLineEdit->text().isEmpty())
    {
        m_linkedNameLineEdit->setText(m_specificationsWidget->implementedClass().className);
    }
}

void
SingleClassSpecificationWidget::onLinkedSpecificationsBtnPressed()
{
    m_linkedSpecificationsWidget->show();
}

void
SingleClassSpecificationWidget::onHideLinkedSpecficationsWidget()
{
    QString className(m_linkedSpecificationsWidget->implementedClass().className);

    m_linkedNameLineEdit->setText(className);

    if (className.isEmpty())
    {
        m_linkedClassButton->setText(S_LINKED_CLASS_BTN_TEXT);
    }
    else
    {
        m_linkedClassButton->setText(className);
    }
}
