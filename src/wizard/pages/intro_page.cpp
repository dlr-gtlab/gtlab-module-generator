/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

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
        QStringLiteral("Edit an existing module");

/*
 * constructor
 */
IntroPage::IntroPage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    // initializations
    auto* infoLabel = new QLabel(S_INFO_TEXT);
    auto* baseLayout = new QVBoxLayout;

    auto* gtIcon = new QLabel;
    QImage img{":/images/gt-logo.png"};

    gtIcon->setPixmap(QPixmap::fromImage(img));

    // page gui
    setTitle(tr(C_INTRO_PAGE_TITLE));

    infoLabel->setWordWrap(true);
    infoLabel->setMinimumHeight(
                AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    infoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_createRBtn = new QRadioButton(S_CREATE_BUTTON_TEXT);
//    m_editRBtn = new QRadioButton(S_EDIT_BUTTON_TEXT);

    // layout
    baseLayout->addWidget(infoLabel);
    baseLayout->addWidget(m_createRBtn);
//    baseLayout->addWidget(m_editRBtn);

    auto* spacer = new QSpacerItem{1, 1, QSizePolicy::Minimum,
                                   QSizePolicy::MinimumExpanding};
    baseLayout->addItem(spacer);
    baseLayout->addWidget(gtIcon);


    setLayout(baseLayout);
}

/*
 * protected
 */
void
IntroPage::initializePage()
{
    LOG_INDENT("intro page...");

    m_createRBtn->setChecked(true);
    m_createRBtn->setEnabled(false);
//    m_editRBtn->setDisabled(true);
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

