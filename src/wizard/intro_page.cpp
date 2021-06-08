#include "intro_page.h"

#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>

#include "module_generator_settings.h"
#include "module_generator_logger.h"

const char* C_INTRO_PAGE_TITLE = "Introduction";

const QString S_INFO_TEXT =
        QStringLiteral("This tool generates a basic module for GTlab (v1.7)");
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
    m_infoTextLabel = new QLabel(S_INFO_TEXT);
    m_createRBtn = new QRadioButton(S_CREATE_BUTTON_TEXT);
    m_editRBtn = new QRadioButton(S_EDIT_BUTTON_TEXT);
    m_baseLayout = new QVBoxLayout;


    // page gui
    setTitle(tr(C_INTRO_PAGE_TITLE));

    m_infoTextLabel->setWordWrap(true);
    m_infoTextLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    m_infoTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_createRBtn->setChecked(true);

    m_editRBtn->setDisabled(true);

    // layout
    m_baseLayout->addWidget(m_infoTextLabel);
    m_baseLayout->addWidget(m_createRBtn);
    m_baseLayout->addWidget(m_editRBtn);

    setLayout(m_baseLayout);
}

/*
 * protected
 */
void
IntroPage::initializePage()
{
    LOG_INSTANCE("intro page...");
}

bool
IntroPage::validatePage()
{
    LOG_INSTANCE("validated!");

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

