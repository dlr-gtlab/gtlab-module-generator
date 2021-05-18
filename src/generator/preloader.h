#ifndef B_H
#define B_H

#include "module_generator_structs.h"

class QString;
class PreLoader
{

public:

    ~PreLoader();

    void searchForInterfaces(/*const QString& devToolsPath*/);

    void searchForPrefixes(const QString& devToolsPath);

    ClassStructsPtr interfaceStructs() const { return m_interfaces; }

    QStringList prefixes() const { return m_prefixes; }

private:

    QString m_devToolsPath;

    QStringList m_prefixes;

    ClassStructsPtr m_interfaces;

    ClassStruct* searchForClass(const QJsonObject& classJObject);

    FunctionStructsPtr searchForFunctions(const QJsonArray& functionsJArray);

    void clearInterfaceStructs();
};

#endif // B_H
