#include "module_generator_settings.h"

#include "module_generator.h"

#include <QStringList>

const QString
    ModuleGeneratorSettings::S_VERSION(QStringLiteral("1.0"));
const QRegularExpression
    ModuleGeneratorSettings::REG_PREFIX(QStringLiteral("[A-Za-z]([A-Za-z]|\\d)*"));
const QRegularExpression
    ModuleGeneratorSettings::REG_OBJECT_NAME(QStringLiteral("[A-Za-z]([\\w\\d]*\\s?)+"));
const QRegularExpression
    ModuleGeneratorSettings::REG_VERSION(QStringLiteral("\\d+(.\\d+)?(.\\d+)?(-\\w+)?"));
const QRegularExpression
    ModuleGeneratorSettings::REG_AUTHOR(QStringLiteral("[A-Za-z].*"));


ModuleGeneratorSettings::ModuleGeneratorSettings()
{
//    m_gtlabPath     = "C:\\GTlab-DevTools-1_7\\stable\\1_7";
//    m_devToolsPath  = "C:\\GTlab-DevTools-1_7\\stable\\1_7";
//    m_outputPath    = "C:\\tmp";

//    m_gtlabPath     = "D:\\";
//    m_devToolsPath  = "D:\\";
//    m_outputPath    = "D:\\";
}

QString
ModuleGeneratorSettings::classNamingScheme(const QString& name, const QString& prefix) const
{
    QString retString;

    QStringList stringList;

    if (name.isEmpty())
    {
        return QString();
    }

    if (!prefix.isEmpty())
    {
        stringList << prefix;
    }

    stringList << name.split(" ", QString::SkipEmptyParts);

    for (auto string : stringList)
    {
        string.replace(0, 1, string.at(0).toUpper());

        retString += string;
    }

    return retString;
}

QString
ModuleGeneratorSettings::classNamingScheme(const QString& name) const
{
    return classNamingScheme(name, modulePrefix());
}

QString
ModuleGeneratorSettings::fileNamingScheme(const QString& name, const QString& prefix) const
{
    QString retString;

    retString = classNamingScheme(name, prefix);

    if (retString.isEmpty())
    {
        return QString();
    }

    if (!prefix.isEmpty())
    {
        retString.insert(prefix.size(), QStringLiteral("_"));
    }

    retString = retString.toLower();

    return retString;
}

QString
ModuleGeneratorSettings::fileNamingScheme(const QString& name) const
{
    return fileNamingScheme(name, modulePrefix());
}

QString
ModuleGeneratorSettings::signatureString() const
{
//    return "// generated " + ModuleGenerator::S_ID_GENERATOR_VERSION;

    return m_signature;
}

void
ModuleGeneratorSettings::setSignatureString(const QString& signature)
{
    m_signature = signature;
}

void
ModuleGeneratorSettings::preLoad()
{    
    /*
    // load interfaces and prefixes in a separate thread
    auto function = [](PreLoader* loader, QString path) -> void
    {
        loader->searchForInterfaces();
        loader->searchForPrefixes(path);
    };

    auto future = QtConcurrent::run(function, &m_preLoader, devToolsPath());
    */

    m_preLoader.searchForInterfaces();
    m_preLoader.searchForPrefixes(devToolsPath());
}
