#include "class_specification_widget.h"

#include "function_specification_widget.h"
#include "module_generator.h"
#include "module_generator_settings.h"
#include "module_generator_logger.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QRegularExpressionValidator>
#include <QCheckBox>

const QString S_CLASS_NAME_LABEL =
        QStringLiteral("Class name:");
const QString S_FILE_NAME_LABEL =
        QStringLiteral("File name:");
const QString S_OBJECT_NAME_LABEL =
        QStringLiteral("Object name:");
const QString S_AUTO_COMPLETE_LABEL =
        QStringLiteral("autocomplete");

const QString S_AUTO_GENERATED_TOOLTIP =
        QStringLiteral("autogenerated using the object name");
const QString S_AUTO_GENERATED_ALT_TOOLTIP =
        QStringLiteral("this value is locked and can not be edited directly");

ClassSpecificationWidget::ClassSpecificationWidget(const FunctionStruct& function,
                                                   ModuleGeneratorSettings* settings,
                                                   bool isLinked,
                                                   QWidget* parent) :
    QWidget(parent),
//    m_implementationStruct(function.implementation),
    m_settings(settings)
{
    m_nameValidator = new QRegularExpressionValidator(ModuleGeneratorSettings::REG_OBJECT_NAME, this);

    m_baseLayout = new QGridLayout();

    m_objectNamelabel = new QLabel(S_OBJECT_NAME_LABEL);
    m_classNameLabel = new QLabel(S_CLASS_NAME_LABEL);
    m_fileNameLabel = new QLabel(S_FILE_NAME_LABEL);

    m_classNameEdit = new QLineEdit();
    m_objectNameEdit = new QLineEdit();
    m_fileNameEdit = new QLineEdit();
    m_autoEditCheckBox = new QCheckBox(S_AUTO_COMPLETE_LABEL);

    m_classNameEdit->setEnabled(false);
    m_fileNameEdit->setEnabled(false);

    m_baseLayout->addWidget(m_objectNamelabel, 0, 0);
    m_baseLayout->addWidget(m_objectNameEdit, 0, 1);
    m_baseLayout->addWidget(m_classNameLabel, 1, 0);
    m_baseLayout->addWidget(m_classNameEdit, 1, 1);
    m_baseLayout->addWidget(m_fileNameLabel, 2, 0);
    m_baseLayout->addWidget(m_fileNameEdit, 2, 1);
    m_baseLayout->addWidget(m_autoEditCheckBox, 3, 0, 1, 2);

    m_objectNameEdit->setValidator(m_nameValidator);

    m_functionSpecificationWidget = Q_NULLPTR;

    auto classStruct(isLinked ? function.linkedClass : function.baseClass);

    if (classStruct.isValid())
    {
        m_implementedClass.functions = classStruct.functions;

        m_functionSpecificationWidget = new FunctionSpecificationWidget(m_implementedClass.functions, settings);

        if (m_functionSpecificationWidget->isEmpty())
        {
           LOG_INSTANCE("empty function widget!", ModuleGeneratorLogger::Type::Warning);
            m_implementedClass.functions = m_functionSpecificationWidget->implementedFunctions();
            delete m_functionSpecificationWidget;
            m_functionSpecificationWidget = Q_NULLPTR;
        }
        else
        {
            QFrame* line = new QFrame();
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);

            m_baseLayout->addWidget(line, 4, 0, 1, 3);
            m_baseLayout->addWidget(m_functionSpecificationWidget, 5, 0, 1, 3);
        }
    }

    auto* spacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    m_baseLayout->addItem(spacer, 6, 0, 1, 3);

    setLayout(m_baseLayout);

    // signals
    connect(m_objectNameEdit, SIGNAL(textChanged(QString)),
            this, SLOT(onEditedObjectName(QString)));
    connect(m_autoEditCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(onAutoCompleteChanged(int)));

    // hack for the package naming problem (issue #153 in Core)
    if (classStruct.className == QStringLiteral("GtPackage"))
    {
        // auto generate the package file and class name
        m_objectNameEdit->setText(QStringLiteral("Package"));
        m_autoEditCheckBox->setChecked(true);
        onAutoCompleteChanged(true);

        // set the correct object name
        m_autoEditCheckBox->setChecked(false);
        m_autoEditCheckBox->setEnabled(false);
        m_objectNameEdit->setText(settings->moduleClass().ident);
        m_objectNameEdit->setEnabled(false);
        m_objectNameEdit->setToolTip(S_AUTO_GENERATED_ALT_TOOLTIP);
        emit hidden();
        return;
    }

    // defaults
    m_autoEditCheckBox->setChecked(true);
    onAutoCompleteChanged(true);
}

ClassStruct
ClassSpecificationWidget::implementedClass()
{
    m_implementedClass.className   = m_classNameEdit->text();
    m_implementedClass.fileName    = m_fileNameEdit->text();
    m_implementedClass.objectName  = m_objectNameEdit->text().simplified();

    if (m_functionSpecificationWidget != Q_NULLPTR)
    {
        m_implementedClass.functions = m_functionSpecificationWidget->implementedFunctions();
    }

    return m_implementedClass;
}

void
ClassSpecificationWidget::autoComplete()
{
    if (!m_autoEditCheckBox->isChecked())
    {
        m_fileNameEdit->setToolTip(S_AUTO_GENERATED_ALT_TOOLTIP);
        m_classNameEdit->setToolTip(S_AUTO_GENERATED_ALT_TOOLTIP);
        return;
    }

    QString objectName = m_objectNameEdit->text();

    m_fileNameEdit->setText(m_settings->fileNamingScheme(objectName));
    m_classNameEdit->setText(m_settings->classNamingScheme(objectName));

    m_fileNameEdit->setToolTip(S_AUTO_GENERATED_TOOLTIP);
    m_classNameEdit->setToolTip(S_AUTO_GENERATED_TOOLTIP);
}

void
ClassSpecificationWidget::closeEvent(QCloseEvent* event)
{
    event->accept();

    emit hidden();
}

void
ClassSpecificationWidget::onEditedObjectName(QString name)
{
    autoComplete();
}

void
ClassSpecificationWidget::onAutoCompleteChanged(int state)
{
    autoComplete();
}
