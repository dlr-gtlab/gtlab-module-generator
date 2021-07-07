#ifndef B_H
#define B_H

#include "module_generator_structs.h"

class QString;
class PreLoader
{

public:

    ~PreLoader();

    void searchForInterfaces();

    void searchForPrefixes(const QString& devToolsPath);

    void searchForDependencies(const QString& gtlabPath, int* status = Q_NULLPTR);

    inline QString searchPath() const { return m_searchPath; }
    inline ClassStructsPtr interfaces() const { return m_interfaces; }
    inline DependencyStructs dependencies() const { return m_dependencies; }
    inline QStringList prefixes() const { return m_prefixes; }

private:

    QString m_searchPath;
    QStringList m_prefixes;
    ClassStructsPtr m_interfaces;
    DependencyStructs m_dependencies;

    ClassStruct searchForClass(const QJsonObject& classJObject);

    Constructors searchForConstructors(const QJsonArray& constructorJObject);

    FunctionStructs searchForFunctions(const QJsonArray& functionsJArray);

    void clearInterfaceStructs();
};

#endif // B_H
