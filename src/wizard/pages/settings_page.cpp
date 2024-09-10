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
#include <QCheckBox>
#include <QRadioButton>

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
SettingsPage::S_VERSION_LABEL = QStringLiteral("Target version");
const QString
SettingsPage::S_VERSION_TOOLTIP = QStringLiteral("Target version of GTlab");
const QString
SettingsPage::S_USE_MACRO_LABEL =
        QStringLiteral("Keep compatibility to other versions");
const QString
SettingsPage::S_USE_MACRO_TOOLTIP =
        QStringLiteral("Macros will be added to the implementation to\ndisable "
                       "and enable code depending on the current\nGTlab "
                       "version. Thus the same module may be used\nfor "
                       "different versions of GTlab.");
const QString
SettingsPage::S_GENERATOR_LABEL = QStringLiteral("Generator:");
const QString
SettingsPage::S_GENERATOR_TOOLTIP =
        QStringLiteral("Generator used to organize the project files");

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
    auto* versionLabel = new QLabel(S_VERSION_LABEL);
    auto* generatorLabel = new QLabel(S_GENERATOR_LABEL);

    m_outputDirEdit      = new QLineEdit;
    m_gtlabDirEdit       = new QLineEdit;
    m_devToolsDirEdit    = new QLineEdit;
    m_versionBox = new QComboBox;
    m_useMacroBox = new QCheckBox{S_USE_MACRO_LABEL};

    m_cmakeBtn = new QRadioButton("CMake");
    m_qmakeBtn = new QRadioButton("QMake (deprecated)");

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

    m_versionBox->setMaximumWidth(100);
    m_versionBox->setToolTip(S_VERSION_TOOLTIP);
    m_useMacroBox->setToolTip(S_USE_MACRO_TOOLTIP);

    m_cmakeBtn->setToolTip(S_GENERATOR_TOOLTIP);
    m_qmakeBtn->setToolTip(S_GENERATOR_TOOLTIP);

    // layout
    int row = 0;
    baseGridLayout->addWidget(infoLabel, row++, 0, 1, 3);
    baseGridLayout->addWidget(outputLabel, row, 0);
    baseGridLayout->addWidget(m_outputDirEdit, row, 1, 1, 2);
    baseGridLayout->addWidget(outputDirPushBtn, row++, 3);
    baseGridLayout->addWidget(gtlabLabel, row, 0);
    baseGridLayout->addWidget(m_gtlabDirEdit, row, 1, 1, 2);
    baseGridLayout->addWidget(gtlabDirPushBtn, row++, 3);
    baseGridLayout->addWidget(devToolsLabel, row, 0);
    baseGridLayout->addWidget(m_devToolsDirEdit, row, 1, 1, 2);
    baseGridLayout->addWidget(devToolsDirPushBtn, row++, 3);
    baseGridLayout->addWidget(versionLabel, row, 0);
    baseGridLayout->addWidget(m_versionBox, row, 1);
    baseGridLayout->addWidget(m_useMacroBox, row++, 2, 1, 2);
    baseGridLayout->addWidget(generatorLabel, row, 0);
    baseGridLayout->addWidget(m_cmakeBtn, row++, 1, 1, 2);
    baseGridLayout->addWidget(m_qmakeBtn, row++, 1, 1, 2);
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
}

/*
 * proteced
 */
void
SettingsPage::initializePage()
{
    LOG_INDENT("settings page...");

    QString gtlabPath(settings()->gtlabPath());

    if (!gtlabPath.isEmpty())
    {
        gtlabPath += QDir::separator() + ModuleGeneratorSettings::S_GTLAB_APP;
    }

    m_outputDirEdit->setText(QDir::toNativeSeparators(
                                 settings()->outputPath()));
    m_gtlabDirEdit->setText(QDir::toNativeSeparators(gtlabPath));
    m_devToolsDirEdit->setText(QDir::toNativeSeparators(
                                   settings()->devToolsPath()));

    m_useMacroBox->setChecked(settings()->useCompatibilityMacros());

    m_cmakeBtn->setChecked(settings()->useCMakeGenerator());
    m_qmakeBtn->setChecked(settings()->useQMakeGenerator());
    if (!m_cmakeBtn->isChecked() && !m_qmakeBtn->isChecked())
    {
        m_cmakeBtn->setChecked(true);
    }

    // register versions
    auto versions = settings()->supportedVersions();
    auto const& lastVersion = settings()->gtlabVersion();
    // set last version
    if (versions.contains(lastVersion))
    {
        versions.removeOne(lastVersion);
        versions.prepend(lastVersion);
    }

    m_versionBox->clear();
    m_versionBox->addItems(versions);

    if (versions.size() <= 1)
    {
        m_useMacroBox->setEnabled(false);
        m_useMacroBox->setChecked(false);
        m_versionBox->setEnabled(false);
    }
}


bool
SettingsPage::isComplete() const
{
    bool retVal(true);

    setLineEditColor(m_outputDirEdit);
    retVal &= setLineEditColor(m_gtlabDirEdit, true);
    retVal &= setLineEditColor(m_devToolsDirEdit);

    if (!retVal) return false;

    if (m_outputDirEdit->text().isEmpty() ||
        m_gtlabDirEdit->text().isEmpty() ||
        m_devToolsDirEdit->text().isEmpty())
    {
        return false;
    }

    if (!m_cmakeBtn->isChecked() && !m_qmakeBtn->isChecked())
    {
        return false;
    }

    return true;
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
        settings()->setUseCompatibilityMacros(m_useMacroBox->isChecked());
        settings()->setUseCMakeGenerator(m_cmakeBtn->isChecked());
        settings()->setUseQMakeGenerator(m_qmakeBtn->isChecked());

        LOG_INFO << "target version: " << settings()->gtlabVersion() << ENDL;
        LOG_INFO << "major version:  "
                 << QString::number(settings()->gtlabMajorVersion()) << ENDL;
        LOG_INFO << "use compatibility macros: "
                 << QString{m_useMacroBox->isChecked() ? "true":"false"}
                 << ENDL;
        LOG_INFO << "using generator: "
                 << QString{m_cmakeBtn->isChecked() ? "cmake ":""}
                 << QString{m_qmakeBtn->isChecked() ? "qmake ":""}
                 << ENDL;

        if (settings()->useCompatibilityMacros())
        {
            QDir devtoolsDir = settings()->devToolsPath(); // dev tools path
            devtoolsDir.cdUp(); // e.g. 2_0
            devtoolsDir.cdUp(); // root dir

            if (!devtoolsDir.cd("tools") || !devtoolsDir.cd("CompatibilityLib"))
            {
                LOG_ERR << "Failed to find compatibility tools!" << ENDL;

                auto btn =
                QMessageBox::warning(
                    nullptr,
                    tr("Compatibility Tools not found"),
                    tr("Failed to find Compatibility Tools! Path does not exist:\n\n"
                       "    <DEV_TOOLS>/../../tools/CompatibilityLib\n\n"
                       "Make sure you have the Compatibility Tools installed using\n"
                       "the Maintenance Tool if you want to use an older version of\n"
                       "GTlab or double check the DevTools path!"),
                    QMessageBox::Cancel | QMessageBox::Ignore,
                    QMessageBox::Cancel
                );

                if (btn != QMessageBox::Ignore)
                {
                    LOG_INFO << ENDL;
                    return false;
                }
            }
        }

        LOG_INFO << "done!";
    }

    settings()->preLoad();

    return true;
}

bool
SettingsPage::setLineEditColor(QLineEdit* edit, bool isExec) const
{
    QString path{edit->text()};
    QFileInfo fileInfo{path};
    QPalette palette;

    bool isValid(isExec ? fileInfo.isFile() &&
                          fileInfo.fileName() == ModuleGeneratorSettings::S_GTLAB_APP:
                          fileInfo.isDir());

    if (!path.isEmpty() && !isValid)
    {
        palette.setColor(QPalette::Text, Qt::red);
        edit->setPalette(palette);
        return false;
    }

    edit->setPalette(palette);
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
    QString dirOutput = QFileDialog::getExistingDirectory(
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
    QString dirGTlab = QFileDialog::getOpenFileName(
                this, tr("Select the GTlab application"),
                m_gtlabDirEdit->text(),
                QStringLiteral("GTlab application (") +
                ModuleGeneratorSettings::S_GTLAB_APP +
                QStringLiteral(")"));

    if (!dirGTlab.isEmpty())
    {
        onEditedGTlabDir(dirGTlab);
    }
}

void
SettingsPage::onPressedDevToolsDirPushBtn()
{
    QString dirDevTools = QFileDialog::getExistingDirectory(
                this, tr("Select DevTools directory"),
                m_devToolsDirEdit->text(),
                QFileDialog::DontResolveSymlinks);

    if (!dirDevTools.isEmpty())
    {
        onEditedDevToolsDir(dirDevTools);
    }
}


