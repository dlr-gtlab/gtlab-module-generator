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
    void searchForInterfaces();

    /**
     * @brief searches in devtools for reserved prefixes (gt, gtp, etc.)
     * @param devToolsPath path to dev tools
     */
    void searchForPrefixes(QString const& devToolsPath);

    /**
     * @brief asks GtlabConsole for modules to generate dependencies.
     * Should be run concurrently
     * @param gtlabPath path to gtlab
     * @param status return code of process
     */
    void searchForDependencies(QString const& gtlabPath, int* status = nullptr);

    ClassDataList const& interfaces() const { return m_interfaces; }
    DependencyDataList const& dependencies() const { return m_dependencies; }
    QStringList const& prefixes() const { return m_prefixes; }

private:

    QStringList m_prefixes{};
    ClassDataList m_interfaces{};
    DependencyDataList m_dependencies{};

    /**
     * @brief Parses json object to a class struct
     * @param classJObject json object for class
     * @return class struct
     */
    ClassData searchForClass(QJsonObject const& classJObject);

    Constructors searchForConstructors(QJsonArray const& constructorJArray);

    FunctionDataList searchForFunctions(QJsonArray const& functionsJArray);

    QStringList parseStringJArray(QJsonArray const& jArray);

    QString parseDescription(QJsonArray const& descriptionJArray);

    void clearInterfaceStructs();
};

#endif // MODULEGENERATORPRELOADER_H
