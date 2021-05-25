#ifndef MULTIPLECLASSSPECIFICATIONWIDGET_H
#define MULTIPLECLASSSPECIFICATIONWIDGET_H

#include "abstract_class_specification.h"

#include <QWidget>

class QVBoxLayout;
class QPushButton;
class WidgetListView;
class ModuleGeneratorSettings;
class MultipleClassSpecificationWidget :
        public QWidget,
        public AbstractClassSpecification
{
    Q_OBJECT

public:

    MultipleClassSpecificationWidget(const FunctionStruct& function,
                                     ModuleGeneratorSettings* settings,
                                     QWidget* parent = nullptr);

    QStringList implementationValues() override;
    ClassStructs derivedClasses() override;

private:    

    FunctionStruct m_functionStruct;
    ModuleGeneratorSettings* m_settings;

    QPushButton* m_addButton;
    WidgetListView* m_widgetListView;
    QVBoxLayout* m_baseLayout;

private slots:

    void onAddButtonPressed();
};

#endif // MULTIPLECLASSSPECIFICATIONWIDGET_H
