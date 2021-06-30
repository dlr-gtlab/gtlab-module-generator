#ifndef MODULEGENERATOR_H
#define MODULEGENERATOR_H

#include <QObject>
#include <QString>
#include <QDir>

struct ClassStruct;
struct FunctionStruct;
struct ImplementationStruct;

class ModuleGeneratorSettings;
class ModuleGenerator : public QObject
{
    Q_OBJECT

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
    const static QString S_ID_FUNCTION;
    const static QString S_ID_IMPLEMENTATION;

    const static QString S_ID_MODULE_NAME;
    const static QString S_ID_MODULE_DESCRIPTION;
    const static QString S_ID_MODULE_VERSION;
    const static QString S_ID_INTERFACE_MACRO;

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

    ModuleGenerator(ModuleGeneratorSettings* settings):
        m_settings(settings) { }

signals:

    void generationFinished();

public slots:

    void onGenerate();

private:

    ModuleGeneratorSettings* m_settings;

    QStringList m_proFileIncludePaths;
    QDir m_srcDir;
    QDir m_moduleDir;
    QDir m_featuresDir;

    int generate();

    bool generateModulePath();

    bool generateModuleSettingsFiles();

    bool generateModuleProjectFile();

    bool generateGitFiles();

    bool generateModuleDependencies();

    bool generateModule();

    void generateFunction(QString& headerString,
                          QString& sourceString,
                          FunctionStruct& f);

    void generateImplementation(QString& headerString,
                                QString& sourceString,
                                FunctionStruct& function);

    void generateBasicClass(ClassStruct& base,
                            ClassStruct& derived);

    void generateIncludes(QString& sourceString,
                          QStringList& includes);

    void generateForwardDeclarations(QString& headerString,
                                     QStringList& forwardDecls);

    void appendFileToProjectFile(QString& fileName,
                                 const QString& path);

    void appendLibToProjectFile(const QString& name);

    void clearIdentifiers(QString& fileString);

    void clearTabulators(QString& fileString);

    void clearProjectFileIdentifiers();
};

#endif // MODULEGENERATOR_H
