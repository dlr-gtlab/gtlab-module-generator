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

    static const char* C_TITLE_SPECIFICATIONS_PAGE;

    static const QString S_INFO_TEXT;
    static const QString S_MODULE_PREFIX_LABEL;
    static const QString S_CLASS_NAME_LABEL;
    static const QString S_FILE_NAME_LABEL;
    static const QString S_MODULE_NAME_LABEL;
    static const QString S_MODULE_VERSION_LABEL;
    static const QString S_MODULE_DESCRIPTION_LABEL;
    static const QString S_AUTO_COMPLETE_LABEL;
    static const QString S_AUTO_GENERATED_TOOLTIP;

public:

    /**
     * @brief ModuleSpecificationpage - constructor
     * @param settings
     * @param parent
     */
    explicit ModuleSpecificationsPage(ModuleGeneratorSettings* settings,
                                      QWidget* parent = nullptr);


protected:

    void initializePage() override;

    bool isComplete() const override;

    bool validatePage() override;

signals:

    void moduleDataChanged();

private:

    /// prefix line edit
    QLineEdit* m_modulePrefixEdit{};

    QLineEdit* m_classNameEdit{};

    QLineEdit* m_fileNameEdit{};
    /// module name line edit
    QLineEdit* m_moduleNameEdit{};
    /// version line edit
    QLineEdit* m_versionEdit{};
    /// description text edit
    QLineEdit* m_descriptionEdit{};

    QCheckBox* m_autoEditCheckBox{};

    bool m_autoEditFileName{false};

    /// prefix validator
    QRegularExpressionValidator* m_prefixValidator{};
    /// module name validator
    QRegularExpressionValidator* m_indentValidator{};
    QRegularExpressionValidator* m_classNameValidator{};
    QRegularExpressionValidator* m_fileNameValidator{};
    QRegularExpressionValidator* m_versionValidator{};

    void autoComplete();

private slots:

    void onEditedPrefix();

    void onEditedModuleName(QString name);

    void onEditedClassName(QString name);

    void onEditedFileName();

    void onEditedVersion(QString content);

    void onAutoCompleteChanged(int state);
};

#endif // SPECIFICATIONSPAGE_H
