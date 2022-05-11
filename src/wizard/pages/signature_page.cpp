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

const char*
SignaturePage::C_TITLE_SIGNATURE_PAGE = "File Signature";

const QString
SignaturePage::S_INFO_TEXT = QStringLiteral("Enter your user details to "
                                            "generate a file signature.");
const QString
SignaturePage::S_AUTHOR_LABEL = QStringLiteral("Author name:");
const QString
SignaturePage::S_EMAIL_LABEL = QStringLiteral("E-Mail:");
const QString
SignaturePage::S_SIGNATURE_LABEL = QStringLiteral("Signature:");

SignaturePage::SignaturePage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    auto* infoLabel = new QLabel(S_INFO_TEXT);
    auto* authorLabel = new QLabel(S_AUTHOR_LABEL);
    auto* emailLabel = new QLabel(S_EMAIL_LABEL);
    auto* signatureLabel = new QLabel(S_SIGNATURE_LABEL);
    auto* baseLayout = new QGridLayout;

    m_authorEdit = new QLineEdit;
    m_emailEdit = new QLineEdit;
    m_signatureTextEdit = new QTextEdit;

    m_authorValdidator = new QRegularExpressionValidator(
                ModuleGeneratorSettings::REG_AUTHOR, this);
    m_emailValdidator = new QRegularExpressionValidator(
                ModuleGeneratorSettings::REG_AUTHOR_EMAIL, this);


    // page gui
    setTitle(C_TITLE_SIGNATURE_PAGE);

    infoLabel->setWordWrap(true);
    infoLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    infoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_authorEdit->setValidator(m_authorValdidator);
    m_emailEdit->setValidator(m_emailValdidator);

    signatureLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_signatureTextEdit->setReadOnly(true);
    m_signatureTextEdit->setFont(ModuleGeneratorSettings::F_MONO_FONT);

    int row = 0;
    baseLayout->addWidget(infoLabel, row++, 0, 1, 3);
    baseLayout->addWidget(authorLabel, row, 0);
    baseLayout->addWidget(m_authorEdit, row++, 1);
    baseLayout->addWidget(emailLabel, row, 0);
    baseLayout->addWidget(m_emailEdit, row++, 1);
    baseLayout->addWidget(signatureLabel, row, 0);
    baseLayout->addWidget(m_signatureTextEdit, row++, 1, 1, 2);
    baseLayout->setColumnMinimumWidth(0,
                                      AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(baseLayout);

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
    LOG_INDENT("signature page...");
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
    LOG_INDENT("validated!");

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
