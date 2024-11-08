/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

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

static QString S_WIDGET_TYPE_BOOL = QStringLiteral("Bool");
static QString S_WIDGET_TYPE_STRING = QStringLiteral("String");
static QString S_WIDGET_TYPE_COMBOBOX = QStringLiteral("ComboBox");
static QString S_WIDGET_TYPE_CLASS = QStringLiteral("Class");
static QString S_WIDGET_TYPE_CLASSNAME = QStringLiteral("ClassName");
static QString S_WIDGET_TYPE_CLASSLINKED = QStringLiteral("ClassLinked");
static QString S_WIDGET_TYPE_MULTI = QStringLiteral("Multi");

namespace details
{
    /**
     * @brief Helper function to test if version is within acceptable range
     * @param vMin Minimum verison of signature (introduced)
     * @param vMax Maximum verison of signature (deprecated)
     * @param vTarget Target version of GTlab to check against
     * @return is enabled for target version
     */
    bool isEnabledHelper(QString const& vMin, QString const& vMax,
                         QString const& vTarget);
}

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

inline bool operator==(IdentifierPair const& first, IdentifierPair const& other)
{
    return (first.identifier == other.identifier &&
            first.value == other.value);
}

struct TypeInfo
{
    QString returnType{};
    QString widgetType{};
    QStringList includes{};
    QStringList values{};
    QStringList implementation{};

    /**
     * @brief Returns the inner type of 'returnType'. If none was
     * found returns the parameter. Uses Regular Expressions.
     * Mostly necessary to extract the single/inner type of an list input widget
     * E.g.:
     *   QList<(.+?)> -> QList<QMetaObject> -> QMetaObject.
     * @param oReturnType Outer return type to match against
     * @param idx Nth capture group
     * @return inner type if successfull else 'returnType'
     */
    QString innerType(QString const& oReturnType, int idx = 1) const;

    bool isValid() const
    {
        return !returnType.isEmpty();
    }
};

/// Data struct to specify version dependent code
struct VersionData
{
    QString min{}, max{};

    bool isEnabled(QString const& vTarget) const
    {
        return details::isEnabledHelper(min, max, vTarget);
    }
};

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
    /// required forward declarations in header file
    QStringList forwardDeclarations{};

    /// list of derived classes to generate (subclasses of base class
    /// in function struct)
    QList<ClassData> derivedClasses{};
    /// list of linked classes to generate (subclasses of linked class
    /// in function struct)
    QList<ClassData> linkedClasses{};
};

/// Data for generating a constructor
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
    /// object name for class or display name in case of an interface
    QString objectName{};
    /// outputpath relative to root dir of module (e.g. 'data/property')
    QString outputPath{};
    /// tooltip in interface selection
    QString tooltip{};
    /// version info
    VersionData version{};
    /// custom constructors, keep empty for default
    QList<ConstructorData> constructors{};
    /// functions to implement
    QList<FunctionData> functions{};

    bool isValid() const
    {
        return !className.isEmpty() && !fileName.isEmpty();
    }
};

/// represents a method that must be implemented (exporter, calc, task etc.)
struct FunctionData
{
    /// function name
    QString name{};
    /// return type
    QString returnType{};
    /// can be used to override the input widget
    QString inputType{};
    /// parameters
    QStringList parameters{};
    /// const function qualifier
    bool isConst{};
    /// is protected identifier
    bool isProtected{};
    /// wether to show the input widget or not
    bool isHidden{};
    /// doxygen description
    QString description{};
    /// tooltip in input mask
    QString tooltip{};
    /// version info
    VersionData version{};

    /// denotes the base class of the meta object (if function returns
    /// meta objects)
    ClassData baseClass{};
    /// denotes the linked class (if function returns a linked class name
    /// i.e. QMap<const char*, QMetaObject>)
    ClassData linkedClass{};
    /// implementation
    ImplementationData implementation{};

    bool isValid() const
    {
        return !returnType.isEmpty() && !name.isEmpty();
    }
};

using DependencyDataList = QList<DependencyStruct>;
using IdentifierPairs    = QList<IdentifierPair>;
using TypeInfoList       = QList<TypeInfo>;
using FunctionDataList   = QList<FunctionData>;
using ClassDataList      = QList<ClassData>;
using Constructors       = QList<ConstructorData>;

#endif // MODULEGENERATOR_STRUCTS_H
