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

    FunctionSpecificationWidget(const FunctionStructs& functions,
                                ModuleGeneratorSettings* settings,
                                QWidget* parent = nullptr);

    int count() const;
    bool isEmpty() const;

    FunctionStructs implementedFunctions();

private:

    FunctionStructs m_functions;
    ModuleGeneratorSettings* m_settings;
    QWidget* m_scrollWidget;
    QGridLayout* m_baseLayout;
    QMap<QString, AbstractClassSpecification*> m_specificationWidgets;

    void setContent();

    void setStandardImplementation(FunctionStruct& function);

    AbstractClassSpecification* setSpecificationWidget(const FunctionStruct& function);
};

#endif // INTERFACESPECIFICATIONSWIDGET_H
