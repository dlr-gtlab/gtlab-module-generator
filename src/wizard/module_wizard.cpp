#include "module_wizard.h"

#include "module_generator.h"
#include "module_generator_logger.h"

#include "intro_page.h"
#include "settings_page.h"
#include "module_specifications_page.h"
#include "summary_page.h"
#include "interface_selection_page.h"
#include "interface_specifications_page.h"
#include "dependency_selection_page.h"
#include "signature_page.h"




const QString S_MODULE_GENERATOR_NAME = QStringLiteral("Module Generator");

const QSize SIZE_WIZARD = QSize(570, 475);

ModuleWizard::ModuleWizard(QWidget* parent) : QWizard(parent)
{
    // generator
    m_generator = new ModuleGenerator(&m_settings);

    // pages
    m_introPage = new IntroPage(&m_settings, this);
    m_settingsPage = new SettingsPage(&m_settings, this);
    m_moduleSpecificationPage = new ModuleSpecificationsPage(&m_settings, this);
    m_interfaceSelectionPage = new InterfaceSelectionPage(&m_settings, this);
    m_interfaceSpecificationsPage = new InterfaceSpecificationsPage(&m_settings, this);
    m_dependencySelectionPage = new DependencySelectionPage(&m_settings, this);
    m_signaturePage = new SignaturePage(&m_settings, this);
    m_summaryPage = new SummaryPage(&m_settings, this);

    // add pages
    setPage(PAGE::INTRO_PAGE, m_introPage);
    setPage(PAGE::SETTINGS_PAGE, m_settingsPage);
    setPage(PAGE::SPECIFICATIONS_PAGE, m_moduleSpecificationPage);
    setPage(PAGE::INTERFACE_SELECTION_PAGE, m_interfaceSelectionPage);
    setPage(PAGE::INTERFACE_SPECIFICATION_PAGE, m_interfaceSpecificationsPage);
    setPage(PAGE::DEPENDENCY_SELECTION_PAGE, m_dependencySelectionPage);
    setPage(PAGE::SIGNATURE_PAGE, m_signaturePage);
    setPage(PAGE::SUMMARY_PAGE, m_summaryPage);

    connect(m_summaryPage, SIGNAL(validated()),
            m_generator, SLOT(onGenerate()));
    connect(m_generator, SIGNAL(generationFinished()),
            m_summaryPage, SLOT(onGenerationFinished()));

    // wizard gui
    setWindowTitle(S_MODULE_GENERATOR_NAME + " (v. " + ModuleGeneratorSettings::S_VERSION + ")");
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    this->resize(SIZE_WIZARD);
}

ModuleWizard::~ModuleWizard()
{
    delete m_generator;
}

int
ModuleWizard::nextId() const
{
    switch (currentId())
    {
    case ModuleWizard::INTERFACE_SELECTION_PAGE:
        if (m_settings.selectedInterfaces().size() > 0)
        {
            return QWizard::nextId();
        }
        return ModuleWizard::DEPENDENCY_SELECTION_PAGE;
    case ModuleWizard::INTERFACE_SPECIFICATION_PAGE:
        return QWizard::nextId();
    default:
        return QWizard::nextId();
    }
}
