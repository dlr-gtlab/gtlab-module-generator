#ifndef SUMMARYPAGE_H
#define SUMMARYPAGE_H

#include "abstract_wizardpage.h"


class QTextEdit;
class QVBoxLayout;
class QCheckBox;

/**
 * @brief The SummaryPage class - final page of this wizard, overview of
 * selections and choices is shown here
 */
class SummaryPage : public AbstractWizardPage
{
    Q_OBJECT

    static const char* C_SUMMARY_PAGE_TITLE;

    static const QString S_GENERATE_BUTTON_TEXT;
    static const QString S_DONE_BUTTON_TEXT;
    static const QString S_GIT_CHECK_BOX_TEXT;
    static const QString S_GIT_CHECK_BOX_TOOLTIP;

public:

    /**
     * @brief SummaryPage - constructor
     * @param settings
     * @param parent
     */
    explicit SummaryPage(ModuleGeneratorSettings* settings,
                         QWidget* parent = nullptr);

public slots:

    void onGenerationFinished();

protected:

    void initializePage() override;

    bool isComplete() const override;

    bool validatePage() override;

private:

    /// summary text edit
    QTextEdit* m_summaryTextEdit{};

    QCheckBox* m_generateGitFilesCheckBox{};

    bool m_isComplete{true};

    /**
     * @brief createSummary - generates a summary of selections and choices
     * made throughout the wizard process
     */
    void createSummary();

    QString selectedInterfaces();

    QString selectedDependencies();

    QString filesToGenerate();

    /**
     * @brief printSummary - prints the content of the textEdit to console
     */
    void printSummary();

signals:

    void validated();

};

#endif // SUMMARYPAGE_H
