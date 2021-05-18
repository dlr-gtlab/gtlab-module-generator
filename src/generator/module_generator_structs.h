#ifndef MODULEGENERATOR_STRUCTS_H
#define MODULEGENERATOR_STRUCTS_H

#include <QList>

class QString;
struct ImplementationStruct;
struct ClassStruct;
struct FunctionStruct;

struct DependencieStruct
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

struct FunctionStruct
{
    QString name;
    QString returnValue;
    QString parameter;
    QString qualifier;
    QString description;
    ClassStruct* baseClass = Q_NULLPTR;
    ImplementationStruct& implementation;

    FunctionStruct(ImplementationStruct& imp) : implementation(imp) {}
    ~FunctionStruct();
};

struct ClassStruct
{
    QString className;
    QString fileName;
    QString objectName;
    QString outputPath;
    QList<FunctionStruct*> functions;

    ~ClassStruct();
};

using DependencieStructs = QList<DependencieStruct>;
using IdentifierPairs    = QList<IdentifierPair>;
using FunctionStructsPtr = QList<FunctionStruct*>;
using ClassStructsPtr    = QList<ClassStruct*>;
using ClassStructs       = QList<ClassStruct>;



#endif // MODULEGENERATOR_STRUCTS_H
