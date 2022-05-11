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
    const static QString S_ID_INCLUDE_FILE;
    const static QString S_ID_QT_INCLUDE_FILE;
    const static QString S_ID_ADD_INCLUDE_FILE;
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
    const static QString S_ID_GTLAB_INSTALL_SUB_DIR;
    const static QString S_ID_DEVTOOLS_INSTALL_DIR;

    const static QString S_ID_PRO_INCLUDE_PATH;
    const static QString S_ID_PRO_HEADER_PATH;
    const static QString S_ID_PRO_SOURCE_PATH;
    const static QString S_ID_PRO_LIBS;
    const static QString S_ID_PRO_LIBS_D;

    const static QString S_ID_AUTHOR;
    const static QString S_ID_AUTHOR_EMAIL;

    const static QString S_ID_INDENT;

    // compatability
    const static QString S_CONSTREF_MACRO;
    const static QString S_2_0_VERSION_CHECK;
    const static QStringList S_2_0_ICON_INCLUDES;
    const static QStringList S_2_0_CONSTREF;

    ModuleGeneratorSettings const* settings() const;
    ModuleGeneratorSettings* settings();

signals:

    void generationFinished();

public slots:

    bool generate();

private:

    ModuleGeneratorSettings m_settings{};

    QStringList m_proFileIncludePaths{};
    QDir m_srcDir{};
    QDir m_moduleDir{};
    QDir m_featuresDir{};

    bool generateHelper();

    bool generateModulePath();

    bool generateModuleSettingsFiles();

    bool generateModuleProjectFile();

    bool generateGitFiles();

    bool generateModuleDependencies();

    bool generateModule();

    void generateFunction(QString& headerString,
                          QString& sourceString,
                          FunctionData const& function,
                          bool isConstructor = false);

    void generateImplementation(QString& headerString,
                                QString& sourceString,
                                FunctionData const& function,
                                bool isConstructor = false);

    void generateImplementationHelper(QString& sourceString,
                                      ClassData const& baseClass,
                                      QList<ClassData> const& classes);

    void generateBasicClass(ClassData const& base,
                            ClassData const& derived);

    void generateIncludes(QString& sourceString,
                          QStringList const& includes);

    void generateForwardDeclarations(QString& headerString,
                                     QStringList const& forwardDecls);

    void generateConstructors(QString& headerString,
                              QString& sourceString,
                              ClassData const& base);

    void appendFileToProjectFile(QString const& fileName,
                                 QString const& path);

    void appendLibToProjectFile(QString const& name);

    void clearFileString(QString& fileString);

    void clearCompabilityMacros(QString& fileString);

    void clearProjectFileIdentifiers();
};

#endif // MODULEGENERATOR_H
