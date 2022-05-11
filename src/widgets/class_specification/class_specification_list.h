#ifndef CLASSSPECIFICATIONLIST_H
#define CLASSSPECIFICATIONLIST_H

#include "abstract_type_specifier.h"

#include <QWidget>

class QVBoxLayout;
class QPushButton;
class WidgetListView;
class ModuleGeneratorSettings;
class ClassSpecificationList : public QWidget,
                               public AbstractTypeSpecifier
{
    Q_OBJECT

    static const int I_LIST_HEIGTH;

    static const QString S_ADD_CLASS_BTN;

public:

    ClassSpecificationList(FunctionData const& function,
                                     ModuleGeneratorSettings* settings,
                                     QWidget* parent = nullptr);

    QStringList implementationCode() override;
    QStringList additionalIncludes() override;
    ClassDataList derivedClasses() override;
    ClassDataList linkedClasses() override;

private:

    FunctionData m_functionData;
    ModuleGeneratorSettings* m_settings{};

    QPushButton* m_addButton{};
    WidgetListView* m_widgetListView{};

private slots:

    void onAddButtonPressed();
};

#endif // CLASSSPECIFICATIONLIST_H
