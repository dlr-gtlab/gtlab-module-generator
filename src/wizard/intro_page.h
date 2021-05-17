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

public:

    /**
     * @brief IntroPage - constructor
     * @param settings
     * @param parent
     */
    IntroPage(ModuleGeneratorSettings* settings, QWidget* parent = nullptr);

protected:

    void initializePage() Q_DECL_OVERRIDE;

    bool validatePage() Q_DECL_OVERRIDE;

private:

    /// info text label
    QLabel* m_infoTextLabel;
    /// create radio button
    QRadioButton* m_createRBtn;
    /// edit radio button
    QRadioButton* m_editRBtn;
    /// vertical layout
    QVBoxLayout* m_baseLayout;

};
#endif // INTROPAGE_H
