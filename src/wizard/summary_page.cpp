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

static QTextEdit* s_summaryEdit;

void updateSummary(QString&, ModuleGeneratorLogger::Type, int);

const char* C_SUMMARY_PAGE_TITLE = "Summary";

const QFont F_LABEL_FONT = QFont(QStringLiteral("Consolas"), 9);

const QString S_CHECKBOX_TEXT =
        QStringLiteral("WIP: Open the project in Qt Creator (this may take a moment)");
const QString S_GENERATE_BUTTON_TEXT =
        QStringLiteral("Generate");
const QString S_DONE_BUTTON_TEXT =
        QStringLiteral("Done");
/*
 * constructor
 */
SummaryPage::SummaryPage(ModuleGeneratorSettings *settings, QWidget *parent) :
    AbstractWizardPage(settings, parent)
{
    // initializations
    m_checkBox = new QCheckBox;
    m_summaryTextEdit = new QTextEdit;
    m_vLayout = new QVBoxLayout(this);


    // page gui
    setTitle(tr(C_SUMMARY_PAGE_TITLE));

    m_summaryTextEdit->setFont(F_LABEL_FONT);
    m_summaryTextEdit->setReadOnly(true);
    m_summaryTextEdit->setWordWrapMode(QTextOption::NoWrap);

    m_checkBox->setText(S_CHECKBOX_TEXT);
    m_checkBox->setChecked(false);
    m_checkBox->setEnabled(false);

    m_vLayout->addWidget(m_summaryTextEdit );
    m_vLayout->addWidget(m_checkBox);

    setLayout(m_vLayout);

    m_isComplete = true;

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
    LOG_INSTANCE("summary page...");

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
    if (wizard()->buttonText(QWizard::FinishButton) == S_DONE_BUTTON_TEXT && m_isComplete)
    {
        return true;
    }

    m_isComplete = false;

    LOG_INSTANCE("validated!");

    generateJson();

    printSummary();

    m_summaryTextEdit->clear();

    m_checkBox->setDisabled(true);

    wizard()->setButtonText(QWizard::FinishButton, S_DONE_BUTTON_TEXT);

    wizard()->setOption(QWizard::DisabledBackButtonOnLastPage);

    wizard()->setOption(QWizard::NoCancelButton);

    ModuleGeneratorLogger::registerFunction(updateSummary);

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
    content << "module name:\t" + moduleClass.ident;
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
    content << "";
    content << "GTLab directory:\t" + settings()->gtlabPath();
    content << "Dev-Tools directory:\t" + settings()->devToolsPath();

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

    for (auto* interface : settings()->selectedInterfaces())
    {
        if (interface == Q_NULLPTR) continue;

        content << interface->className;
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

    for (auto dependencie : settings()->selectedDependencies())
    {
        content << QString(dependencie.name + " (" + dependencie.version + ")");
    }

    if (content.isEmpty())
    {
        content << "";
    }

    content.first().prepend("selected dependencies:\t");
    content << "";

    return content.join("\n\t\t");
}

QString
SummaryPage::filesToGenerate()
{
    QStringList content;

    QString baseFolder = "src" + QString(QDir::separator());

    auto moduleClass = settings()->moduleClass();

    content << "local_settings.pri";
    content << "settings.pri";
    content << moduleClass.fileName + ".pro";
    content << baseFolder + moduleClass.fileName + ".h";
    content << baseFolder + moduleClass.fileName + ".cpp";
    content << baseFolder + moduleClass.fileName + ".json";

    for (auto* interface : settings()->selectedInterfaces())
    {
        if (interface == Q_NULLPTR) continue;

        for (auto* function : interface->functions)
        {
            if (function == Q_NULLPTR) continue;

            auto* baseClass  = function->baseClass;

            if (baseClass == Q_NULLPTR) continue;


            for (auto derivedClass : function->implementation.derivedClasses)
            {
                if (derivedClass.className.isEmpty()) continue;

                QString filePath (baseFolder + baseClass->outputPath +
                                  QDir::separator() + derivedClass.fileName);

                content << QString(filePath + ".h");
                content << QString(filePath + ".cpp");
            }
        }
    }

    if (!content.isEmpty())
    {
        content.first().prepend("files to generate:\t");

        content << "";
    }

    return content.join("\n\t\t");
}

void
SummaryPage::printSummary()
{
    QString debugString(m_summaryTextEdit->toPlainText());
    QString line(QStringLiteral("-").repeated(30));

    qDebug() << line;
    qDebug() << debugString.toStdString().c_str();
    qDebug() << line;
}


/*
 *  private
 */
void
SummaryPage::generateJson()
{
}


void
updateSummary(QString& text, ModuleGeneratorLogger::Type type, int indent)
{
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
        actualText = QString("[" + ModuleGeneratorLogger::typeName(type) +"] " + actualText);
    }

    s_summaryEdit->setTextColor(color);
    s_summaryEdit->insertPlainText(actualText);

    actualText.remove(ENDL);
    qDebug().noquote() << indendation + actualText;

    QCoreApplication::processEvents();
}
