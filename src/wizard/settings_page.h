#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H


#include "abstract_wizardpage.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;
class QFileSystemModel;
class QString;
class QCompleter;

/**
 * @brief to select output path, gtlab path and dev tools path
 */
class SettingsPage : public AbstractWizardPage
{
    Q_OBJECT

public:

    /**
     * @brief SettingsPage - constructor
     * @param settings
     * @param parent
     */
    explicit SettingsPage(ModuleGeneratorSettings* settings,
                          QWidget* parent = nullptr);

protected:

    void initializePage() Q_DECL_OVERRIDE;

    bool isComplete() const Q_DECL_OVERRIDE;

    bool validatePage() Q_DECL_OVERRIDE;

private:

    /// info tet label
    QLabel* m_infoTextLabel;

    /// output dir label
    QLabel* m_outputDirLabel;
    /// gtlab label
    QLabel* m_gtlabLabel;
    /// devtools label
    QLabel* m_devToolsLabel;

    /// output line edit
    QLineEdit* m_outputDirEdit;
    /// gtlab line edit
    QLineEdit* m_gtlabDirEdit;
    /// dev tools line edit
    QLineEdit* m_devToolsDirEdit;

    /// output push button
    QPushButton* m_outputDirPushBtn;
    /// gtlab push button
    QPushButton* m_gtlabDirPushBtn;
    /// dev tools push button
    QPushButton* m_devToolsDirPushBtn;

    /// grid layout
    QGridLayout* m_baseGridLayout;

    bool setLineEditColor(QLineEdit* edit, bool isExec = false) const;

private slots:

    /**
     * @brief onEditedOutputDir
     */
    void onEditedOutputDir(QString path);

    /**
     * @brief onEditedGTlabDir
     */
    void onEditedGTlabDir(QString path);

    /**
     * @brief onEditedDevToolsDir
     */
    void onEditedDevToolsDir(QString path);

    /**
     * @brief onPressedOutputDirPushBtn - opens file dialog
     * selects dir in CREATE mode, selects file in EDIT mode
     */
    void onPressedOutputDirPushBtn();

    /**
     * @brief onPressedGTlabDirPushBtn - opens file dialog
     */
    void onPressedGTlabDirPushBtn();

    /**
     * @brief onPressedDevToolsDirPushBtn - opens file dialog
     */
    void onPressedDevToolsDirPushBtn();

};

#endif // SETTINGSPAGE_H
