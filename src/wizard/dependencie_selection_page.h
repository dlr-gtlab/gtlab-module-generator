#ifndef DEPENDENCIESELECTIONPAGE_H
#define DEPENDENCIESELECTIONPAGE_H

#include "abstract_wizardpage.h"

#include "module_generator_structs.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;
class DependencieWidget;
class ModuleGeneratorSettings;
class WidgetListView;

class DependencieSelectionPage : public AbstractWizardPage
{
    Q_OBJECT


public:


    DependencieSelectionPage(ModuleGeneratorSettings* settings,
                             QWidget* parent = nullptr);

    DependencieStructs selectedDependencies() const;

protected:

    void initializePage() Q_DECL_OVERRIDE;

    bool isComplete() const Q_DECL_OVERRIDE;

    bool validatePage() Q_DECL_OVERRIDE;

private:

    /// info text label
    QLabel* m_infoTextLabel;
    /// add dependencie label
    QLabel* m_addDependencieLabel;
    /// add dependencie edit
    QLineEdit* m_addDependencieEdit;
    /// add dependencie push button
    QPushButton* m_addDependenciePushBtn;
    /// dependencie label
    QLabel* m_dependenciesLabel;
    /// grid layout
    QGridLayout* m_baseLayout;

    WidgetListView* m_widgetListView;


    void addStandardDependencies();

    void addDependencie(const DependencieStruct& name, bool isEditable = true);


private slots:

    /**
     * @brief onAddDependenciePushBtnPressed - adds a deleteable dependencie to
     * dependencie layout, if name of dependencie is not empty nor a duplicate
     */
    void onAddDependencieBtnPressed();

    void onEditedDependcieEdit(QString content);
};

#endif // DEPENDENCIESELECTIONPAGE_H
