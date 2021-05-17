#ifndef INTERFACESPEZIFICATIONPAGE_H
#define INTERFACESPEZIFICATIONPAGE_H


#include "abstract_wizardpage.h"

class QLabel;
class QGridLayout;
class QTabWidget;
class InterfaceSpecificationsPage : public AbstractWizardPage
{
    Q_OBJECT

public:

    InterfaceSpecificationsPage(ModuleGeneratorSettings* settings,
                               QWidget* parent = nullptr);

protected:

    void initializePage() Q_DECL_OVERRIDE;

    bool isComplete() const Q_DECL_OVERRIDE;

    bool validatePage() Q_DECL_OVERRIDE;

    void cleanupPage() Q_DECL_OVERRIDE;

private:

    /// info text label
    QLabel* m_infoTextLabel;
    /// grid layout
    QGridLayout* m_baseLayout;

    QTabWidget* m_interfaceTabBar;

    void setSelectedInterfaces();

    void clearInterfaceTabs();

    void setInterfaceImplementation();

    bool checkInterfaceData();
};

#endif // INTERFACESPEZIFICATIONPAGE_H
