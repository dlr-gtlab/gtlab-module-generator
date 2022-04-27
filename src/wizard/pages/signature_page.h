#ifndef SIGNATUREPAGE_H
#define SIGNATUREPAGE_H

#include "abstract_wizardpage.h"

class QLabel;
class QLineEdit;
class QGridLayout;
class QTextEdit;
class QRegularExpressionValidator;
class SignaturePage : public AbstractWizardPage
{
    Q_OBJECT

    static const char* C_TITLE_SIGNATURE_PAGE;

    static const QString S_INFO_TEXT;
    static const QString S_AUTHOR_LABEL;
    static const QString S_EMAIL_LABEL;
    static const QString S_SIGNATURE_LABEL;

public:

    explicit SignaturePage(ModuleGeneratorSettings* settings,
                           QWidget* parent = nullptr);

protected:

    void initializePage() override;

    bool isComplete() const override;

    bool validatePage() override;

private:

    QLineEdit* m_authorEdit{};
    QLineEdit* m_emailEdit{};
    QTextEdit* m_signatureTextEdit{};

    /// author name validator
    QRegularExpressionValidator* m_authorValdidator{};
    /// email validator
    QRegularExpressionValidator* m_emailValdidator{};

private slots:

    void updateSignature();

};

#endif // SIGNATUREPAGE_H
