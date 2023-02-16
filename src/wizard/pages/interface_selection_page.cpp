#include <QWizardPage>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

#include "interface_selection_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"

#include "interface_selection_widget.h"
#include "widget_list_view.h"


const char*
InterfaceSelectionPage::C_INTERFACE_PAGE_TITLE = "Interface Selection";

const QString
InterfaceSelectionPage::S_INFO_TEXT =
        QStringLiteral("Select the desired interfaces to extend.");
const QString
InterfaceSelectionPage::S_INTERFACE_LABEL = QStringLiteral("Interfaces:");


InterfaceSelectionPage::InterfaceSelectionPage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    // initializations
    auto* infoTextLabel = new QLabel(S_INFO_TEXT);
    auto* interfaceLabel = new QLabel(S_INTERFACE_LABEL);
    auto* gridLayout = new QGridLayout;

    m_widgetListView = new WidgetListView;

    // page gui
    setTitle(tr(C_INTERFACE_PAGE_TITLE));

    infoTextLabel->setWordWrap(true);
    infoTextLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    infoTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    interfaceLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    int row = 0;
    gridLayout->addWidget(infoTextLabel, row++, 0, 1, 2);
    gridLayout->addWidget(interfaceLabel, row, 0);
    gridLayout->addWidget(m_widgetListView, row++, 1);
    gridLayout->setColumnMinimumWidth(0,
                                      AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(gridLayout);
}

void
InterfaceSelectionPage::initializePage()
{
    LOG_INDENT("interface page...");

    if (settings()->selectedInterfaces().empty())
    {
        initInterfaces();
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
    LOG_INDENT("validated!");

    auto list = selectedInterfaces();

    LOG_INFO << "selected " << QString::number(list.count()) << " interfaces..." << ENDL;

    for (auto const& item : qAsConst(list))
    {
        LOG_INFO << item.className << ENDL;
    }

    settings()->setSelectedInterfaces(std::move(list));

    LOG_INFO << "done!";

    return true;
}

void
InterfaceSelectionPage::initInterfaces()
{
    m_widgetListView->clear();

    auto interfaces = settings()->availableInterfaces();

    for (auto const& interfaceStruct : interfaces)
    {
        if (interfaceStruct.version.isEnabled(settings()->gtlabVersion()))
        {
            auto widget = new InterfaceSelectionWidget(interfaceStruct);
            m_widgetListView->appendWidget(widget);
        }
    }
}

ClassDataList
InterfaceSelectionPage::selectedInterfaces() const
{
    ClassDataList interfaces;

    for (auto* widget : m_widgetListView->widgets())
    {
        auto* interfaceWidget = qobject_cast<InterfaceSelectionWidget*>(widget);

        if (!interfaceWidget) continue;

        if (interfaceWidget->isChecked())
        {
            interfaces << interfaceWidget->interface();
        }
    }

    return interfaces;
}

