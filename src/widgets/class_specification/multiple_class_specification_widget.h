#ifndef MULTIPLECLASSSPECIFICATIONWIDGET_H
#define MULTIPLECLASSSPECIFICATIONWIDGET_H

#include "abstract_class_specification_widget.h"

#include <QWidget>

class QVBoxLayout;
class QPushButton;
class WidgetListView;
class SingleClassSpecificationWidget;
struct FunctionStruct;
class ModuleGeneratorSettings;
class MultipleClassSpecificationWidget :
        public QWidget,
        public AbstractClassSpecification
{
    Q_OBJECT

public:

    MultipleClassSpecificationWidget(FunctionStruct& function,
                                     ModuleGeneratorSettings* settings,
                                     QWidget* parent = nullptr);
private:    

    FunctionStruct& m_functionStruct;
    ModuleGeneratorSettings* m_settings;

    QPushButton* m_addButton;
    WidgetListView* m_widgetListView;
    QVBoxLayout* m_baseLayout;

protected:

    QStringList implementationValues() override;
    ClassStructs derivedClasses() override;

private slots:

    void onAddButtonPressed();

    void onWidgetDeleted(SingleClassSpecificationWidget*);
};

#endif // MULTIPLECLASSSPECIFICATIONWIDGET_H
