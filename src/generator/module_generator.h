/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#ifndef MODULEGENERATOR_H
#define MODULEGENERATOR_H

#include "module_generator_settings.h"

#include <QObject>
#include <QString>
#include <QDir>
#include <memory>

struct ClassData;
struct FunctionData;
struct ImplementationData;

class ModuleGeneratorSettings;
class ModuleGenerator : public QObject
{
    Q_OBJECT

    // default ctor
    static const ConstructorData G_CONSTRUCTOR_DEFAULT;

public:

    const static QString S_ID_SIGNATURE;
    const static QString S_ID_GENERATOR_VERSION;
    // current year
    const static QString S_ID_YEAR;
    // current date
    const static QString S_ID_DATE;

    /// include for header file
    const static QString S_ID_INCLUDE_FILE;
    /// qt inlcude (for <...> styled includes)
    const static QString S_ID_QT_INCLUDE_FILE;
    /// additional include (for "..." styled includes)
    const static QString S_ID_ADD_INCLUDE_FILE;
    /// forward declarations
    const static QString S_ID_CLASS_FORWARD_DECL;

    const static QString S_ID_BASE_CLASS;
    const static QString S_ID_BASE_FILE_NAME;
    const static QString S_ID_DERIVE_BASE_CLASS;

    const static QString S_ID_PREFIX;
    const static QString S_ID_CLASS_NAME;
    const static QString S_ID_OBJECT_NAME;
    const static QString S_ID_FILE_NAME;
    const static QString S_ID_HEADER_NAME;
    const static QString S_ID_CONSTRUCTOR;
    const static QString S_ID_CONSTRUCTOR_INIT_LIST;
    const static QString S_ID_FUNCTION;
    const static QString S_ID_IMPLEMENTATION;

    const static QString S_ID_MODULE_NAME;
    const static QString S_ID_MODULE_DESCRIPTION;
    const static QString S_ID_MODULE_VERSION;
    const static QString S_ID_INTERFACE_MACRO;
    const static QString S_ID_GTLAB_MAJOR_VERSION;

    const static QString S_ID_GTLAB_INSTALL_DIR;
    const static QString S_ID_GTLAB_INSTALL_BIN_DIR;
    const static QString S_ID_DEVTOOLS_INSTALL_DIR;
    const static QString S_ID_TARGET_DIR_NAME;

    const static QString S_ID_PRO_INCLUDE_PATH;
    const static QString S_ID_PRO_HEADER_PATH;
    const static QString S_ID_PRO_SOURCE_PATH;
    /// link release libs
    const static QString S_ID_PRO_LIBS;
    /// link debug libs
    const static QString S_ID_PRO_LIBS_D;

    const static QString S_ID_AUTHOR;
    const static QString S_ID_AUTHOR_EMAIL;

    /// for nicer indent of method parameters
    const static QString S_ID_INDENT;

    const static QString S_ID_CMAKE_SOURCE_FILES;
    const static QString S_ID_CMAKE_ADDITIONAL_FILES;
    const static QString S_ID_CMAKE_MODULE_ALIAS;
    const static QString S_ID_CMAKE_INCLUDE_DIRS;
    const static QString S_ID_CMAKE_FIND_PACKAGE_DEPS;
    const static QString S_ID_CMAKE_TARGET_LINK_LIBRARIES;

    /**
     * @brief Applies the default values for certain indentifiers.
     * @param identifierPairs
     */
    void registerDefaults(IdentifierPairs& identifierPairs);

    /**
     * @brief Getter for settings class
     * @return Settings
     */
    ModuleGeneratorSettings const* settings() const;
    ModuleGeneratorSettings* settings();

signals:

    /**
     * @brief Emitted once generation has finished
     */
    void generationFinished();

public slots:

    /**
     * @brief Main function to run module generation
     * @return
     */
    bool generate();

private:

    /// Helper struct to "hide" cmake specific helper functions
    struct CMake;
    struct QMake;

    /// settings
    ModuleGeneratorSettings m_settings{};

    /// helper struct that holds data of a generated file
    struct GeneratedSourceFile
    {
        QString fileName;
        QString dir;
        bool headerOnly = false;
    };

    /// list of all generated source files
    QList<GeneratedSourceFile> m_generatedSourceFiles{};
    /// src directory in which the source files are generated
    QDir m_srcDir{};
    /// main module directory
    QDir m_moduleDir{};

    /**
     * @brief Internal helper function that coordinates the module generation
     * and calls subgroutines
     * @return Success
     */
    bool generateHelper();

    /**
     * @brief Generates all necessary directories for the module geneations
     * @return Success
     */
    bool generateModuleDirs();

    /**
     * @brief Generates the unittest file structure
     * @return Success
     */
    bool generateUnittestStructure();

    /**
     * @brief Generates the project files of the module
     * @return Success
     */
    bool generateModuleProjectFiles();

    /**
     * @brief Generates qmake-specific project files
     * @return Success
     */
    bool generateQMakeFiles();

    /**
     * @brief Generates cmake-specific project files
     * @return Success
     */
    bool generateCMakeFiles();

    /**
     * @brief Generates git specific files
     * @return Success
     */
    bool generateGitFiles();

    /**
     * @brief Generates the json file of the module
     * @return Success
     */
    bool generateModuleMetaFile();

    /**
     * @brief Generates all source files (.cpp and .h) of the module
     * @return Success
     */
    bool generateModuleFiles();

    /**
     * @brief Helper function that coordinates the generation of a c++ function
     * in the target module. This includes the function signature,
     * function body, classes used and includes required.
     * @param headerString Header string to implement function in
     * @param sourceString Source string to implement function in
     * @param function Function struct containing all necessary information
     * @param isConstructor Whether the function is a constructor
     */
    void generateFunction(QString& headerString,
                          QString& sourceString,
                          FunctionData const& function,
                          bool isConstructor = false);

    /**
     * @brief Helper function that generates the function body. Also generates
     * the incldues required as well as classes used.
     * @param headerString Header string to implement function in
     * @param sourceString Source string to implement function in
     * @param function Function struct containing all necessary information
     * @param isConstructor Whether the function is a constructor
     */
    void generateImplementation(QString& headerString,
                                QString& sourceString,
                                FunctionData const& function,
                                bool isConstructor = false);

    /**
     * @brief Helper function that generates the given classes, derived of the
     * base class and implements the required include in the source file where
     * it is used
     * @param sourceString Source string where the classes are used
     * @param baseClass Base class of which the classes are derived
     * @param classes Classes to generate
     */
    void generateBasicClassHelper(QString& sourceString,
                                  ClassData const& baseClass,
                                  QList<ClassData> const& classes);

    /**
     * @brief Helper function that cooridantes the generation of a class
     * @param base Base class of which the class is derived
     * @param derived Derived class to generate
     */
    void generateBasicClass(ClassData const& base,
                            ClassData const& derived);

    /**
     * @brief Helper function that appends the includes to the given source file
     * @param sourceString Source string to append includes to
     * @param includes Includes to generate
     */
    void generateIncludes(QString& sourceString,
                          QStringList includes);

    /**
     * @brief Helper function that appends the forward declarations to the given
     * header file
     * @param headerString Header string to append declarations to
     * @param forwardDecls Forward declarations to generate
     */
    void generateForwardDeclarations(QString& headerString,
                                     QStringList const& forwardDecls);

    /**
     * @brief Helper function that generates the constructor of a class
     * @param headerString Header string to generate constructor declaration in
     * @param sourceString Source string to generate constructor implementation
     * in
     * @param base Class struct which contains the constructors to generate
     * @param derivedName Name of class for which the constructor should be
     * generated for
     */
    void generateConstructors(QString& headerString,
                              QString& sourceString,
                              ClassData const& base,
                              QString const& derivedName);

    /**
     * @brief Appends the file to the list of generated source files
     * @param fileName Name of the file that were generated
     * (base name, no suffix)
     * @param dir Directory relative to the src directory in which the file
     * was generated in
     * @param headerOnly Whether only a header file was generated
     */
    void appendGeneratedSourceFile(QString const& fileName,
                                   QString const& dir = {},
                                   bool headerOnly = false);

    /**
     * @brief Returns a list of all source files. List does not contain
     * duplicate entries
     * @return Generated source files
     */
    QStringList generatedSourceFiles() const;

    /**
     * @brief Returns a list of all include paths. List does not contain
     * duplicate entries
     * @return Generated include paths
     */
    QStringList generatedIncludePaths() const;

    /**
     * @brief Clears all identifiers, applies standardized formatting and removes
     * compatibility code snippets.
     * @param settings Module generator settings
     * @param fileString File string to clear
     */
    void clearFileString(QString& fileString);

    enum ApplyTemplateFlags
    {
        OverwriteFiles = 1 << 0,
        Recursive      = 1 << 1
    };

    /**
     * @brief Applies a directory template to the target destination. For each
     * file the indentifiers are replaced.
     * @param originDirectory Source Directory (e.g. resource path)
     * @param destinationDirectory Destination Directory (e.g. module path)
     * @param identifierPairs Identifiers to replace.
     * @param flags Flags, e.g. to apply template recursively
     * @param filter Filter regexp used to
     */
    bool applyTemplate(QDir const& originDirectory,
                       QDir const& destinationDirectory,
                       IdentifierPairs const& identifierPairs,
                       int flags = OverwriteFiles | Recursive,
                       QRegularExpression const& filter = {});
};

#endif // MODULEGENERATOR_H
