#ifndef ABSTRACTCLASSSPECIFICATIONWIDGET_H
#define ABSTRACTCLASSSPECIFICATIONWIDGET_H

#include "module_generator_structs.h"
#include "module_generator_logger.h"

class AbstractClassSpecification
{

public:

    AbstractClassSpecification(ImplementationStruct const& implementation) :
        m_implementation(implementation) { }

    virtual ~AbstractClassSpecification() { }

    ImplementationStruct functionImplementation()
    {
        m_implementation.values = this->implementationValues();
        m_implementation.derivedClasses = this->derivedClasses();
        m_implementation.linkedClasses  = this->linkedClasses();

        return m_implementation;
    }

    virtual QStringList implementationValues() = 0;
    virtual ClassStructs derivedClasses() { return {}; }
    virtual ClassStructs linkedClasses() { return {}; }

private:

    ImplementationStruct m_implementation;
};

#endif // ABSTRACTCLASSSPECIFICATIONWIDGET_H
