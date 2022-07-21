#ifndef INTROPAGE_H
#define INTROPAGE_H

#include "abstract_wizardpage.h"

class QLabel;
class QRadioButton;
class QVBoxLayout;

/**
 * @brief The IntroPage class - the mode of the wizard is selected here
 */
class IntroPage : public AbstractWizardPage
{
    Q_OBJECT

    static const char* C_INTRO_PAGE_TITLE;

    static const QString S_INFO_TEXT;

public:

    /**
     * @brief IntroPage - constructor
     * @param settings
     * @param parent
     */
    IntroPage(ModuleGeneratorSettings* settings, QWidget* parent = nullptr);

protected:

    void initializePage() override;

    bool validatePage() override;

private:

    QRadioButton* m_createRBtn{};
//    QRadioButton* m_editRBtn{};
};
#endif // INTROPAGE_H
