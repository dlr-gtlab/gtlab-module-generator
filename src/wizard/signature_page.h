#ifndef SIGNATUREPAGE_H
#define SIGNATUREPAGE_H

#include "abstract_wizardpage.h"

class QLabel;
class QLineEdit;
class QGridLayout;
class QTextEdit;
class QCheckBox;
class QRegularExpressionValidator;
class SignaturePage : public AbstractWizardPage
{
    Q_OBJECT

public:

    explicit SignaturePage(ModuleGeneratorSettings* settings,
                           QWidget* parent = nullptr);

protected:

    void initializePage() Q_DECL_OVERRIDE;

    bool isComplete() const Q_DECL_OVERRIDE;

    bool validatePage() Q_DECL_OVERRIDE;

private:

    QLabel* m_infoTextLabel;

    QLabel* m_authorLabel;
    QLabel* m_emailLabel;
    QLabel* m_signatureLabel;

    QLineEdit* m_authorEdit;
    QLineEdit* m_emailEdit;
    QTextEdit* m_signatureTextEdit;

    QGridLayout* m_baseLayout;
    /// author name validator
    QRegularExpressionValidator* m_authorValdidator;

private slots:

    void updateSignature();

};

#endif // SIGNATUREPAGE_H
