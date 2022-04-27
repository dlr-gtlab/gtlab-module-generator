#ifndef INTERFACESPECIFICATIONSWIDGET_H
#define INTERFACESPECIFICATIONSWIDGET_H

#include <QScrollArea>

#include "module_generator_structs.h"

class QGridLayout;
class AbstractClassSpecification;
class ModuleGeneratorSettings;
class FunctionSpecificationWidget : public QScrollArea
{
    Q_OBJECT

public:

    FunctionSpecificationWidget(FunctionStructs const& functions,
                                ModuleGeneratorSettings* settings,
                                QWidget* parent = nullptr);

    int count() const;
    bool isEmpty() const;

    FunctionStructs implementedFunctions();

private:

    FunctionStructs m_functions{};
    ModuleGeneratorSettings* m_settings{};
    QGridLayout* m_baseLayout{};
    QMap<QString, AbstractClassSpecification*> m_specificationWidgets{};

    void setContent();

    void setStandardImplementation(FunctionStruct& function);

    AbstractClassSpecification* setSpecificationWidget(const FunctionStruct& function);
};

#endif // INTERFACESPECIFICATIONSWIDGET_H
