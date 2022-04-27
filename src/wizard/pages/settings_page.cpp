#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QComboBox>
#include <QVersionNumber>

#include "settings_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"


const int
SettingsPage::I_DIR_PUSHBTN_WIDTH = 40;

const char*
SettingsPage::C_SETTINGS_PAGE_TITLE = "Settings";

const QString
SettingsPage::S_INFO_TEXT = QStringLiteral("Enter the output directory for the "
                                           "generated module. Select the GTlab "
                                           "application and enter the path to "
                                           "your local DevTools installation.");
const QString
SettingsPage::S_OUTPUT_LABEL = QStringLiteral("Output directory:");
const QString
SettingsPage::S_DEVTOOLS_LABEL = QStringLiteral("DevTools directory:");
const QString
SettingsPage::S_GTLAB_LABEL = QStringLiteral("GTlab application:");

const QString
SettingsPage::S_OUTPUT_TOOLTIP = QStringLiteral("All module files will be "
                                                "placed in a separate sub"
                                                "folder.\nThe path is created "
                                                "at runtime if necessary");
const QString
SettingsPage::S_DEVTOOLS_TOOLTIP = QStringLiteral("This directory should "
                                                  "contain:\ninclude/ lib/ "
                                                  "ThirdPartyLibraries/");
const QString
SettingsPage::S_GTLAB_TOOLTIP = QStringLiteral("This must point to GTlab");
const QString
SettingsPage::S_VERSION_LABEL(QStringLiteral("Target version"));
const QString
SettingsPage::S_VERSION_TOOLTIP(QStringLiteral("Target version of GTlab"));

const QString
SettingsPage::S_DIR_BTN_TOOLTIP = QStringLiteral("select directory");
const QString
SettingsPage::S_DIR_BTN_TEXT = QStringLiteral("...");



/*
 * constructor
 */
SettingsPage::SettingsPage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    // initializations
    auto* infoLabel      = new QLabel(S_INFO_TEXT);

    auto* outputLabel     = new QLabel(S_OUTPUT_LABEL);
    auto* outputDirPushBtn   = new QPushButton(S_DIR_BTN_TEXT);

    auto* gtlabLabel         = new QLabel(S_GTLAB_LABEL);
    auto* gtlabDirPushBtn    = new QPushButton(S_DIR_BTN_TEXT);

    auto* devToolsLabel      = new QLabel(S_DEVTOOLS_LABEL);
    auto* devToolsDirPushBtn = new QPushButton(S_DIR_BTN_TEXT);
    auto* baseGridLayout     = new QGridLayout;
    auto* label = new QLabel(S_VERSION_LABEL);

    m_outputDirEdit      = new QLineEdit;
    m_gtlabDirEdit       = new QLineEdit;
    m_devToolsDirEdit    = new QLineEdit;
    m_versionBox = new QComboBox;

    // page gui
    setTitle(tr(C_SETTINGS_PAGE_TITLE));

    infoLabel->setWordWrap(true);
    infoLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    infoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_outputDirEdit->setToolTip(S_OUTPUT_TOOLTIP);
    outputDirPushBtn->setFixedWidth(I_DIR_PUSHBTN_WIDTH);

    m_gtlabDirEdit->setToolTip(S_GTLAB_TOOLTIP);
    gtlabDirPushBtn->setFixedWidth(I_DIR_PUSHBTN_WIDTH);

    m_devToolsDirEdit->setToolTip(S_DEVTOOLS_TOOLTIP);
    devToolsDirPushBtn->setFixedWidth(I_DIR_PUSHBTN_WIDTH);

    m_versionBox->addItems(settings->supportedVersions());
    m_versionBox->setMaximumWidth(100);
    m_versionBox->setToolTip(S_VERSION_TOOLTIP);

    // layout
    baseGridLayout->addWidget(infoLabel, 0, 0, 1, 3);
    baseGridLayout->addWidget(outputLabel, 1, 0);
    baseGridLayout->addWidget(m_outputDirEdit, 1, 1);
    baseGridLayout->addWidget(outputDirPushBtn, 1, 2);
    baseGridLayout->addWidget(gtlabLabel, 2, 0);
    baseGridLayout->addWidget(m_gtlabDirEdit, 2, 1);
    baseGridLayout->addWidget(gtlabDirPushBtn, 2, 2);
    baseGridLayout->addWidget(devToolsLabel, 3, 0);
    baseGridLayout->addWidget(m_devToolsDirEdit, 3, 1);
    baseGridLayout->addWidget(devToolsDirPushBtn, 3, 2);
    baseGridLayout->addWidget(label, 4, 0);
    baseGridLayout->addWidget(m_versionBox, 4, 1);
    baseGridLayout->setColumnMinimumWidth(0, AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(baseGridLayout);

    // signals
    connect(m_outputDirEdit, SIGNAL(textEdited(QString)),
            this, SLOT(onEditedOutputDir(QString)));
    connect(m_outputDirEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(completeChanged()));
    connect(outputDirPushBtn, SIGNAL(clicked(bool)),
            this, SLOT(onPressedOutputDirPushBtn()));

    connect(m_gtlabDirEdit, SIGNAL(textEdited(QString)),
            this, SLOT(onEditedGTlabDir(QString)));
    connect(m_gtlabDirEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(completeChanged()));
    connect(gtlabDirPushBtn, SIGNAL(clicked(bool)),
            this, SLOT(onPressedGTlabDirPushBtn()));


    connect(m_devToolsDirEdit, SIGNAL(textEdited(QString)),
            this, SLOT(onEditedDevToolsDir(QString)));
    connect(m_devToolsDirEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(completeChanged()));
    connect(devToolsDirPushBtn, SIGNAL(clicked(bool)),
            this, SLOT(onPressedDevToolsDirPushBtn()));

    // tool tips
    outputDirPushBtn->setToolTip(S_DIR_BTN_TOOLTIP);
    gtlabDirPushBtn->setToolTip(S_DIR_BTN_TOOLTIP);
    devToolsDirPushBtn->setToolTip(S_DIR_BTN_TOOLTIP);

    // defaults
    QString gtlabPath(settings->gtlabPath());

    if (!gtlabPath.isEmpty())
    {
        gtlabPath += QStringLiteral("/") + ModuleGeneratorSettings::S_GTLAB_APP;
    }

    m_outputDirEdit->setText(QDir::toNativeSeparators(settings->outputPath()));
    m_gtlabDirEdit->setText(QDir::toNativeSeparators(gtlabPath));
    m_devToolsDirEdit->setText(QDir::toNativeSeparators(settings->devToolsPath()));
}

/*
 * proteced
 */
void
SettingsPage::initializePage()
{
    LOG_INDENT("settings page...");
}


bool
SettingsPage::isComplete() const
{
    bool retVal(true);

    setLineEditColor(m_outputDirEdit);
    retVal &= setLineEditColor(m_gtlabDirEdit, true);
    retVal &= setLineEditColor(m_devToolsDirEdit);

    if (m_outputDirEdit->text().isEmpty() ||
        m_gtlabDirEdit->text().isEmpty() ||
        m_devToolsDirEdit->text().isEmpty())
    {
        return false;
    }

    return retVal;
}

bool
SettingsPage::validatePage()
{
    // for nicer debug output
    {
        LOG_INDENT("validated!");

        QFileInfo gtlabApp(m_gtlabDirEdit->text());
        QString path(gtlabApp.path());
        path = QDir::toNativeSeparators(path);

        LOG_INFO << "output path      " << m_outputDirEdit->text() << ENDL;
        LOG_INFO << "path to GTlab    " << path << ENDL;
        LOG_INFO << "path to DevTools " << m_devToolsDirEdit->text() << ENDL;

        settings()->setOutputPath(m_outputDirEdit->text());
        settings()->setGTlabPath(path);
        settings()->setDevToolsPath(m_devToolsDirEdit->text());
        settings()->setGTlabVersion(m_versionBox->currentText());

        LOG_INFO << "target version: " << settings()->gtlabVersion() << ENDL;
        LOG_INFO << "major version:  "
                 << QString::number(settings()->gtlabMajorVersion());
    }

    settings()->preLoad();

    return true;
}

bool
SettingsPage::setLineEditColor(QLineEdit* edit, bool isExec) const
{
    QString path(edit->text());
    QFileInfo fileInfo(path);

    bool isValid(isExec ? fileInfo.isFile() &&
                          fileInfo.fileName() == ModuleGeneratorSettings::S_GTLAB_APP:
                          fileInfo.isDir());

    if (!path.isEmpty() && !isValid)
    {
        edit->setStyleSheet("QLineEdit { color : red }");
        return false;
    }

    edit->setStyleSheet("QLineEdit { color : black }");
    return true;
}


/*
 * private slots
 */
void
SettingsPage::updateDirEdit(QLineEdit* edit, QString& path)
{
    int cursor(edit->cursorPosition());

    edit->setText(QDir::toNativeSeparators(path));

    edit->setCursorPosition(cursor);
}

void
SettingsPage::onEditedOutputDir(QString path)
{
    updateDirEdit(m_outputDirEdit, path);
}

void
SettingsPage::onEditedGTlabDir(QString path)
{
    updateDirEdit(m_gtlabDirEdit, path);
}

void
SettingsPage::onEditedDevToolsDir(QString path)
{
    updateDirEdit(m_devToolsDirEdit, path);
}

void
SettingsPage::onPressedOutputDirPushBtn()
{
    QString dirOutput;

    dirOutput = QFileDialog::getExistingDirectory(
                this, tr("Select target directory"),
                m_outputDirEdit->text(),
                QFileDialog::DontResolveSymlinks);

    if(!dirOutput.isEmpty())
    {
        onEditedOutputDir(dirOutput);
    }
}

void
SettingsPage::onPressedGTlabDirPushBtn()
{
    QString dirGTlab;

    dirGTlab = QFileDialog::getOpenFileName(
                this, tr("Select the GTlab application"),
                m_gtlabDirEdit->text(),
                QStringLiteral("GTlab application (") +
                ModuleGeneratorSettings::S_GTLAB_APP +
                QStringLiteral(")"));

    if (dirGTlab.isEmpty() == false)
    {
        onEditedGTlabDir(dirGTlab);
    }
}

void
SettingsPage::onPressedDevToolsDirPushBtn()
{
    QString dirDevTools;

    dirDevTools = QFileDialog::getExistingDirectory(
                this, tr("Select DevTools directory"),
                m_devToolsDirEdit->text(),
                QFileDialog::DontResolveSymlinks);

    if (dirDevTools.isEmpty() == false)
    {
        onEditedDevToolsDir(dirDevTools);
    }
}


