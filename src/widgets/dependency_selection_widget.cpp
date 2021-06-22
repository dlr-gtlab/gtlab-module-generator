#include "dependency_selection_widget.h"
#include "module_generator_structs.h"
#include "module_generator_settings.h"

#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QIcon>
#include <QLineEdit>

#include <QRegularExpression>
#include <QRegularExpressionValidator>

const QString S_DELETE_BTN_ICON_PATH = QStringLiteral(":/images/cross.png");

const QString S_DELETE_BTN_TOOLTIP = QStringLiteral("delete");
const QString S_CHECKBOX_TOOLTIP = QStringLiteral("use dependency");
const QString S_SPINBOX_TOOLTIP = QStringLiteral("version of dependency");


DependencySelectionWidget::DependencySelectionWidget(const DependencyStruct& dependency,
                                                     const bool isEditable,
                                                     QWidget* parent) :
    QWidget(parent)
{
    m_versionValidator = new QRegularExpressionValidator(ModuleGeneratorSettings::REG_VERSION, this);

    // initializations
    m_deletePushBtn = new QPushButton;
    m_versionEdit = new QLineEdit;
    m_isActivCheckBox = new QCheckBox;
    m_baseLayout = new QHBoxLayout;

    m_deletePushBtn->setIconSize(QSize(16, 16));
    m_deletePushBtn->setFixedSize(16, 16);

    m_versionEdit->setFixedWidth(120);
    m_versionEdit->setFixedWidth(120);
    m_versionEdit->setText(dependency.version);
    m_versionEdit->setValidator(m_versionValidator);

    m_isActivCheckBox->setText(dependency.name);
    m_isActivCheckBox->setChecked(isEditable);

    m_deletePushBtn->setIcon(QIcon(S_DELETE_BTN_ICON_PATH));
    m_deletePushBtn->setToolTip(S_DELETE_BTN_TOOLTIP);

    if (!isEditable)
    {
        m_deletePushBtn->setEnabled(false);
        m_versionEdit->setEnabled(false);
    }

    m_baseLayout->setSpacing(0);
    m_baseLayout->setContentsMargins(5, 2, 5, 2);
    m_baseLayout->addWidget(m_isActivCheckBox);
    m_baseLayout->addWidget(m_versionEdit);
    m_baseLayout->setSpacing(15);
    m_baseLayout->addWidget(m_deletePushBtn);

    setLayout(m_baseLayout);

    // ToolTips
    m_isActivCheckBox->setToolTip(S_CHECKBOX_TOOLTIP);
    m_versionEdit->setToolTip(S_SPINBOX_TOOLTIP);

    // signals
    connect(m_deletePushBtn, SIGNAL(pressed()),
            this, SLOT(onDeleteBtnPressed()));
    connect(m_versionEdit, SIGNAL(textChanged(QString)),
            this, SLOT(onEditedVersion(QString)));
    connect(m_versionEdit, SIGNAL(textChanged(QString)),
            this,  SIGNAL(completeChanged()));
}

DependencyStruct
DependencySelectionWidget::dependency() const
{
    return { m_isActivCheckBox->text(), m_versionEdit->text() };
}

bool
DependencySelectionWidget::isChecked() const
{
    return m_isActivCheckBox->isChecked();
}

bool
DependencySelectionWidget::isComplete() const
{
    int  pos = 0;
    auto version = m_versionEdit->text();

    return m_versionValidator->validate(version, pos) == QRegExpValidator::Acceptable;
}

void
DependencySelectionWidget::onDeleteBtnPressed()
{
    disconnect(this);

    this->deleteLater();
}

void
DependencySelectionWidget::onEditedVersion(QString content)
{
    int pos = 0;

    if (m_versionValidator->validate(content, pos) == QRegExpValidator::Intermediate)
    {
        m_versionEdit->setStyleSheet("QLineEdit { color : red }");
    }
    else
    {
        m_versionEdit->setStyleSheet("QLineEdit { color : black }");
    }
}
