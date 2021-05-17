#ifndef INTERFACESPECIFICATIONSWIDGET_H
#define INTERFACESPECIFICATIONSWIDGET_H

//#include <QWidget>
#include <QScrollArea>

#include "module_generator_structs.h"

class QGridLayout;
class AbstractClassSpecification;
class ModuleGeneratorSettings;
//class FunctionSpecificationWidget : public QWidget
class FunctionSpecificationWidget : public QScrollArea
{
    Q_OBJECT

public:

    FunctionSpecificationWidget(FunctionStructsPtr functions,
                                ModuleGeneratorSettings* settings,
                                QWidget* parent = nullptr);

    int count() const;
    bool isEmpty() const;

    void setImplementation();

private:

    FunctionStructsPtr m_functions;
    ModuleGeneratorSettings* m_settings;
    QWidget* m_scrollWidget;
    QGridLayout* m_baseLayout;
    QList<AbstractClassSpecification*> m_widgets;

    void setContent();

    void setStandardImplementation(FunctionStruct& function);

    AbstractClassSpecification* setSpecificationWidget(FunctionStruct& function);
};

#endif // INTERFACESPECIFICATIONSWIDGET_H
