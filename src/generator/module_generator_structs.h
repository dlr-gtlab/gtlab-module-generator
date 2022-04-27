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

/// Data struct fr dependencies
struct DependencyStruct
{
    QString name{};
    QString version{};
};

/// Data struct for signature
struct AuthorDetails
{
    QString name{};
    QString email{};
};

/// Data struct representing an identifier pair
struct IdentifierPair
{
    QString identifier{};
    QString value{};
};

inline bool operator==(IdentifierPair const& first,
                IdentifierPair const& other)
{
    return (first.identifier == other.identifier &&
            first.value == other.value);
}

/// Data struct that holds the module specifications
struct ModuleClass
{
    QString ident{};
    QString className{};
    QString fileName{};
    QString description{};
    QString version{};
};

struct ImplementationStruct
{
    QStringList values{};
    QStringList includes{};
    QStringList forwardDeclarations{};
    QList<ClassStruct> derivedClasses{};
    QList<ClassStruct> linkedClasses{};
};

struct Constructor
{
    QString parameter{};
    QStringList implementation{};
};

struct ClassStruct
{
    QString className{};
    QString fileName{};
    QString objectName{};
    QString outputPath{};
    QList<Constructor> constructors{};
    QList<FunctionStruct> functions{};

    bool isValid() const
    {
        return !className.isEmpty() && !fileName.isEmpty();
    }
};

struct FunctionStruct
{
    QString name{};
    QString returnValue{};
    QString parameter{};
    QString qualifier{};
    QString description{};
    QString tooltip{};
    ClassStruct baseClass{};
    ClassStruct linkedClass{};
    ImplementationStruct implementation{};

    bool isValid() const
    {
        return !returnValue.isEmpty() && !name.isEmpty();
    }
};

using DependencyStructs = QList<DependencyStruct>;
using IdentifierPairs   = QList<IdentifierPair>;
using FunctionStructs   = QList<FunctionStruct>;
using ClassStructs      = QList<ClassStruct>;
using Constructors      = QList<Constructor>;



#endif // MODULEGENERATOR_STRUCTS_H
