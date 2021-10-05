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

#include "settings_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"


const int I_DIR_PUSHBTN_WIDTH = 40;

const char* C_SETTINGS_PAGE_TITLE = "Settings";

const QString S_INFO_TEXT =
        QStringLiteral("Enter the output directory for the generated module. "
                       "Select the GTlab application and enter "
                       "the path to your local DevTools installation.");
const QString S_OUTPUT_LABEL =
        QStringLiteral("Output directory:");
const QString S_DEVTOOLS_LABEL =
        QStringLiteral("DevTools directory:");
const QString S_GTLAB_LABEL =
        QStringLiteral("GTlab application:");

const QString S_OUTPUT_TOOLTIP =
        QStringLiteral("All module files will be placed in a separate subfolder.\n"
                       "The path is created at runtime if necessary");
const QString S_DEVTOOLS_TOOLTIP =
        QStringLiteral("This directory should contain:\n"
                       "include/ lib/ ThirdPartyLibraries/");
const QString S_GTLAB_TOOLTIP =
        QStringLiteral("This must point to GTlab");

const QString S_DIR_BTN_TOOLTIP =
        QStringLiteral("select directory");
const QString S_DIR_BTN_TEXT =
        QStringLiteral("...");



/*
 * constructor
 */
SettingsPage::SettingsPage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    // initializations
    m_infoTextLabel      = new QLabel(S_INFO_TEXT);

    m_outputDirLabel     = new QLabel(S_OUTPUT_LABEL);
    m_outputDirEdit      = new QLineEdit;
    m_outputDirPushBtn   = new QPushButton(S_DIR_BTN_TEXT);

    m_gtlabLabel         = new QLabel(S_GTLAB_LABEL);
    m_gtlabDirEdit       = new QLineEdit;
    m_gtlabDirPushBtn    = new QPushButton(S_DIR_BTN_TEXT);

    m_devToolsLabel      = new QLabel(S_DEVTOOLS_LABEL);
    m_devToolsDirEdit    = new QLineEdit;
    m_devToolsDirPushBtn = new QPushButton(S_DIR_BTN_TEXT);
    m_baseGridLayout     = new QGridLayout;

    // page gui
    setTitle(tr(C_SETTINGS_PAGE_TITLE));

    m_infoTextLabel->setWordWrap(true);
    m_infoTextLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    m_infoTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);    

    m_outputDirEdit->setToolTip(S_OUTPUT_TOOLTIP);
    m_outputDirPushBtn->setFixedWidth(I_DIR_PUSHBTN_WIDTH);

    m_gtlabDirEdit->setToolTip(S_GTLAB_TOOLTIP);
    m_gtlabDirPushBtn->setFixedWidth(I_DIR_PUSHBTN_WIDTH);

    m_devToolsDirEdit->setToolTip(S_DEVTOOLS_TOOLTIP);
    m_devToolsDirPushBtn->setFixedWidth(I_DIR_PUSHBTN_WIDTH);



    // layout
    m_baseGridLayout->addWidget(m_infoTextLabel, 0, 0, 1, 3);
    m_baseGridLayout->addWidget(m_outputDirLabel, 1, 0);
    m_baseGridLayout->addWidget(m_outputDirEdit, 1, 1);
    m_baseGridLayout->addWidget(m_outputDirPushBtn, 1, 2);
    m_baseGridLayout->addWidget(m_gtlabLabel, 2, 0);
    m_baseGridLayout->addWidget(m_gtlabDirEdit, 2, 1);
    m_baseGridLayout->addWidget(m_gtlabDirPushBtn, 2, 2);
    m_baseGridLayout->addWidget(m_devToolsLabel, 3, 0);
    m_baseGridLayout->addWidget(m_devToolsDirEdit, 3, 1);
    m_baseGridLayout->addWidget(m_devToolsDirPushBtn, 3, 2);
    m_baseGridLayout->setColumnMinimumWidth(0, AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(m_baseGridLayout);


    // signals
    connect(m_outputDirEdit, SIGNAL(textEdited(QString)),
            this, SLOT(onEditedOutputDir(QString)));
    connect(m_outputDirEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(completeChanged()));
    connect(m_outputDirPushBtn, SIGNAL(clicked(bool)),
            this, SLOT(onPressedOutputDirPushBtn()));

    connect(m_gtlabDirEdit, SIGNAL(textEdited(QString)),
            this, SLOT(onEditedGTlabDir(QString)));
    connect(m_gtlabDirEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(completeChanged()));
    connect(m_gtlabDirPushBtn, SIGNAL(clicked(bool)),
            this, SLOT(onPressedGTlabDirPushBtn()));


    connect(m_devToolsDirEdit, SIGNAL(textEdited(QString)),
            this, SLOT(onEditedDevToolsDir(QString)));
    connect(m_devToolsDirEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(completeChanged()));
    connect(m_devToolsDirPushBtn, SIGNAL(clicked(bool)),
            this, SLOT(onPressedDevToolsDirPushBtn()));

    // tool tips
    m_outputDirPushBtn->setToolTip(S_DIR_BTN_TOOLTIP);
    m_gtlabDirPushBtn->setToolTip(S_DIR_BTN_TOOLTIP);
    m_devToolsDirPushBtn->setToolTip(S_DIR_BTN_TOOLTIP);

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
    LOG_INSTANCE("settings page...");
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
        LOG_INSTANCE("validated!");

        QFileInfo gtlabApp(m_gtlabDirEdit->text());
        QString path(gtlabApp.path());
        path = QDir::toNativeSeparators(path);

        LOG_INFO << "output path      " << m_outputDirEdit->text() << ENDL;
        LOG_INFO << "path to GTlab    " << path << ENDL;
        LOG_INFO << "path to DevTools " << m_devToolsDirEdit->text() << ENDL;

        settings()->setOutputPath(m_outputDirEdit->text());
        settings()->setGTlabPath(path);
        settings()->setDevToolsPath(m_devToolsDirEdit->text());
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
updateDirEdit(QLineEdit* edit, QString& path)
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
                this, tr("Select target directory"), m_outputDirEdit->text(),
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
                this, tr("Select the GTlab application"), m_gtlabDirEdit->text(),
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
                this, tr("Select DevTools directory"), m_devToolsDirEdit->text(),
                QFileDialog::DontResolveSymlinks);

    if (dirDevTools.isEmpty() == false)
    {
        onEditedDevToolsDir(dirDevTools);
    }
}


