#include "intro_page.h"

#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "module_generator_settings.h"
#include "module_generator_logger.h"

const char*
IntroPage::C_INTRO_PAGE_TITLE = "Introduction";

const QString
IntroPage::S_INFO_TEXT{QStringLiteral("This tool generates a basic module for "
                                      "GTlab.")};

const QString S_CREATE_BUTTON_TEXT =
        QStringLiteral("Create a new module");
const QString S_EDIT_BUTTON_TEXT =
        QStringLiteral("Edit an existing module (WIP)");

/*
 * constructor
 */
IntroPage::IntroPage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    // initializations
    auto* infoLabel = new QLabel(S_INFO_TEXT);
    auto* baseLayout = new QVBoxLayout;


    // page gui
    setTitle(tr(C_INTRO_PAGE_TITLE));

    infoLabel->setWordWrap(true);
    infoLabel->setMinimumHeight(
                AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    infoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_createRBtn = new QRadioButton(S_CREATE_BUTTON_TEXT);
    m_editRBtn = new QRadioButton(S_EDIT_BUTTON_TEXT);
    m_createRBtn->setChecked(true);
    m_editRBtn->setDisabled(true);

    // layout
    baseLayout->addWidget(infoLabel);
    baseLayout->addWidget(m_createRBtn);
    baseLayout->addWidget(m_editRBtn);

    setLayout(baseLayout);
}

/*
 * protected
 */
void
IntroPage::initializePage()
{
    LOG_INDENT("intro page...");
}

bool
IntroPage::validatePage()
{
    LOG_INDENT("validated!");

    if(m_createRBtn->isChecked())
    {
        LOG_INFO << "CREATE";

        settings()->setWizardMode(ModuleGeneratorSettings::Create);
    }
    else
    {
        LOG_INFO << "EDIT";

        settings()->setWizardMode(ModuleGeneratorSettings::Edit);
    }

    return true;
}

