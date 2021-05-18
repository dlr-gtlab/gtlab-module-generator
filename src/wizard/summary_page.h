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

    void initializePage() Q_DECL_OVERRIDE;

    bool isComplete() const Q_DECL_OVERRIDE;

    bool validatePage() Q_DECL_OVERRIDE;

private:

    /// summary text edit
    QTextEdit* m_summaryTextEdit;
    /// vertical layout
    QVBoxLayout* m_vLayout;
    /// check box to set open qtcreator
    QCheckBox* m_checkBox;

    bool m_isComplete;

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
