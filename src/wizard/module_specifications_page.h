#ifndef SPECIFICATIONSPAGE_H
#define SPECIFICATIONSPAGE_H


#include "abstract_wizardpage.h"


class QLabel;
class QLineEdit;
class QGridLayout;
class QCheckBox;
class QRegularExpressionValidator;
class QSpacerItem;

/**
 * @brief The ModuleSpecificationpage class - spefications for the module are
 * entered here
 */
class ModuleSpecificationsPage : public AbstractWizardPage
{
    Q_OBJECT

public:

    /**
     * @brief ModuleSpecificationpage - constructor
     * @param settings
     * @param parent
     */
    explicit ModuleSpecificationsPage(ModuleGeneratorSettings* settings,
                                      QWidget* parent = nullptr);


protected:

    void initializePage() Q_DECL_OVERRIDE;

    bool isComplete() const Q_DECL_OVERRIDE;

    bool validatePage() Q_DECL_OVERRIDE;


private:

    /// info text label
    QLabel* m_infoTextLabel;

    /// prefix label
    QLabel* m_modulePrefixLabel;
    /// module name label
    QLabel* m_moduleNameLabel;

    QLabel* m_classNameLabel;

    QLabel* m_fileNameLabel;
    /// version label
    QLabel* m_versionLabel;
    /// description label
    QLabel* m_descriptionLabel;

    /// prefix line edit
    QLineEdit* m_modulePrefixEdit;

    QLineEdit* m_classNameEdit;

    QLineEdit* m_fileNameEdit;
    /// module name line edit
    QLineEdit* m_moduleNameEdit;
    /// version line edit
    QLineEdit* m_versionEdit;
    /// description text edit
    QLineEdit* m_descriptionEdit;

    QCheckBox* m_autoEditCheckBox;

    /// grid layout
    QGridLayout* m_baseLayout;
    /// prefix validator
    QRegularExpressionValidator* m_prefixValidator;
    /// module name validator
    QRegularExpressionValidator* m_nameValidator;

    QRegularExpressionValidator* m_versionValidator;
    /// spacer between description and wizard bottom
    QSpacerItem* m_vSpacer;

    void autoComplete();

private slots:

    void onEditedPrefix();

    void onEditedModuleName(QString name);

    void onEditedVersion(QString content);

    void onAutoCompleteChanged(int state);
};

#endif // SPECIFICATIONSPAGE_H
