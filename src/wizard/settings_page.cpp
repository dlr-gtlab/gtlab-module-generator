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
#include <QCompleter>
#include <QFileSystemModel>

#include "settings_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"


const int I_DIR_PUSHBTN_WIDTH = 40;

const char* C_SETTINGS_PAGE_TITLE = "Settings";

const QString S_INFO_TEXT =
        QStringLiteral("Enter the output directory, in which the module should "
                       "be generated. Select the path to the parent "
                       "directory of your local GTlab and "
                       "Dev-Tools installation. ");
const QString S_OUTPUT_LABEL =
        QStringLiteral("Output Directory:");
const QString S_DEVTOOLS_LABEL =
        QStringLiteral("Dev-Tools Directory:");
const QString S_GTLAB_LABEL =
        QStringLiteral("GTlab Directory:");

const QString S_OUTPUT_TOOLTIP =
        QStringLiteral("All module files will be placed in a separate subfolder.\n"
                       "The path is created at runtime if necessary");
const QString S_DEVTOOLS_TOOLTIP =
        QStringLiteral("This directory should contain the 'include', 'lib' \n"
                       "and 'ThirdPartyLibraries' subfolders");
const QString S_GTLAB_TOOLTIP =
        QStringLiteral("This directory should contain the 'bin' folder,\n"
                       "which in turn should contain the 'GTlab.exe'");

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
    m_fileSystemCompleter = new QCompleter(this);
    m_fileSystemModel     = new QFileSystemModel(this);

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


    // file system setup
    m_fileSystemModel->setRootPath("/");

    m_fileSystemCompleter->setWrapAround(false);
    m_fileSystemCompleter->setModel(m_fileSystemModel);

    // page gui
    setTitle(tr(C_SETTINGS_PAGE_TITLE));

    m_infoTextLabel->setWordWrap(true);
    m_infoTextLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    m_infoTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);    

    m_outputDirEdit->setToolTip(S_OUTPUT_TOOLTIP);
    m_outputDirEdit->setCompleter(m_fileSystemCompleter);
    m_outputDirPushBtn->setFixedWidth(I_DIR_PUSHBTN_WIDTH);

    m_gtlabDirEdit->setCompleter(m_fileSystemCompleter);
    m_gtlabDirEdit->setToolTip(S_GTLAB_TOOLTIP);
    m_gtlabDirPushBtn->setFixedWidth(I_DIR_PUSHBTN_WIDTH);

    m_devToolsDirEdit->setCompleter(m_fileSystemCompleter);
    m_devToolsDirEdit->setToolTip(S_DEVTOOLS_TOOLTIP);
    m_devToolsDirPushBtn->setFixedWidth(I_DIR_PUSHBTN_WIDTH);

    m_fileSystemModel->setFilter(QDir::Dirs | QDir::Drives |
                                 QDir::NoDotAndDotDot | QDir::AllDirs);


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
    m_outputDirEdit->setText(QDir::toNativeSeparators(settings->outputPath()));
    m_gtlabDirEdit->setText(QDir::toNativeSeparators(settings->gtlabPath()));
    m_devToolsDirEdit->setText(QDir::toNativeSeparators(settings->devToolsPath()));
}

/*
 * getter
 */
QString
SettingsPage::outputPath() const
{
    return m_outputDirEdit->text();
}

QString
SettingsPage::gtlabPath() const
{
    return m_gtlabDirEdit->text();
}

QString
SettingsPage::devToolsPath() const
{
    return m_devToolsDirEdit->text();
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
    if (devToolsPath().isEmpty() ||
        gtlabPath().isEmpty() ||
        outputPath().isEmpty())
    {
        return false;
    }

    if (!QFileInfo(QFile(outputPath())).isDir())
    {
        m_outputDirEdit->setStyleSheet("color : red");
    }
    else
    {
        m_outputDirEdit->setStyleSheet("color : black");
    }

    if (!QFileInfo(QFile(gtlabPath())).isDir())
    {
        m_gtlabDirEdit->setStyleSheet("color : red");

        return false;
    }

    m_gtlabDirEdit->setStyleSheet("color : black");

    if (!QFileInfo(QFile(devToolsPath())).isDir())
    {
        m_devToolsDirEdit->setStyleSheet("color : red");

        return false;
    }

    m_devToolsDirEdit->setStyleSheet("color : black");

    return QWizardPage::isComplete();
}

bool
SettingsPage::validatePage()
{
    // for nicer debug output
    {
        LOG_INSTANCE("validated!");

        LOG_INFO << "output path " << outputPath() << ENDL;
        LOG_INFO << "path to GTlab " << gtlabPath() << ENDL;
        LOG_INFO << "path to DevTools " << devToolsPath() << ENDL;

        settings()->setOutputPath(outputPath());
        settings()->setGTlabPath(gtlabPath());
        settings()->setDevToolsPath(devToolsPath());
    }

    settings()->preLoad();

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
                this, tr("Open Directory"), outputPath(),
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

    dirGTlab = QFileDialog::getExistingDirectory(
                this, tr("Open Directory"), gtlabPath(),
                QFileDialog::DontResolveSymlinks);

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
                this, tr("Open Directory"), devToolsPath(),
                  QFileDialog::DontResolveSymlinks);

    if (dirDevTools.isEmpty() == false)
    {
        onEditedDevToolsDir(dirDevTools);
    }
}


