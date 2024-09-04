#ifndef MODULEGENERATORUTILS_H
#define MODULEGENERATORUTILS_H

#include "module_generator_structs.h"

#include <QRegularExpression>

class QString;
class QDir;

namespace utils
{
    void replaceIdentifier(QString& inputString, const IdentifierPairs& pairs);
    void replaceIdentifier(QString& inputString, const IdentifierPair& pair);

    QString readFile(const QString& filePath);

    void writeStringToFile(const QString& fileContent,
                           const QDir& targetDir,
                           const QString& fileName);

    QString makeInclude(const QString& inlcude,
                        const QString& identifier = {},
                        const bool isLib = false);

    /**
     * @brief Returns a list of relative filenames that exist in dir
     * @param dir The directory to search
     * @param recursive If true, search also in subdirs
     * @param reFilter Regex to filter files to be returned
     * @return
     */
    QStringList listFiles(const QDir& dir, bool recursive, const QRegularExpression& reFilter = {});

    /**
     * @brief Returns a list of unique entries
     * @param list List to make unique
     * @return Unique list
     */
    QStringList unique(QStringList const& list);

} // namespace utils

#endif // MODULEGENERATORUTILS_H
