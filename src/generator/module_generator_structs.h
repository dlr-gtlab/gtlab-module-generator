#ifndef MODULEGENERATOR_STRUCTS_H
#define MODULEGENERATOR_STRUCTS_H

#include <QList>

class QString;
struct ModuleData;
struct AuthorDetails;
struct ImplementationData;
struct ConstructorData;
struct ClassData;
struct FunctionData;

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
struct ModuleData
{
    QString ident{};
    QString className{};
    QString fileName{};
    QString description{};
    QString version{};
};

/// Implementation data for function struct
struct ImplementationData
{
    /// lines in the cpp function. Last line is the return statement
    QStringList lines{};
    /// required includes in the cpp file
    QStringList includes{};
//    /// required includes in the h file
//    QStringList headerIncludes{};
    /// required declarations in header file
    QStringList forwardDeclarations{};
    /// list of derived classes (subclasses of base class in function struct)
    QList<ClassData> derivedClasses{};
    /// list of linked classes (subclasses of linked class in function struct)
    QList<ClassData> linkedClasses{};
};

struct ConstructorData
{
    /// parameters
    QStringList parameters{};
    /// implementation lines
    QStringList implementation{};
    /// initializer list
    QStringList initilizerList{};
};

/// represents an interface or a user defined class (exporter, calc, task etc.)
struct ClassData
{
    /// class name
    QString className{};
    /// file name
    QString fileName{};
    /// object name for class or display name for interface
    QString objectName{};
    /// outputpath relative to root dir of module (e.g. 'data/property')
    QString outputPath{};
    /// tooltip in interface selection
    QString tooltip{};
    /// custom constructors, keep empty for default
    QList<ConstructorData> constructors{};
    /// functions to implement
    QList<FunctionData> functions{};

    bool isValid() const
    {
        return !className.isEmpty() && !fileName.isEmpty();
    }
};

struct FunctionData
{
    /// function anme
    QString name{};
    /// return type
    QString returnType{};
    /// parameters
    QStringList parameters{};
    /// const function
    bool isConst{};
    /// is protected
    bool isProtected{};
    /// doxygen description
    QString description{};
    /// tooltip in input mask
    QString tooltip{};
    /// denotes the base class of the meta object (if func return a single
    /// or multiple meta objects)
    ClassData baseClass{};
    /// denotes the linked class (linked class is the first template
    /// parameter in QMap<const char*, QMetaObject>)
    ClassData linkedClass{};
    /// implementation data
    ImplementationData implementation{};

    bool isValid() const
    {
        return !returnType.isEmpty() && !name.isEmpty();
    }
};

using DependencyDataList = QList<DependencyStruct>;
using IdentifierPairs    = QList<IdentifierPair>;
using FunctionDataList   = QList<FunctionData>;
using ClassDataList      = QList<ClassData>;
using Constructors       = QList<ConstructorData>;

#endif // MODULEGENERATOR_STRUCTS_H
