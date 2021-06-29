#include "dependency_selection_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"

#include "widget_list_view.h"
#include "dependency_selection_widget.h"

#include <QWizardPage>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>


const char* C_DEPENDENCY_PAGE_TITLE = "Module Dependencies";

const QString S_INFO_TEXT =
        QStringLiteral("Select or add module dependencies. The list of "
                       "available modules is based on the GTlab environment "
                       "set in the settings page.");
const QString S_DEPENDENCY_BTN_TEXT =
        QStringLiteral("Add");
const QString S_DEPENDENCY_ADD_LABEL =
        QStringLiteral("Add dependency:");
const QString S_DEPENDENCY_LABEL =
        QStringLiteral("Dependencies:");


DependencySelectionPage::DependencySelectionPage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    m_infoTextLabel = new QLabel(S_INFO_TEXT);
    m_addDependencyLabel = new QLabel(S_DEPENDENCY_ADD_LABEL);
    m_dependenciesLabel = new QLabel(S_DEPENDENCY_LABEL);
    m_resolveStatusLabel = new QLabel;
    m_addDependencyEdit = new QLineEdit;
    m_addDependencyPushBtn = new QPushButton;

    m_baseLayout = new QGridLayout(this);

    m_widgetListView = new WidgetListView;

    // page gui
    setTitle(tr(C_DEPENDENCY_PAGE_TITLE));

    m_resolveStatusLabel->setStyleSheet("QLabel { color : red }");
    m_resolveStatusLabel->setVisible(false);
    m_resolveStatusLabel->setWordWrap(true);

    m_infoTextLabel->setWordWrap(true);
    m_infoTextLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    m_infoTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_addDependencyPushBtn->setText(S_DEPENDENCY_BTN_TEXT);
    m_addDependencyPushBtn->setEnabled(false);

    m_dependenciesLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_baseLayout->addWidget(m_infoTextLabel, 0, 0, 1, 2);
    m_baseLayout->addWidget(m_addDependencyLabel, 1, 0);
    m_baseLayout->addWidget(m_addDependencyEdit, 1, 1);
    m_baseLayout->addWidget(m_addDependencyPushBtn, 1, 2);
    m_baseLayout->addWidget(m_dependenciesLabel, 2, 0);
    m_baseLayout->addWidget(m_widgetListView, 2, 1, 1, 2);
    m_baseLayout->addWidget(m_resolveStatusLabel, 3, 1, 1, 2);
    m_baseLayout->setColumnMinimumWidth(0, AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(m_baseLayout);

    // signals
    connect(m_addDependencyEdit, SIGNAL(textChanged(QString)),
            this, SLOT(onEditedDependcyEdit(QString)));

    connect(m_addDependencyPushBtn, SIGNAL(pressed()),
            this, SLOT(onAddDependencyBtnPressed()));
    connect(m_addDependencyPushBtn, SIGNAL(pressed()),
            this, SIGNAL(completeChanged()));
}

void
DependencySelectionPage::initializePage()
{
    LOG_INSTANCE("dependency page...");

    m_addDependencyPushBtn->setDefault(true);

    if (m_widgetListView->widgets().isEmpty())
    {
        addStandardDependencies();
    }

    LOG_INFO << "done!";
}

bool
DependencySelectionPage::isComplete() const
{
    for (auto* widget : m_widgetListView->widgets())
    {
        auto* dependencyWidget = dynamic_cast<DependencySelectionWidget*>(widget);

        if (dependencyWidget == Q_NULLPTR) continue;
        if (!dependencyWidget->isComplete()) return false;
    }

    return QWizardPage::isComplete();
}

bool
DependencySelectionPage::validatePage()
{
    LOG_INSTANCE("validated!");

    auto list = selectedDependencies();

    LOG_INFO << "selected " + QString::number(list.count()) + " dependencies..." << ENDL;

    for (auto dependency : list)
    {
        LOG_INFO << dependency.name << ENDL;
    }

    settings()->setSelectedDependencies(list);

    LOG_INFO << "done!";

    return true;
}

DependencyStructs
DependencySelectionPage::selectedDependencies() const
{
    DependencyStructs list;

    for (auto* widget : m_widgetListView->widgets())
    {
        auto* dependencyWidget = dynamic_cast<DependencySelectionWidget*>(widget);

        if (dependencyWidget == Q_NULLPTR)  continue;

        if (dependencyWidget->isChecked())
        {
            list << dependencyWidget->dependency();
        }
    }

    return list;
}

void
DependencySelectionPage::addStandardDependencies()
{
    LOG_INSTANCE("adding standard dependencies...");
    auto list  = settings()->availableDependencies();
    int status = settings()->dependencyResolveStatus();

    LOG_INFO << "dependeny resolve status: " << QString::number(status) << ENDL;

    if (list.isEmpty())
    {
        QString statusText;

        switch (status)
        {
        case -1:
            statusText = QStringLiteral("Process of retrieving "
                                        "dependencies failed to start!");
            break;
        case 0:
            statusText = QStringLiteral("Process of retrieving dependencies "
                                        "timed out!");
            break;
        case 1:
            statusText = QStringLiteral("Process of retrieving dependencies "
                                        "failed! Could not parse xml output! "
                                        "(The GTlab version may be incompatible "
                                        "with this feature)");
            break;
        case 2:
            statusText = QStringLiteral("Process of retrieving dependencies is "
                                        "still in progress! (Reload this page "
                                        "to update the list)");
            break;
        default:
            LOG_ERR << "invalid dependency resolve status!";
            return;
        }

        m_resolveStatusLabel->setText(statusText);
        m_resolveStatusLabel->setVisible(true);
        return;
    }

    m_resolveStatusLabel->setVisible(false);

    for (DependencyStruct dependency : list)
    {
        addDependency(dependency, false);
    }

    LOG_INFO << "done!";
}

void DependencySelectionPage::addDependency(const DependencyStruct& name,
                                              bool isEditable)
{
    auto widget = new DependencySelectionWidget(name, isEditable);

    connect(widget, SIGNAL(completeChanged()),
            this, SIGNAL(completeChanged()));

    m_widgetListView->insertWidget(-1, widget);

    emit completeChanged();
}

void
DependencySelectionPage::onAddDependencyBtnPressed()
{
    auto name = m_addDependencyEdit->text().simplified();
    m_addDependencyEdit->clear();

    if (name.isEmpty()) return;

    addDependency( { name, "0.0.1" }, true);
}

void
DependencySelectionPage::onEditedDependcyEdit(QString content)
{
    m_addDependencyPushBtn->setEnabled(!content.isEmpty());
}
