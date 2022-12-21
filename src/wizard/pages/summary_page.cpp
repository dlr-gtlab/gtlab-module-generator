#include <QWizardPage>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QColor>
#include <QCheckBox>
#include <QFileDialog>
#include <QFont>
#include <QCoreApplication>

#include "summary_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"

static QTextEdit* s_summaryEdit; // ref for updateSummary methode

void updateSummary(QString const&, ModuleGeneratorLogger::Type, int);

const char*
SummaryPage::C_SUMMARY_PAGE_TITLE = "Summary";

const QString
SummaryPage::S_GENERATE_BUTTON_TEXT = QStringLiteral("Generate");
const QString
SummaryPage::S_DONE_BUTTON_TEXT = QStringLiteral("Done");
const QString
SummaryPage::S_GIT_CHECK_BOX_TEXT = QStringLiteral("generate additional git "
                                                   "files");
const QString
SummaryPage::S_GIT_CHECK_BOX_TOOLTIP = QStringLiteral("README.md, CHANGELOG.md "
                                                      "and .gitignore");
/*
 * constructor
 */
SummaryPage::SummaryPage(ModuleGeneratorSettings *settings, QWidget *parent) :
    AbstractWizardPage(settings, parent)
{
    // initializations
    m_summaryTextEdit = new QTextEdit;
    m_generateGitFilesCheckBox = new QCheckBox(S_GIT_CHECK_BOX_TEXT);
    auto* baseLayout = new QVBoxLayout(this);

    // page gui
    setTitle(tr(C_SUMMARY_PAGE_TITLE));

    m_summaryTextEdit->setFont(ModuleGeneratorSettings::F_MONO_FONT);
    m_summaryTextEdit->setReadOnly(true);
    m_summaryTextEdit->setWordWrapMode(QTextOption::NoWrap);

    m_generateGitFilesCheckBox->setChecked(true);
    m_generateGitFilesCheckBox->setToolTip(S_GIT_CHECK_BOX_TOOLTIP);

    baseLayout->addWidget(m_summaryTextEdit);
    baseLayout->addWidget(m_generateGitFilesCheckBox);

    setLayout(baseLayout);

    s_summaryEdit = m_summaryTextEdit;
}

void
SummaryPage::onGenerationFinished()
{
    m_isComplete = true;

    this->update();

    wizard()->button(QWizard::FinishButton)->setEnabled(true);

    QCoreApplication::processEvents();
}

/*
 * protected
 */
void
SummaryPage::initializePage()
{
    LOG_INDENT("summary page...");

    wizard()->setButtonText(QWizard::FinishButton, S_GENERATE_BUTTON_TEXT);

    createSummary();
}

bool
SummaryPage::isComplete() const
{
    return m_isComplete;
}

bool
SummaryPage::validatePage()
{
    if (wizard()->buttonText(QWizard::FinishButton) == S_DONE_BUTTON_TEXT &&
        m_isComplete)
    {
        ModuleGeneratorLogger::unregisterLoggingFunction();

        return true;
    }

    // for nicer std out
    {
        LOG_INDENT("validated!");

        m_isComplete = false;

        settings()->setCreateGitFiles(m_generateGitFilesCheckBox->isChecked());
        m_generateGitFilesCheckBox->setEnabled(false);

        settings()->serializeUserData();

        LOG_INFO << "done!";
    }

    printSummary();

    m_summaryTextEdit->clear();

    wizard()->setButtonText(QWizard::FinishButton, S_DONE_BUTTON_TEXT);

    wizard()->setOption(QWizard::DisabledBackButtonOnLastPage);

    wizard()->setOption(QWizard::NoCancelButton);

    ModuleGeneratorLogger::registerLoggingFunction(updateSummary);

    emit validated();

    return false;
}

/*
 * private
 */
void
SummaryPage::createSummary()
{
    QStringList content;

    auto moduleClass = settings()->moduleClass();

    content << "moudle prefix:\t" + settings()->modulePrefix();
    content << "module name:  \t" + moduleClass.ident;
    content << "module class name:\t" + moduleClass.className;
    content << "module file name:\t" + moduleClass.fileName;
    content << "module description:\t" + moduleClass.description;
    content << "module version:\t" + moduleClass.version;

    QString moduleFolder = moduleClass.ident;
    moduleFolder.remove(' ');
    QString outputPath   = QDir::cleanPath(settings()->outputPath() +
                                           QDir::separator() +
                                           moduleFolder);
    outputPath.replace("/", QDir::separator());

    content << "";
    content << "Output directory:\t" + outputPath;
    content << "DevTools directory:\t" + settings()->devToolsPath();
    content << "GTlab directory:\t" + settings()->gtlabPath();
    content << "GTlab target version:\t" + settings()->gtlabVersion();

    content << "";
    content << selectedInterfaces();
    content << selectedDependencies();
    content << filesToGenerate();

    m_summaryTextEdit->setText(content.join(QStringLiteral("\n")));
}


QString
SummaryPage::selectedInterfaces()
{
    QStringList content;

    for (auto& interface : settings()->selectedInterfaces())
    {
//        if (interface == Q_NULLPTR) continue;

        content << interface.className;
    }

    if (content.isEmpty())
    {
        content << "";
    }

    content.first().prepend("selected interfaces:\t");
    content << "";

    return content.join("\n\t\t");
}

QString
SummaryPage::selectedDependencies()
{
    QStringList content;

    for (auto const& dependency : settings()->selectedDependencies())
    {
        content << QString(dependency.name + " (" + dependency.version + ")");
    }

    if (content.isEmpty())
    {
        content << QString{};
    }

    content.first().prepend("selected dependencies:\t");
    content << QString{};

    return content.join("\n\t\t");
}

QString
SummaryPage::filesToGenerate()
{
    QStringList content;

    QString baseFolder = "src" + QString(QDir::separator());

    auto moduleClass = settings()->moduleClass();

    content << moduleClass.fileName + ".pro";
    content << "settings.pri";
    content << "local_settings.pri";
    content << "features" + QString(QDir::separator()) + "local_settings.pri";
    content << baseFolder + "src.pro";
    content << baseFolder + moduleClass.fileName + ".h";
    content << baseFolder + moduleClass.fileName + ".cpp";
    content << baseFolder + moduleClass.fileName + ".json";

    for (auto& interface : settings()->selectedInterfaces())
    {
//        if (interface == Q_NULLPTR) continue;

        for (auto& function : interface.functions)
        {
            auto& baseClass  = function.baseClass;

            for (auto const& derivedClass : function.implementation.derivedClasses)
            {
                if (derivedClass.className.isEmpty()) continue;

                QString filePath (baseFolder + baseClass.outputPath +
                                  QDir::separator() + derivedClass.fileName);

                content << QDir::toNativeSeparators(filePath + ".h");
                content << QDir::toNativeSeparators(filePath + ".cpp");
            }
        }
    }

    if (!content.isEmpty())
    {
        content.first().prepend("files to generate:\t");

        content << QString{};
    }

    return content.join("\n\t\t");
}

void
SummaryPage::printSummary()
{
    QString line = QStringLiteral("-").repeated(30);

    qDebug() << line;
    qDebug().noquote() << m_summaryTextEdit->toPlainText();
    qDebug() << line;
}

void
updateSummary(QString const& text, ModuleGeneratorLogger::Type type, int indent)
{
    if (!s_summaryEdit) return;

    QString indendation = text.left(indent > 0 ? indent - 1:0);
    QString actualText  = text.mid(indent > 0 ? indent - 1:0);

    QColor color;
    color = QStringLiteral("#000000"); // black

    s_summaryEdit->setTextColor(color);
    s_summaryEdit->insertPlainText(indendation);

    if (type == ModuleGeneratorLogger::Warning)
    {
        color = QStringLiteral("#e5a800"); // yellow
    }
    else if (type == ModuleGeneratorLogger::Error)
    {
        color = QStringLiteral("#ff0000"); // red
    }

    if (type != ModuleGeneratorLogger::Info)
    {
        actualText = QString("[" + ModuleGeneratorLogger::typeName(type) +"] " +
                             actualText);
    }

    s_summaryEdit->setTextColor(color);
    s_summaryEdit->insertPlainText(actualText);

    actualText.remove(ENDL);
    qDebug().noquote() << indendation + actualText;

    QCoreApplication::processEvents();
}
