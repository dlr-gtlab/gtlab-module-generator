#ifndef ABSTRACTTYPESPECIFICATIONITEM_H
#define ABSTRACTTYPESPECIFICATIONITEM_H

#include "module_generator_structs.h"
#include "module_generator_logger.h"

class ModuleGeneratorSettings;
class AbstractTypeSpecificationItem
{

public:

    const static QString S_ID_VALUE;
    const static QString S_ID_LINKED_VALUE;
    const static QString S_ID_VARIABLE_NAME;
    const static QString S_ID_RETURN_TYPE;

    AbstractTypeSpecificationItem(ImplementationData implementation,
                                  TypeInfo typeInfo);

    virtual ~AbstractTypeSpecificationItem() = default;

    ImplementationData functionImplementation();

    virtual QStringList codeImplementation() = 0;
    virtual QStringList additionalIncludes() { return m_typeInfo.includes; }
    virtual ClassDataList derivedClasses() { return {}; }
    virtual ClassDataList linkedClasses() { return {}; }

    TypeInfo const& typeInfo() const { return m_typeInfo; }

private:

    ImplementationData m_implementation;
    TypeInfo m_typeInfo;
};

/**
 * @brief Creates and returns an type sepcification item (can be casted
 * to QWidget). May return null if no specification item was found. In this case
 * implementation and includes are updated
 * @param settings Module generator settings
 * @param function Function Data
 * @param inputType Input Type of function
 * @return sepcification item or null
 */
AbstractTypeSpecificationItem* generateFunctionSpecification(
        ModuleGeneratorSettings* settings,
        FunctionData& function,
        QString const& inputType);

#endif // ABSTRACTTYPESPECIFICATIONITEM_H
