#include "module_generator_utils.h"

#include <QString>
#include <QFile>
#include <QDir>

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
