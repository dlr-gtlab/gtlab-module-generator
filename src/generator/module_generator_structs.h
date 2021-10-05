#ifndef MODULEGENERATOR_STRUCTS_H
#define MODULEGENERATOR_STRUCTS_H

#include <QList>

class QString;
struct ModuleClass;
struct AuthorDetails;
struct ImplementationStruct;
struct Constructor;
struct ClassStruct;
struct FunctionStruct;

struct DependencyStruct
{
    QString name;
    QString version;
};

struct IdentifierPair
{
    QString identifier;
    QString value;

    bool operator==(const IdentifierPair& other)
    {
        return (other.identifier == this->identifier &&
                other.value == this->value);
    }
};

struct ModuleClass
{
    QString ident;
    QString className;
    QString fileName;
    QString description;
    QString version;
};

struct AuthorDetails
{
    QString name;
    QString email;
};

struct ImplementationStruct
{
    QStringList values;
    QStringList includes;
    QStringList forwardDeclarations;
    QList<ClassStruct> derivedClasses;
    QList<ClassStruct> linkedClasses;
};

struct Constructor
{
    QString parameter;
    QStringList implementation;
};

const Constructor G_CONSTRUCTOR_DEFAULT =
        Constructor({ QString(), QStringList() <<
                      QStringLiteral("setObjectName(\"$$OBJECT_NAME$$\")")
                    });

struct ClassStruct
{
    QString className;
    QString fileName;
    QString objectName;
    QString outputPath;
    QList<Constructor> constructors;
    QList<FunctionStruct> functions;

    bool isValid() const
    {
        return !className.isEmpty() && !fileName.isEmpty();
    }
};

struct FunctionStruct
{
    QString name;
    QString returnValue;
    QString parameter;
    QString qualifier;
    QString description;
    QString tooltip;
    ClassStruct baseClass;
    ClassStruct linkedClass;
    ImplementationStruct implementation;

    bool isValid() const
    {
        return !returnValue.isEmpty() && !name.isEmpty();
    }
};

using DependencyStructs = QList<DependencyStruct>;
using IdentifierPairs   = QList<IdentifierPair>;
using FunctionStructs   = QList<FunctionStruct>;
using ClassStructsPtr   = QList<ClassStruct*>;
using ClassStructs      = QList<ClassStruct>;
using Constructors      = QList<Constructor>;



#endif // MODULEGENERATOR_STRUCTS_H
