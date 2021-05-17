#ifndef INTERFACESELECTIONPAGE_H
#define INTERFACESELECTIONPAGE_H


#include "abstract_wizardpage.h"
#include "module_generator_structs.h"

class QLabel;
class WidgetListView;
class QGridLayout;
class InterfaceSelectionWidget;

class InterfaceSelectionPage : public AbstractWizardPage
{
    Q_OBJECT

public:

    InterfaceSelectionPage(ModuleGeneratorSettings* settings,
                           QWidget* parent = nullptr);

    ClassStructsPtr selectedInterfaces() const;

protected:

    void initializePage() Q_DECL_OVERRIDE;

    bool isComplete() const Q_DECL_OVERRIDE;

    bool validatePage() Q_DECL_OVERRIDE;

private:

    /// info text label
    QLabel* m_infoTextLabel;
    /// interface label
    QLabel* m_interfaceLabel;
    /// grid layout
    QGridLayout* m_gridLayout;

    WidgetListView* m_widgetListView;


    void setInterfaces();
};

#endif // INTERFACESELECTIONPAGE_H
