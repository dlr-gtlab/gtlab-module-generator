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

public:

    void searchForInterfaces();

    void searchForPrefixes(QString const& devToolsPath);

    void searchForDependencies(QString const& gtlabPath, int* status = nullptr);

    QString const& searchPath() const { return m_searchPath; }
    ClassStructs const& interfaces() const { return m_interfaces; }
    DependencyStructs const& dependencies() const { return m_dependencies; }
    QStringList const& prefixes() const { return m_prefixes; }

private:

    QString m_searchPath{};
    QStringList m_prefixes{};
    ClassStructs m_interfaces{};
    DependencyStructs m_dependencies{};

    ClassStruct searchForClass(QJsonObject const& classJObject);

    Constructors searchForConstructors(QJsonArray const& constructorJObject);

    FunctionStructs searchForFunctions(QJsonArray const& functionsJArray);

    void clearInterfaceStructs();
};

#endif // MODULEGENERATORPRELOADER_H
