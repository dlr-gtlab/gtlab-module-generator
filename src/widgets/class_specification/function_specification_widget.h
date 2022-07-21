#ifndef FUNCTIONSPECIFICATIONWIDGET_H
#define FUNCTIONSPECIFICATIONWIDGET_H

#include <QScrollArea>
#include <QMap>

#include "module_generator_structs.h"

class QGridLayout;
class AbstractTypeSpecificationItem;
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
    QMap<QString, AbstractTypeSpecificationItem*> m_specificationWidgets{};

    void setContent();
};

#endif // FUNCTIONSPECIFICATIONWIDGET_H
