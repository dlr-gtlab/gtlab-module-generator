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

const QString
DependencySelectionWidget::S_DELETE_BTN_ICON_PATH = QStringLiteral(":/images/cross.png");

const QString
DependencySelectionWidget::S_DELETE_BTN_TOOLTIP = QStringLiteral("delete");
const QString
DependencySelectionWidget::S_CHECKBOX_TOOLTIP = QStringLiteral("use dependency");
const QString
DependencySelectionWidget::S_VERSION_TOOLTIP = QStringLiteral("version of dependency");


DependencySelectionWidget::DependencySelectionWidget(
        DependencyStruct const& dependency, bool isEditable, QWidget* parent) :
    QWidget(parent)
{
    m_versionValidator = new QRegularExpressionValidator{
            ModuleGeneratorSettings::REG_VERSION, this};

    // initializations
    auto* deletePushBtn = new QPushButton;
    auto* baseLayout = new QHBoxLayout;
    m_versionEdit = new QLineEdit;
    m_isActivCheckBox = new QCheckBox;

    deletePushBtn->setIconSize(QSize(16, 16));
    deletePushBtn->setFixedSize(16, 16);

    m_versionEdit->setFixedWidth(120);
    m_versionEdit->setFixedWidth(120);
    m_versionEdit->setText(dependency.version);
    m_versionEdit->setValidator(m_versionValidator);

    m_isActivCheckBox->setText(dependency.name);
    m_isActivCheckBox->setChecked(isEditable);

    deletePushBtn->setIcon(QIcon(S_DELETE_BTN_ICON_PATH));
    deletePushBtn->setToolTip(S_DELETE_BTN_TOOLTIP);

    if (!isEditable)
    {
        deletePushBtn->setEnabled(false);
        m_versionEdit->setEnabled(false);
    }

    baseLayout->setSpacing(0);
    baseLayout->setContentsMargins(5, 2, 5, 2);
    baseLayout->addWidget(m_isActivCheckBox);
    baseLayout->addWidget(m_versionEdit);
    baseLayout->setSpacing(15);
    baseLayout->addWidget(deletePushBtn);

    setLayout(baseLayout);

    // ToolTips
    m_isActivCheckBox->setToolTip(S_CHECKBOX_TOOLTIP);
    m_versionEdit->setToolTip(S_VERSION_TOOLTIP);

    // signals
    connect(deletePushBtn, SIGNAL(pressed()),
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
    QPalette palette;

    if (m_versionValidator->validate(content, pos) == QRegExpValidator::Intermediate)
    {
        palette.setColor(QPalette::Text, Qt::red);
    }

    m_versionEdit->setPalette(palette);
}
