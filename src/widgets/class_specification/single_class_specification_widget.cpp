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
const QString S_LINKED_CLASS_PLACEHOLDER =
        QStringLiteral("Linked class");

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

    m_nameLineEdit = new QLineEdit();
    m_deleteButton = new QPushButton();
    m_specificationsButton = new QPushButton(S_SPECIFICATIONS_BTN_TEXT);

    m_deleteButton->setIconSize(QSize(16, 16));
    m_deleteButton->setFixedSize(16, 16);
    m_deleteButton->setIcon(QIcon(S_DELETE_BTN_ICON_PATH));

    m_specificationsButton->setFixedWidth(I_SPECIFICATIONS_PUSHBTN_WIDTH);

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
        m_nameLineEdit->setEnabled(false);
        m_nameLineEdit->setPlaceholderText(S_CLASS_NAME_PLACEHOLDER);
    }
    else
    {
        m_nameLineEdit->setPlaceholderText(S_LINKED_CLASS_PLACEHOLDER);
    }

    m_baseLayout->addWidget(m_deleteButton);
    m_baseLayout->addWidget(m_nameLineEdit);
    m_baseLayout->addWidget(m_specificationsButton);

    setLayout(m_baseLayout);

    m_specificationsWidget = new ClassSpecificationWidget(m_functionStruct, settings);
    m_specificationsWidget->setWindowFlag(Qt::Dialog);
    m_specificationsWidget->setWindowModality(Qt::ApplicationModal);
    m_specificationsWidget->setMinimumWidth(300);
    m_specificationsWidget->setMinimumHeight(150);

    m_specificationsWidget->setWindowTitle(m_functionStruct.baseClass.className);

    connect(m_deleteButton, SIGNAL(pressed()),
            this, SLOT(onDeleteBtnPressed()));
    connect(m_specificationsButton, SIGNAL(pressed()),
            this, SLOT(onSpecificationsBtnPressed()));
    connect(m_specificationsWidget, SIGNAL(hidden()),
            this, SLOT(onHideSpecficationsWidget()));
}

SingleClassSpecificationWidget::~SingleClassSpecificationWidget()
{
    delete m_specificationsWidget;
}

QString
SingleClassSpecificationWidget::linkedClassName() const
{
    return m_nameLineEdit->text();
}

QStringList
SingleClassSpecificationWidget::implementationValues()
{
//    LOG_INSTANCE("retrieving single class implemenation values...");

    ClassStruct implementation = m_specificationsWidget->implementedClass();
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
        values << QString();
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
    if (!m_editable || m_nameLineEdit->text().isEmpty())
    {
        m_nameLineEdit->setText(m_specificationsWidget->implementedClass().className);
    }
}
