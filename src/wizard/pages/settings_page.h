/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "abstract_wizardpage.h"

class QLineEdit;
class QComboBox;
class QCheckBox;
class QRadioButton;

/**
 * @brief to select output path, gtlab path and dev tools path
 */
class SettingsPage : public AbstractWizardPage
{
    Q_OBJECT

    static const int I_DIR_PUSHBTN_WIDTH;

    static const char* C_SETTINGS_PAGE_TITLE;

    static const QString S_INFO_TEXT;
    static const QString S_OUTPUT_LABEL;
    static const QString S_DEVTOOLS_LABEL;
    static const QString S_GTLAB_LABEL;
    static const QString S_OUTPUT_TOOLTIP;
    static const QString S_DEVTOOLS_TOOLTIP;
    static const QString S_GTLAB_TOOLTIP;
    static const QString S_VERSION_LABEL;
    static const QString S_VERSION_TOOLTIP;
    static const QString S_USE_MACRO_LABEL;
    static const QString S_USE_MACRO_TOOLTIP;
    static const QString S_GENERATOR_LABEL;
    static const QString S_GENERATOR_TOOLTIP;
    static const QString S_DIR_BTN_TOOLTIP;
    static const QString S_DIR_BTN_TEXT;

public:

    /**
     * @brief SettingsPage - constructor
     * @param settings
     * @param parent
     */
    explicit SettingsPage(ModuleGeneratorSettings* settings,
                          QWidget* parent = nullptr);

protected:

    void initializePage() override;

    bool isComplete() const override;

    bool validatePage() override;

private:

    /// output line edit
    QLineEdit* m_outputDirEdit{};
    /// gtlab line edit
    QLineEdit* m_gtlabDirEdit{};
    /// dev tools line edit
    QLineEdit* m_devToolsDirEdit{};

    QComboBox* m_versionBox{};

    QCheckBox* m_useMacroBox{};
    /// which generator to use
    QRadioButton* m_cmakeBtn{};
    QRadioButton* m_qmakeBtn{};

    static void updateDirEdit(QLineEdit* edit, QString& path);

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
