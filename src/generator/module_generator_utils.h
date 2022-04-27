#ifndef MODULEGENERATORUTILS_H
#define MODULEGENERATORUTILS_H

#include "module_generator_structs.h"

class QString;
class QDir;

namespace utils
{
    void replaceIdentifier(QString& inputString, const IdentifierPairs& pairs);
    void replaceIdentifier(QString& inputString, const IdentifierPair& pair);

    QString readFile(const QString& filePath);

    void writeStringToFile(const QString& textStr,
                           const QDir& targetDir,
                           const QString& fileName);

    QString makeInclude(const QString& inlcude,
                        const QString& identifier = {},
                        const bool isLib = false);
}


#endif // MODULEGENERATORUTILS_H
