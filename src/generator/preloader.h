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

    void searchForDependencies(const QString& gtlabPath);

    ClassStructsPtr interfaces() const { return m_interfaces; }
    DependencieStructs dependencies() const { return m_dependencies; }
    QStringList prefixes() const { return m_prefixes; }

private:

    QStringList m_prefixes;
    ClassStructsPtr m_interfaces;
    DependencieStructs m_dependencies;

    ClassStruct searchForClass(const QJsonObject& classJObject);

    FunctionStructs searchForFunctions(const QJsonArray& functionsJArray);



    void clearInterfaceStructs();
};

#endif // B_H
