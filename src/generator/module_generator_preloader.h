#ifndef MODULEGENERATORPRELOADER_H
#define MODULEGENERATORPRELOADER_H

#include "module_generator_structs.h"

class QString;
class ModuleGeneratorPreLoader
{
    static const QString S_INCLUDE_CORE_PATH;
    static const QString S_INCLUDE_PATH;

    static const QStringList S_GTLAB_CONSOLE_ARGS;

    static const int I_PROCESS_TIMEOUT_MS;

    static const QRegularExpression REGEXP_PREFIX;

public:

    /**
     * @brief Parses interface json files and generates a list of class structs
     */
    void loadInterfaceData();

    void loadTypeInfo();

    /**
     * @brief searches in devtools for reserved prefixes (gt, gtp, etc.)
     * @param devToolsPath path to dev tools
     */
    void findPrefixData(QString const& devToolsPath);

    /**
     * @brief asks GtlabConsole for modules to generate dependencies.
     * Should be run concurrently
     * @param gtlabPath path to gtlab
     * @param status return code of process
     */
    void findDependencies(QString const& gtlabPath, int* status = nullptr);

    ClassDataList const& interfaces() const { return m_interfaces; }
    TypeInfoList const& typeInfo() const { return m_typeInfo; }
    DependencyDataList const& dependencies() const { return m_dependencies; }
    QStringList const& prefixes() const { return m_prefixes; }

private:

    ClassDataList m_interfaces{};
    TypeInfoList m_typeInfo{};
    DependencyDataList m_dependencies{};
    QStringList m_prefixes{};

    /**
     * @brief Parses json object to a class struct
     * @param classJObject json object for class
     * @return class struct
     */
    ClassData loadClassData(QJsonObject const& classJObject);
    ClassData loadClassData(QString const& fileName);

    Constructors loadConstructorData(QJsonArray const& constructorJArray);

    FunctionDataList loadFunctionData(QJsonArray const& functionsJArray);

    QStringList parseStringJArray(QJsonArray const& array);

    VersionData parseVersionJObject(QJsonObject const& version);

    QString parseDescription(QJsonArray const& descriptionJArray);
};

#endif // MODULEGENERATORPRELOADER_H
