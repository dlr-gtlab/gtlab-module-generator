#ifndef MODULEGENERATOR_STRUCTS_H
#define MODULEGENERATOR_STRUCTS_H

#include <QList>

class QString;
struct ImplementationStruct;
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
};

struct ClassStruct
{
    QString className;
    QString fileName;
    QString objectName;
    QString outputPath;
    QList<FunctionStruct> functions;

    bool isValid() const
    {
        return !className.isEmpty() && !fileName.isEmpty();
    }
//    ~ClassStruct();
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
    ImplementationStruct implementation;

    bool isValid() const
    {
        return !returnValue.isEmpty() && !name.isEmpty();
    }

//    ~FunctionStruct();
};
using DependencyStructs = QList<DependencyStruct>;
using IdentifierPairs    = QList<IdentifierPair>;
using FunctionStructsPtr = QList<FunctionStruct*>;
using FunctionStructs    = QList<FunctionStruct>;
using ClassStructsPtr    = QList<ClassStruct*>;
using ClassStructs       = QList<ClassStruct>;



#endif // MODULEGENERATOR_STRUCTS_H
