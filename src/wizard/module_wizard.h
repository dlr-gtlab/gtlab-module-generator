#ifndef MODULEWIZARD_H
#define MODULEWIZARD_H

#include <QWizard>

#include "module_generator_settings.h"

class ModuleGenerator;
class IntroPage;
class SettingsPage;
class ModuleSpecificationsPage;
class InterfaceSelectionPage;
class InterfaceSpecificationsPage;
class DependencieSelectionPage;
class SignaturePage;
class SummaryPage;
class ModuleWizard : public QWizard
{
    Q_OBJECT

public:

    explicit ModuleWizard(QWidget* parent = nullptr);
    ~ModuleWizard() override;

    /**
     * @brief page enum - enumerates wizard pages
     */
    enum PAGE {INTRO_PAGE = 0,
               SETTINGS_PAGE = 1,
               SPECIFICATIONS_PAGE = 2,
               INTERFACE_SELECTION_PAGE = 3,
               INTERFACE_SPECIFICATION_PAGE = 5,
               DEPENDENCIE_SELECTION_PAGE = 6,
               SIGNATURE_PAGE = 7,
               SUMMARY_PAGE = 8};

    int nextId() const Q_DECL_OVERRIDE;

private:

    /// pointer for intro page
    IntroPage* m_introPage;
    SettingsPage* m_settingsPage;
    ModuleSpecificationsPage* m_moduleSpecificationPage;
    InterfaceSelectionPage* m_interfaceSelectionPage;
    InterfaceSpecificationsPage* m_interfaceSpecificationsPage;
    DependencieSelectionPage* m_dependencieSelectionPage;
    SignaturePage* m_signaturePage;
    SummaryPage* m_summaryPage;

    ModuleGeneratorSettings m_settings;
    ModuleGenerator* m_generator;
};

#endif // MODULEWIZARD_H
