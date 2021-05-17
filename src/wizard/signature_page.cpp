#include "signature_page.h"

#include "module_generator_settings.h"
#include "module_generator_logger.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QGridLayout>
#include <QRegularExpressionValidator>
#include <QDate>

const char* C_TITLE_SIGNATURE_PAGE = "File Signature";

const QString S_INFO_TEXT =
        QStringLiteral("add a signature");
const QString S_AUTHOR_LABEL =
        QStringLiteral("Author Name:");
const QString S_EMAIL_LABEL =
        QStringLiteral("E-Mail:");
const QString S_SIGNATURE_LABEL =
        QStringLiteral("Signature:");

SignaturePage::SignaturePage(ModuleGeneratorSettings* settings, QWidget* parent) :
    AbstractWizardPage(settings, parent)
{
    m_infoTextLabel = new QLabel;
    m_authorLabel = new QLabel(S_AUTHOR_LABEL);
    m_authorEdit = new QLineEdit;
    m_emailLabel = new QLabel(S_EMAIL_LABEL);
    m_emailEdit = new QLineEdit;
    m_signatureLabel = new QLabel(S_SIGNATURE_LABEL);
    m_signatureTextEdit = new QTextEdit;
    m_authorValdidator = new QRegularExpressionValidator(ModuleGeneratorSettings::REG_AUTHOR, this);

    m_baseLayout = new QGridLayout;

    m_infoTextLabel->setWordWrap(true);
    m_infoTextLabel->setMinimumHeight(AbstractWizardPage::I_INFOTEXTLABEL_HEIGHT);
    m_infoTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_authorEdit->setValidator(m_authorValdidator);

    m_signatureLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_signatureTextEdit->setReadOnly(true);
    m_signatureTextEdit->setFont(QFont("Consolas", 9));

    m_baseLayout->addWidget(m_infoTextLabel, 0, 0, 1, 3);
    m_baseLayout->addWidget(m_authorLabel, 1, 0);
    m_baseLayout->addWidget(m_authorEdit, 1, 1);
    m_baseLayout->addWidget(m_emailLabel, 2, 0);
    m_baseLayout->addWidget(m_emailEdit, 2, 1);
    m_baseLayout->addWidget(m_signatureLabel, 3, 0);
    m_baseLayout->addWidget(m_signatureTextEdit, 3, 1, 1, 2);
    m_baseLayout->setColumnMinimumWidth(0, AbstractWizardPage::I_PAGES_COLUMN_WIDTH);

    setLayout(m_baseLayout);

    setTitle(C_TITLE_SIGNATURE_PAGE);

    // sginals
    connect(m_authorEdit, SIGNAL(textEdited(QString)),
            this, SLOT(updateSignature()));
    connect(m_emailEdit, SIGNAL(textEdited(QString)),
            this, SLOT(updateSignature()));
    connect(m_authorEdit, SIGNAL(textEdited(QString)),
            this, SIGNAL(completeChanged()));

    updateSignature();
}

void
SignaturePage::initializePage()
{
    LOG_INSTANCE("signature page...");

    m_infoTextLabel->setText(S_INFO_TEXT);

}

bool
SignaturePage::isComplete() const
{
    QString name = m_authorEdit->text();
    int pos = 0;
    return m_authorValdidator->validate(name, pos);
}

bool
SignaturePage::validatePage()
{
    LOG_INSTANCE("validated!");

//    settings()->setSignature(getSignature());
//    settings()->setAuthor(getAuthor());
//    settings()->setTel(getTelephone());
//    settings()->setEmail(getEmail());

    AuthorDetails details;

    details.name = m_authorEdit->text().simplified();

    settings()->setAuthorDetails(details);
    settings()->setSignatureString(m_signatureTextEdit->toPlainText());

    return true;
}

void
SignaturePage::updateSignature()
{
    QString author(m_authorEdit->text().simplified());
    QString email(m_emailEdit->text().simplified());
    QString signature;

    signature = QString("/* GTlab - Gas Turbine laboratory\n"
                        " * Source File: $$FILE_NAME$$\n"
                        " * copyright 2009-" + QDate::currentDate().toString("yyyy")+ " by DLR\n"
                        " * \n"
                        " * Created on: " +
                        QDate::currentDate().toString("dd.MM.yyyy") +
                        "\n");


    signature += " * Author: " + author + " \n";
    if (!email.isEmpty())
    {
        signature += " * Email: " + email + " \n";
    }

    signature += " */";

    m_signatureTextEdit->setText(signature);
}
