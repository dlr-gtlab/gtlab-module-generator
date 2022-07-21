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


const char*
DependencySelectionPage::C_DEPENDENCY_PAGE_TITLE = "Module Dependencies";

const QString
DependencySelectionPage::S_INFO_TEXT =
        QStringLiteral("Select or add module dependencies. The list of "
                       "available modules is based on the GTlab environment "
                       "set in the settings page.");
const QString
DependencySelectionPage::S_DEPENDENCY_BTN_TEXT = QStringLiteral("Add");
const QString
DependencySelectionPage::S_DEPENDENCY_ADD_LABEL = QStringLiteral("Add dependency:");
const QString
DependencySelectionPage::S_DEPENDENCY_LABEL = QStringLiteral("Dependencies:");

DependencySelectionPage::DependencySelectionPage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    auto* infoLabel = new QLabel(S_INFO_TEXT);
    auto* addDependencyLabel = new QLabel(S_DEPENDENCY_ADD_LABEL);
    auto* dependenciesLabel = new QLabel(S_DEPENDENCY_LABEL);
    m_resolveStatusLabel = new QLabel;
    m_addDependencyEdit = new QLineEdit;
    m_addDependencyPushBtn = new QPushButton;

    auto* baseLayout = new QGridLayout(this);

    m_widgetListView = new WidgetListView;

    // page gui
    setTitle(tr(C_DEPENDENCY_PAGE_TITLE));

    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::red);

    m_resolveStatusLabel->setPalette(palette);
    m_resolveStatusLabel->setVisible(false);
    m_resolveStatusLabel->setWordWrap(true);

    infoLabel->setWordWrap(true);
    infoLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    infoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_addDependencyPushBtn->setText(S_DEPENDENCY_BTN_TEXT);
    m_addDependencyPushBtn->setEnabled(false);

    dependenciesLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    int row = 0;
    baseLayout->addWidget(infoLabel, row++, 0, 1, 2);
    baseLayout->addWidget(addDependencyLabel, row, 0);
    baseLayout->addWidget(m_addDependencyEdit, row, 1);
    baseLayout->addWidget(m_addDependencyPushBtn, row++, 2);
    baseLayout->addWidget(dependenciesLabel, row, 0);
    baseLayout->addWidget(m_widgetListView, row++, 1, 1, 2);
    baseLayout->addWidget(m_resolveStatusLabel, row++, 1, 1, 2);
    baseLayout->setColumnMinimumWidth(0, AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(baseLayout);

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
    LOG_INDENT("dependency page...");

    m_addDependencyPushBtn->setDefault(true);

    if (m_widgetListView->widgets().isEmpty() ||
        !m_resolveStatusLabel->isHidden())
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
    LOG_INDENT("validated!");

    auto list = selectedDependencies();

    LOG_INFO << "selected " + QString::number(list.count()) + " dependencies..." << ENDL;

    for (auto const& dependency : list)
    {
        LOG_INFO << dependency.name << ENDL;
    }

    settings()->setSelectedDependencies(list);

    LOG_INFO << "done!";

    return true;
}

DependencyDataList
DependencySelectionPage::selectedDependencies() const
{
    DependencyDataList list;

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
    LOG_INDENT("adding standard dependencies...");
    auto list  = settings()->availableDependencies();
    int status = settings()->dependencyResolveStatus();

    LOG_INFO << "dependeny resolve status: " << QString::number(status) << ENDL;

    if (list.isEmpty())
    {
        QString statusText;

        switch (status)
        {
        case -1:
            statusText = QStringLiteral("The process for fetching the existing "
                                        "dependencies failed to start!");
            break;
        case 0:
            statusText = QStringLiteral("The process for fetching the existing "
                                        "dependencies timed out!");
            break;
        case 1:
            statusText = QStringLiteral("The process for fetching the existing "
                                        "dependencies failed! Could not parse "
                                        "the XML output. The GTlab version may "
                                        "be incompatible with this feature.");
            break;
        case 2:
            statusText = QStringLiteral("The process for fetching the existing "
                                        "dependencies is still in progress! "
                                        "Reload this page to update the list.");
            break;
        default:
            statusText = QStringLiteral("The process for fetching the existing "
                                        "dependencies failed!");
            LOG_ERR << "invalid dependency resolve status!";
        }

        m_resolveStatusLabel->setText(statusText);
        m_resolveStatusLabel->setVisible(true);
        return;
    }

    m_resolveStatusLabel->setVisible(false);

    for (auto const&  dependency : list)
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

    m_widgetListView->appendWidget(widget);

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
