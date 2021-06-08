#include "signature_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"
#include "module_generator_utils.h"
#include "module_generator.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>
#include <QRegularExpressionValidator>

const char* C_TITLE_SIGNATURE_PAGE = "File Signature";

const QString S_INFO_TEXT =
        QStringLiteral("Enter your user details to generate a file signature");
const QString S_AUTHOR_LABEL =
        QStringLiteral("Author Name:");
const QString S_EMAIL_LABEL =
        QStringLiteral("E-Mail:");
const QString S_SIGNATURE_LABEL =
        QStringLiteral("Signature:");

SignaturePage::SignaturePage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    m_infoTextLabel = new QLabel(S_INFO_TEXT);
    m_authorLabel = new QLabel(S_AUTHOR_LABEL);
    m_emailLabel = new QLabel(S_EMAIL_LABEL);
    m_signatureLabel = new QLabel(S_SIGNATURE_LABEL);
    m_authorEdit = new QLineEdit;
    m_emailEdit = new QLineEdit;
    m_signatureTextEdit = new QTextEdit;

    m_authorValdidator = new QRegularExpressionValidator(
                ModuleGeneratorSettings::REG_AUTHOR, this);
    m_emailValdidator = new QRegularExpressionValidator(
                ModuleGeneratorSettings::REG_AUTHOR_EMAIL, this);

    m_baseLayout = new QGridLayout;

    // page gui
    setTitle(C_TITLE_SIGNATURE_PAGE);

    m_infoTextLabel->setWordWrap(true);
    m_infoTextLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    m_infoTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_authorEdit->setValidator(m_authorValdidator);
    m_emailEdit->setValidator(m_emailValdidator);

    m_signatureLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_signatureTextEdit->setReadOnly(true);
    m_signatureTextEdit->setFont(ModuleGeneratorSettings::F_MONO_FONT);

    m_baseLayout->addWidget(m_infoTextLabel, 0, 0, 1, 3);
    m_baseLayout->addWidget(m_authorLabel, 1, 0);
    m_baseLayout->addWidget(m_authorEdit, 1, 1);
    m_baseLayout->addWidget(m_emailLabel, 2, 0);
    m_baseLayout->addWidget(m_emailEdit, 2, 1);
    m_baseLayout->addWidget(m_signatureLabel, 3, 0);
    m_baseLayout->addWidget(m_signatureTextEdit, 3, 1, 1, 2);
    m_baseLayout->setColumnMinimumWidth(0, AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(m_baseLayout);

    // sginals
    connect(m_authorEdit, SIGNAL(textEdited(QString)),
            this, SLOT(updateSignature()));
    connect(m_emailEdit, SIGNAL(textEdited(QString)),
            this, SLOT(updateSignature()));
    connect(m_authorEdit, SIGNAL(textEdited(QString)),
            this, SIGNAL(completeChanged()));    

    // defaults
    m_authorEdit->setText(settings->authorDetails().name);
    m_emailEdit->setText(settings->authorDetails().email);

    updateSignature();
}

void
SignaturePage::initializePage()
{
    LOG_INSTANCE("signature page...");
}

bool
SignaturePage::isComplete() const
{
    QString name = m_authorEdit->text();
    QString email = m_emailEdit->text();
    int pos = 0;

    return (m_authorValdidator->validate(name, pos) == QRegularExpressionValidator::Acceptable &&
            m_emailValdidator->validate(email, pos) == QRegularExpressionValidator::Acceptable);
}

bool
SignaturePage::validatePage()
{
    LOG_INSTANCE("validated!");

    AuthorDetails details;

    details.name = m_authorEdit->text().simplified();
    details.email = m_emailEdit->text().simplified();

    settings()->setAuthorDetails(details);

    return true;
}

void
SignaturePage::updateSignature()
{
    QString author(m_authorEdit->text().simplified());
    QString email(m_emailEdit->text().simplified());
    QString signature(ModuleGeneratorSettings::S_SIGNATURE);

    IdentifierPairs identifier;

    identifier << IdentifierPair{ ModuleGenerator::S_ID_AUTHOR, author };
    identifier << IdentifierPair{ ModuleGenerator::S_ID_AUTHOR_EMAIL, email };
    identifier << IdentifierPair{ ModuleGenerator::S_ID_FILE_NAME,
                  QStringLiteral("filename")};

    utils::replaceIdentifier(signature, identifier);

    m_signatureTextEdit->setText(signature);
}
