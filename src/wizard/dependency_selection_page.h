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


public:


    DependencySelectionPage(ModuleGeneratorSettings* settings,
                            QWidget* parent = nullptr);

    DependencyStructs selectedDependencies() const;

protected:

    void initializePage() Q_DECL_OVERRIDE;

    bool isComplete() const Q_DECL_OVERRIDE;

    bool validatePage() Q_DECL_OVERRIDE;

private:

    /// info text label
    QLabel* m_infoTextLabel;
    /// add dependency label
    QLabel* m_addDependencyLabel;
    /// add dependency edit
    QLineEdit* m_addDependencyEdit;
    /// add dependency push button
    QPushButton* m_addDependencyPushBtn;
    /// dependency label
    QLabel* m_dependenciesLabel;
    /// grid layout
    QGridLayout* m_baseLayout;

    WidgetListView* m_widgetListView;


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
