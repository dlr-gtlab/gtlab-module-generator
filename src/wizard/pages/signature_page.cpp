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
#include <QCheckBox>
#include <QDate>

const char*
SignaturePage::C_TITLE_SIGNATURE_PAGE = "File Signature";

const QString
SignaturePage::S_INFO_TEXT =
        QStringLiteral("Enter your user details to generate a file signature. "
                       "Contact details will also be used for the meta data "
                       "of the module.");
const QString
SignaturePage::S_AUTHOR_LABEL = QStringLiteral("Author name:");
const QString
SignaturePage::S_EMAIL_LABEL = QStringLiteral("E-Mail:");

SignaturePage::SignaturePage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    auto* infoLabel = new QLabel(S_INFO_TEXT);
    auto* authorLabel = new QLabel(S_AUTHOR_LABEL);
    auto* emailLabel = new QLabel(S_EMAIL_LABEL);
    auto* signatureLabel = new QLabel(tr("Signature preview:"));
    auto* baseLayout = new QGridLayout;

    m_authorEdit = new QLineEdit;
    m_emailEdit = new QLineEdit;
    m_signatureTextEdit = new QTextEdit;
    m_signatureTextView = new QTextEdit;

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
    signatureLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_signatureTextEdit->setFont(ModuleGeneratorSettings::F_MONO_FONT);
    m_signatureTextEdit->setToolTip(tr(
R"(Signature template. Following identifiers may be used:
- $$YEAR$: current year
- $$DATE$: current date
- $$AUTHOR$: author name
- $$AUTHOR_EMAIL$: author email
- $$MODULE_NAME$$: module name
- $$PREFIX$$: module prefix
)"));

    m_signatureTextView->setFont(ModuleGeneratorSettings::F_MONO_FONT);
    m_signatureTextView->setReadOnly(true);
    m_signatureTextView->setToolTip(tr("Preview of generated signature"));

    m_useSignature = new QCheckBox(tr("Signature:"));
    m_useSignature->setToolTip(tr("If disabled, no signature will "
                                  "be applied to the generated source files"));
    m_useSignature->setChecked(true);


    int row = 0;
    baseLayout->addWidget(infoLabel, row++, 0, 1, 3);
    baseLayout->addWidget(authorLabel, row, 0);
    baseLayout->addWidget(m_authorEdit, row++, 1);
    baseLayout->addWidget(emailLabel, row, 0);
    baseLayout->addWidget(m_emailEdit, row++, 1);
    baseLayout->addWidget(m_useSignature, row, 0, Qt::AlignTop | Qt::AlignLeft);
    baseLayout->addWidget(m_signatureTextEdit, row++, 1, 1, 2);
    baseLayout->addWidget(signatureLabel, row, 0);
    baseLayout->addWidget(m_signatureTextView, row++, 1, 1, 2);
    baseLayout->setColumnMinimumWidth(0,
                                      AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(baseLayout);

    // sginals
    connect(m_authorEdit, SIGNAL(textEdited(QString)),
            this, SLOT(updateSignature()));
    connect(m_emailEdit, SIGNAL(textEdited(QString)),
            this, SLOT(updateSignature()));
    connect(m_signatureTextEdit, SIGNAL(textChanged()),
            this, SLOT(updateSignature()));

    connect(m_authorEdit, SIGNAL(textEdited(QString)),
            this, SIGNAL(completeChanged()));
    connect(m_emailEdit, SIGNAL(textEdited(QString)),
            this, SIGNAL(completeChanged()));

    connect(m_useSignature, &QCheckBox::stateChanged, this, [=](){
        m_signatureTextEdit->setEnabled(m_useSignature->isChecked());
        m_signatureTextView->setEnabled(m_useSignature->isChecked());
    });

    // defaults
    m_authorEdit->setText(settings->authorDetails().name);
    m_emailEdit->setText(settings->authorDetails().email);
}

void
SignaturePage::initializePage()
{
    LOG_INDENT("signature page...");

    m_useSignature->setChecked(settings()->useSignature());
    m_signatureTextEdit->setText(settings()->plainSignature());

    updateSignature();
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
    settings()->setPlainSignature(m_signatureTextEdit->toPlainText());
    settings()->setUseSignature(m_useSignature->isChecked());

    LOG_INFO << "use signature: " << (settings()->useSignature() ? "true":"false") << ENDL;
    LOG_INFO << "signature text: " << settings()->plainSignature() << ENDL;
    LOG_INFO << "done!";

    return true;
}

void
SignaturePage::updateSignature()
{
    QString author(m_authorEdit->text().simplified());
    QString email(m_emailEdit->text().simplified());
    QString signature(m_signatureTextEdit->toPlainText());

    IdentifierPairs identifierPairs;
    identifierPairs.append({ ModuleGenerator::S_ID_AUTHOR, author });
    identifierPairs.append({ ModuleGenerator::S_ID_AUTHOR_EMAIL, email });
    identifierPairs.append({ ModuleGenerator::S_ID_GENERATOR_VERSION, ModuleGeneratorSettings::S_VERSION });
    identifierPairs.append({ ModuleGenerator::S_ID_YEAR, settings()->currentYear() });
    identifierPairs.append({ ModuleGenerator::S_ID_DATE, settings()->currentDate() });

    identifierPairs.append({ ModuleGenerator::S_ID_PREFIX, settings()->modulePrefix() });
    identifierPairs.append({ ModuleGenerator::S_ID_MODULE_NAME, settings()->moduleClass().ident });

    utils::replaceIdentifier(signature, identifierPairs);

    m_signatureTextView->setText(signature);
}
