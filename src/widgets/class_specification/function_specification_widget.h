#ifndef FUNCTIONSPECIFICATIONWIDGET_H
#define FUNCTIONSPECIFICATIONWIDGET_H

#include <QScrollArea>

#include "module_generator_structs.h"

class QGridLayout;
class AbstractTypeSpecifier;
class ModuleGeneratorSettings;
class FunctionSpecificationWidget : public QScrollArea
{
    Q_OBJECT

public:

    FunctionSpecificationWidget(FunctionDataList functions,
                                ModuleGeneratorSettings* settings,
                                QWidget* parent = nullptr);

    int count() const;
    bool isEmpty() const;

    FunctionDataList implementedFunctions();

private:

    FunctionDataList m_functions{};
    ModuleGeneratorSettings* m_settings{};
    QGridLayout* m_baseLayout{};
    QMap<QString, AbstractTypeSpecifier*> m_specificationWidgets{};

    void setContent();

    void generateStdImplementation(FunctionData& function);

    AbstractTypeSpecifier* getSpecificationWidget(const FunctionData& function);
};

#endif // FUNCTIONSPECIFICATIONWIDGET_H
