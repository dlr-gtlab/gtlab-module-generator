#include <QWizardPage>
#include <QLabel>
#include <QGridLayout>

#include "interface_selection_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"

#include "interface_selection_widget.h"
#include "widget_list_view.h"


const char* C_INTERFACE_PAGE_TITLE = "Interface Selection";

const QString S_INFO_TEXT =
        QStringLiteral("Select the desired interfaces to extend.");
const QString S_INTERFACE_LABEL =
        QStringLiteral("Interfaces:");


InterfaceSelectionPage::InterfaceSelectionPage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    // initializations
    m_infoTextLabel = new QLabel(S_INFO_TEXT);
    m_interfaceLabel = new QLabel(S_INTERFACE_LABEL);
    m_gridLayout = new QGridLayout;

    m_widgetListView = new WidgetListView;

    // page gui
    setTitle(tr(C_INTERFACE_PAGE_TITLE));

    m_infoTextLabel->setWordWrap(true);
    m_infoTextLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    m_infoTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_interfaceLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_gridLayout->addWidget(m_infoTextLabel, 0, 0, 1, 2);
    m_gridLayout->addWidget(m_interfaceLabel, 1, 0);
    m_gridLayout->addWidget(m_widgetListView, 1, 1);
    m_gridLayout->setColumnMinimumWidth(0, AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(m_gridLayout);
}

void
InterfaceSelectionPage::initializePage()
{
    LOG_INSTANCE("interface page...");

    if (m_widgetListView->widgets().isEmpty())
    {
        setInterfaces();
    }
}

bool
InterfaceSelectionPage::isComplete() const
{
    return QWizardPage::isComplete();
}

bool
InterfaceSelectionPage::validatePage()
{
    LOG_INSTANCE("validated!");

    auto list = selectedInterfaces();

    LOG_INFO << "selected " << QString::number(list.count()) << " interfaces..." << ENDL;

    for (auto* item : list)
    {
        LOG_INFO << item->className << ENDL;
    }

    settings()->setSelectedInterfaces(list);

    LOG_INFO << "done!";

    return true;
}

void
InterfaceSelectionPage::setInterfaces()
{
    auto interfaceStructs = settings()->availableInterfaces();

    for (auto* interfaceStruct : interfaceStructs)
    {
        auto widget = new InterfaceSelectionWidget(*interfaceStruct);

        m_widgetListView->insertWidget(-1, widget);
    }
}

ClassStructsPtr
InterfaceSelectionPage::selectedInterfaces() const
{
    ClassStructsPtr list;

    for (auto* widget : m_widgetListView->widgets())
    {
        auto* interfaceWidget = dynamic_cast<InterfaceSelectionWidget*>(widget);

        if (interfaceWidget == Q_NULLPTR) continue;

        if (interfaceWidget->isChecked())
        {
            auto* interface = &interfaceWidget->interfaceStruct();

            list << interface;
        }
    }

    return list;
}

