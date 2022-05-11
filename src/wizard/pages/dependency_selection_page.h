#ifndef DEPENDENCYSELECTIONPAGE_H
#define DEPENDENCYSELECTIONPAGE_H

#include "abstract_wizardpage.h"

#include "module_generator_structs.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;
class ModuleGeneratorSettings;
class WidgetListView;

class DependencySelectionPage : public AbstractWizardPage
{
    Q_OBJECT

    static const char* C_DEPENDENCY_PAGE_TITLE;

    static const QString S_INFO_TEXT;
    static const QString S_DEPENDENCY_BTN_TEXT;
    static const QString S_DEPENDENCY_ADD_LABEL;
    static const QString S_DEPENDENCY_LABEL;

public:

    DependencySelectionPage(ModuleGeneratorSettings* settings,
                            QWidget* parent = nullptr);

    DependencyDataList selectedDependencies() const;

protected:

    void initializePage() override;

    bool isComplete() const override;

    bool validatePage() override;

private:

    QLabel* m_resolveStatusLabel{};
    /// add dependency edit
    QLineEdit* m_addDependencyEdit{};
    /// add dependency push button
    QPushButton* m_addDependencyPushBtn{};

    WidgetListView* m_widgetListView{};

    void addStandardDependencies();

    void addDependency(const DependencyStruct& name, bool isEditable = true);

private slots:

    /**
     * @brief onAddDependencyBtnPressed - adds a deleteable dependency to
     * dependency layout, if name of dependency is not empty nor a duplicate
     */
    void onAddDependencyBtnPressed();

    void onEditedDependcyEdit(QString content);
};

#endif // DEPENDENCYSELECTIONPAGE_H
