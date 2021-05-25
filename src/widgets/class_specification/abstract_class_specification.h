#ifndef ABSTRACTCLASSSPECIFICATIONWIDGET_H
#define ABSTRACTCLASSSPECIFICATIONWIDGET_H

#include "module_generator_structs.h"
#include "module_generator_logger.h"

class AbstractClassSpecification
{

public:

    AbstractClassSpecification(const ImplementationStruct& implementation) :
        m_implementation(implementation) { }

    virtual ~AbstractClassSpecification() { }

    ImplementationStruct functionImplementation()
    {
        m_implementation.values = this->implementationValues();
        m_implementation.derivedClasses = this->derivedClasses();

        return m_implementation;
    }

    virtual QStringList implementationValues() = 0;
    virtual ClassStructs derivedClasses() { return ClassStructs(); }

private:

    ImplementationStruct m_implementation;
};

#endif // ABSTRACTCLASSSPECIFICATIONWIDGET_H
