#ifndef ABSTRACTCLASSSPECIFICATIONWIDGET_H
#define ABSTRACTCLASSSPECIFICATIONWIDGET_H

#include "module_generator_structs.h"
#include "module_generator_logger.h"

class AbstractTypeSpecifier
{

public:

    AbstractTypeSpecifier(ImplementationData implementation) :
        m_implementation(std::move(implementation)) { }

    virtual ~AbstractTypeSpecifier() = default;

    ImplementationData functionImplementation()
    {
        m_implementation.lines = this->implementationCode();
        m_implementation.includes += this->additionalIncludes();
        m_implementation.derivedClasses = this->derivedClasses();
        m_implementation.linkedClasses  = this->linkedClasses();

        return m_implementation;
    }

    virtual QStringList implementationCode() = 0;
    virtual QStringList additionalIncludes() { return {}; }
    virtual ClassDataList derivedClasses() { return {}; }
    virtual ClassDataList linkedClasses() { return {}; }

private:

    ImplementationData m_implementation;
};

#endif // ABSTRACTCLASSSPECIFICATIONWIDGET_H
