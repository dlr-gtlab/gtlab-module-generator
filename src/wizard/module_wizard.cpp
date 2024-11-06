/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

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

const QString
ModuleWizard::S_MODULE_GENERATOR_NAME = QStringLiteral("Module Generator");

const QSize
ModuleWizard::S_SIZE_WIZARD = QSize(585, 475);

ModuleWizard::ModuleWizard(QWidget* parent) : QWizard(parent),
    m_generator{std::make_unique<ModuleGenerator>()}
{
    // pages
    auto* m_introPage = new IntroPage(
                m_generator->settings(), this);
    auto* m_settingsPage = new SettingsPage(
                m_generator->settings(), this);
    auto* m_moduleSpecificationPage = new ModuleSpecificationsPage(
                m_generator->settings(), this);
    auto* m_interfaceSelectionPage = new InterfaceSelectionPage(
                m_generator->settings(), this);
    auto* m_interfaceSpecificationsPage = new InterfaceSpecificationsPage(
                m_generator->settings(), this);
    auto* m_dependencySelectionPage = new DependencySelectionPage(
                m_generator->settings(), this);
    auto* m_signaturePage = new SignaturePage(
                m_generator->settings(), this);
    auto* m_summaryPage = new SummaryPage(
                m_generator->settings(), this);

    // add pages
    setPage(PAGE::INTRO_PAGE, m_introPage);
    setPage(PAGE::SETTINGS_PAGE, m_settingsPage);
    setPage(PAGE::SPECIFICATIONS_PAGE, m_moduleSpecificationPage);
    setPage(PAGE::INTERFACE_SELECTION_PAGE, m_interfaceSelectionPage);
    setPage(PAGE::INTERFACE_SPECIFICATION_PAGE, m_interfaceSpecificationsPage);
    setPage(PAGE::DEPENDENCY_SELECTION_PAGE, m_dependencySelectionPage);
    setPage(PAGE::SIGNATURE_PAGE, m_signaturePage);
    setPage(PAGE::SUMMARY_PAGE, m_summaryPage);

    // cnnect generate signals
    connect(m_summaryPage, SIGNAL(validated()),
            m_generator.get(), SLOT(generate()));
    connect(m_generator.get(), SIGNAL(generationFinished()),
            m_summaryPage, SLOT(onGenerationFinished()));

    // connect helper signals
    connect(m_moduleSpecificationPage, SIGNAL(moduleDataChanged()),
            m_interfaceSpecificationsPage, SLOT(clearInterfaceTabs()));

    // wizard gui
    setWindowTitle(S_MODULE_GENERATOR_NAME + " (v. " +
                   ModuleGeneratorSettings::S_VERSION + ")");
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    this->resize(S_SIZE_WIZARD);
}

int
ModuleWizard::nextId() const
{
    switch (currentId())
    {
    case ModuleWizard::INTERFACE_SELECTION_PAGE:
        if (!m_generator->settings()->selectedInterfaces().isEmpty())
        {
            return QWizard::nextId();
        }
        return ModuleWizard::DEPENDENCY_SELECTION_PAGE;
    }
    return QWizard::nextId();
}
