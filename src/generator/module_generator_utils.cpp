#include "module_generator_utils.h"

#include <QString>
#include <QFile>
#include <QDir>

#include <set>


namespace
{

void
dirEntriesImpl(const QDir& dir, const QString& prefix, QStringList& result, bool recursive)
{
    if (!dir.exists())
    {
        return;
    }

    auto files = dir.entryList(QDir::Files);
    std::transform(files.begin(), files.end(), files.begin(),
                   [&prefix](const QString& fname) {
                       return prefix + fname;
                   });

    result.append(std::move(files));

    if (recursive)
    {
        for (const QString& directoryName : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        {
            QDir subDir(dir.path() + "/" + directoryName);
            dirEntriesImpl(subDir, prefix + directoryName + "/", result, true);
        }
    }

    return;
}

} // namespace


void
utils::replaceIdentifier(QString& inputString, const IdentifierPairs& pairs)
{
    for (auto const& pair : pairs)
    {
        inputString.replace(pair.identifier, pair.value);
    }
}

void
utils::replaceIdentifier(QString& inputString, const IdentifierPair& pair)
{
    inputString.replace(pair.identifier, pair.value);
}

QString
utils::readFile(const QString& filePath)
{
    QFile file(filePath);

    if(!file.exists())
    {
        return {};
    }

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray fileContent = file.readAll();

    QString retStr(fileContent);

    file.close();

    return retStr;
}


void
utils::writeStringToFile(const QString& fileContent,
                         const QDir& targetDir,
                         const QString& fileName)
{
    QFile file{targetDir.absoluteFilePath(fileName)};

    file.open(QIODevice::ReadWrite | QIODevice::Text);

    file.resize(0);

    file.write(fileContent.toUtf8());

    file.close();
}

QString
utils::makeInclude(const QString& inlcude,
                   const QString& identifier,
                   const bool isLib)
{
    if (isLib)
    {
        return {"#include <"+ inlcude + ">\n" + identifier};
    }

    return {"#include \""+ inlcude + ".h\"\n" + identifier};
}

QStringList
utils::listFiles(const QDir& dir, bool recursive, const QRegularExpression& expr)
{
    QStringList files;
    dirEntriesImpl(dir, "", files, recursive);

    // filter according to expr
    QStringList filesFiltered;
    std::copy_if(files.begin(), files.end(),
                 std::back_inserter(filesFiltered),
                 [&expr](const QString& fname) {
        return expr.match(fname).hasMatch();
    });

    return filesFiltered;
}

QStringList
utils::unique(const QStringList& list)
{
    std::set<QString> tmp(std::begin(list), std::end(list));

    QStringList result;
    std::copy(tmp.begin(), tmp.end(), std::back_inserter(result));
    return result;
}
