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

    static const char* C_INTERFACE_PAGE_TITLE;

    static const QString S_INFO_TEXT;
    static const QString S_INTERFACE_LABEL;

public:

    InterfaceSelectionPage(ModuleGeneratorSettings* settings,
                           QWidget* parent = nullptr);

    ClassDataList selectedInterfaces() const;

protected:

    void initializePage() override;

    bool isComplete() const override;

    bool validatePage() override;

private:

    WidgetListView* m_widgetListView{};

    void initInterfaces();
};

#endif // INTERFACESELECTIONPAGE_H
