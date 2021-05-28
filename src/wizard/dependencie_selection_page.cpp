#include "dependencie_selection_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"

#include "widget_list_view.h"
#include "dependencie_selection_widget.h"

#include <QWizardPage>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>


const char* C_DEPENDENCIE_PAGE_TITLE = "Module Dependecies";

const QString S_INFO_TEXT =
        QStringLiteral("Select or add any module dependencies.");
const QString S_DEPENDENCIE_BTN_TEXT =
        QStringLiteral("Add");
const QString S_DEPENDENCIE_ADD_LABEL =
        QStringLiteral("Add Dependencie:");
const QString S_DEPENDENCIE_LABEL =
        QStringLiteral("Dependencies:");


DependencieSelectionPage::DependencieSelectionPage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    m_infoTextLabel = new QLabel(S_INFO_TEXT);
    m_addDependencieLabel = new QLabel(S_DEPENDENCIE_ADD_LABEL);
    m_dependenciesLabel = new QLabel(S_DEPENDENCIE_LABEL);
    m_addDependencieEdit = new QLineEdit;
    m_addDependenciePushBtn = new QPushButton;

    m_baseLayout = new QGridLayout(this);

    m_widgetListView = new WidgetListView;

    // page gui
    setTitle(tr(C_DEPENDENCIE_PAGE_TITLE));

    m_infoTextLabel->setWordWrap(true);
    m_infoTextLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    m_infoTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_addDependenciePushBtn->setText(S_DEPENDENCIE_BTN_TEXT);
    m_addDependenciePushBtn->setEnabled(false);

    m_dependenciesLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_baseLayout->addWidget(m_infoTextLabel, 0, 0, 1, 2);
    m_baseLayout->addWidget(m_addDependencieLabel, 1, 0);
    m_baseLayout->addWidget(m_addDependencieEdit, 1, 1);
    m_baseLayout->addWidget(m_addDependenciePushBtn, 1, 2);
    m_baseLayout->addWidget(m_dependenciesLabel, 2, 0);
    m_baseLayout->addWidget(m_widgetListView, 2, 1, 1, 2);
    m_baseLayout->setColumnMinimumWidth(0, AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(m_baseLayout);

    // signals
    connect(m_addDependencieEdit, SIGNAL(textChanged(QString)),
            this, SLOT(onEditedDependcieEdit(QString)));

    connect(m_addDependenciePushBtn, SIGNAL(pressed()),
            this, SLOT(onAddDependencieBtnPressed()));
    connect(m_addDependenciePushBtn, SIGNAL(pressed()),
            this, SIGNAL(completeChanged()));
}

void
DependencieSelectionPage::initializePage()
{
    LOG_INSTANCE("dependencie page...");

    m_addDependenciePushBtn->setDefault(true);    

    if (m_widgetListView->widgets().isEmpty())
    {
        addStandardDependencies();
    }

    LOG_INFO << "done!";
}

bool
DependencieSelectionPage::isComplete() const
{
    for (auto* widget : m_widgetListView->widgets())
    {
        auto* dependencieWidget = dynamic_cast<DependencieSelectionWidget*>(widget);

        if (dependencieWidget == Q_NULLPTR) continue;
        if (!dependencieWidget->isComplete()) return false;
    }

    return QWizardPage::isComplete();
}

bool
DependencieSelectionPage::validatePage()
{
    LOG_INSTANCE("validated!");

    auto list = selectedDependencies();

    {
        LOG_INSTANCE("selected " + QString::number(list.count()) + " dependencies...");

        for (auto dependencie : list)
        {
            LOG_INFO << dependencie.name;
        }
    }

    settings()->setSelectedDependencies(list);

    return true;
}

DependencieStructs
DependencieSelectionPage::selectedDependencies() const
{
    DependencieStructs list;

    for (auto* widget : m_widgetListView->widgets())
    {
        auto* dependencieWidget = dynamic_cast<DependencieSelectionWidget*>(widget);

        if (dependencieWidget == Q_NULLPTR)  continue;

        if (dependencieWidget->isChecked())
        {
            list << dependencieWidget->dependencie();
        }
    }

    return list;
}

void
DependencieSelectionPage::addStandardDependencies()
{
    auto list = settings()->availableDependencies();

    for (DependencieStruct dependencie : list)
    {
        addDependencie(dependencie, false);
    }
}

void DependencieSelectionPage::addDependencie(const DependencieStruct& name,
                                              bool isEditable)
{
    auto widget = new DependencieSelectionWidget(name, isEditable);

    connect(widget, SIGNAL(completeChanged()),
            this, SIGNAL(completeChanged()));

    m_widgetListView->insertWidget(-1, widget);

    emit completeChanged();
}

void
DependencieSelectionPage::onAddDependencieBtnPressed()
{
    auto name = m_addDependencieEdit->text();

    if (name.isEmpty()) return;

    addDependencie( { name, "0.0.1" }, true);
}

void
DependencieSelectionPage::onEditedDependcieEdit(QString content)
{
    m_addDependenciePushBtn->setEnabled(!content.isEmpty());
}
