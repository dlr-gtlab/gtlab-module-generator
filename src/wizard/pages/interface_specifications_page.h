#ifndef INTERFACESPEZIFICATIONPAGE_H
#define INTERFACESPEZIFICATIONPAGE_H


#include "abstract_wizardpage.h"

struct ClassStruct;
class QLabel;
class QGridLayout;
class QTabWidget;
class InterfaceSpecificationsPage : public AbstractWizardPage
{
    Q_OBJECT

    static const char* C_TITLE_INTERFACE_SPECIFICATIONS_PAGE;

    static const QString S_INFO_TEXT;

public:

    InterfaceSpecificationsPage(ModuleGeneratorSettings* settings,
                                QWidget* parent = nullptr);

protected:

    void initializePage() override;

    bool isComplete() const override;

    bool validatePage() override;

private:

    QStringList m_activeInterfaces{};

    QTabWidget* m_interfaceTabBar{};

    void intInterfaces();

    void clearInterfaceTabs();

    void setInterfaceImplementation();

    bool checkInterfaceData();

    bool checkClassImplementation(ClassStruct const& base,
                                  QStringList& classNames);

    bool checkDuplicateClassNames(QStringList const& classNames);
};

#endif // INTERFACESPEZIFICATIONPAGE_H