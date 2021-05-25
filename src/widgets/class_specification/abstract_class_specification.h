#ifndef ABSTRACTCLASSSPECIFICATIONWIDGET_H
#define ABSTRACTCLASSSPECIFICATIONWIDGET_H

#include "module_generator_structs.h"

class AbstractClassSpecification
{

public:

    AbstractClassSpecification(ImplementationStruct& imp)
        : m_implementation(imp) { }

    virtual ~AbstractClassSpecification() { }

    void setImplementation()
    {
        QStringList list     = this->implementationValues();
        ClassStructs classes = this->derivedClasses();

        m_implementation.values = list;
        m_implementation.derivedClasses = classes;
    }

protected:

    virtual QStringList implementationValues() = 0;
    virtual ClassStructs derivedClasses() { return ClassStructs(); }

private:

    ImplementationStruct& m_implementation;
};

#endif // ABSTRACTCLASSSPECIFICATIONWIDGET_H
