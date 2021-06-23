#include "module_generator_settings.h"

#include "module_generator.h"
#include "module_generator_logger.h"
#include "module_generator_utils.h"

#include <QStringList>
#include <QDate>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QApplication>

#include <QFuture>
#include <QtConcurrent>
#include <QFont>

const QString
    ModuleGeneratorSettings::S_VERSION(QStringLiteral("1.0.2"));
const QRegularExpression
    ModuleGeneratorSettings::REG_PREFIX(QStringLiteral("[A-Za-z]([A-Za-z]|\\d)*"));
const QRegularExpression
    ModuleGeneratorSettings::REG_OBJECT_NAME(QStringLiteral("\\s?[A-Za-z]([\\w\\d]*\\s?)+"));
const QRegularExpression
    ModuleGeneratorSettings::REG_VERSION(QStringLiteral("\\d+(.\\d+)?(.\\d+)?(-\\w+)?"));
const QRegularExpression
    ModuleGeneratorSettings::REG_AUTHOR(QStringLiteral("[A-Za-z].*"));
const QRegularExpression
    ModuleGeneratorSettings::REG_AUTHOR_EMAIL(QStringLiteral(".*"));
const QFont ModuleGeneratorSettings::F_MONO_FONT = QFont(QStringLiteral("Consolas"), 9);

const QString S_YEAR = QDate::currentDate().toString("yyyy");
const QString S_DATE = QDate::currentDate().toString("dd.MM.yyyy");

const QString ModuleGeneratorSettings::S_SIGNATURE = QStringLiteral(
        "/* GTlab - Gas Turbine laboratory\n"
        " * Source File: $$FILE_NAME$$\n"
        " * copyright 2009-") + S_YEAR + QStringLiteral(" by DLR\n"
        " * \n"
        " * Created on: ") + S_DATE + QStringLiteral("\n"
        " * Author: ") + ModuleGenerator::S_ID_AUTHOR + QStringLiteral("\n"
        " * Email: ") + ModuleGenerator::S_ID_AUTHOR_EMAIL + QStringLiteral("\n"
        " */");

const QString
    ModuleGeneratorSettings::S_EXEC_SUFFIX(isOsWindows() ? QStringLiteral(".exe"):
                                                           QStringLiteral(""));
const QString
    ModuleGeneratorSettings::S_GTLAB_APP(QStringLiteral("GTlab") +
                                         S_EXEC_SUFFIX);
const QString
    ModuleGeneratorSettings::S_GTLAB_CONSOLE_APP(QStringLiteral("GTlabConsole") +
                                                 S_EXEC_SUFFIX);


ModuleGeneratorSettings::ModuleGeneratorSettings()
{
    deserializeUserData();

    m_preLoader.searchForInterfaces();

    // defaults
    if (m_moduleClass.version.isEmpty())
        m_moduleClass.version = QStringLiteral("0.0.1");
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

void
ModuleGeneratorSettings::preLoad()
{
    if (m_preLoader.searchPath() != gtlabPath() ||
        m_preLoader.prefixes().isEmpty())
    {
        m_preLoader.searchForPrefixes(devToolsPath());
    }

    if (m_preLoader.searchPath() != gtlabPath() ||
        m_preLoader.dependencies().isEmpty())
    {
        // load dependencies in a separate thread
        auto function = [](PreLoader* loader, QString path) -> void
        {
            loader->searchForDependencies(path);
        };
        QFuture<void> future = QtConcurrent::run(function,
                                                 &m_preLoader,
                                                 gtlabPath());
    }
}

void
ModuleGeneratorSettings::serializeUserData() const
{
    LOG_INSTANCE("serializing wizard data to json...");

    QJsonObject userObject;
    userObject["author"] = m_authorDetails.name;
    userObject["email"]  = m_authorDetails.email;

    QJsonObject pathsObject;
    pathsObject["output"]   = m_outputPath;
    pathsObject["devtools"] = m_devToolsPath;
    pathsObject["gtlab"]    = m_gtlabPath;

//    QJsonObject moduleObject;
//    moduleObject["prefix"]  = m_modulePrefix;
//    moduleObject["ident"]  = m_moduleClass.ident;
//    moduleObject["version"]     = m_moduleClass.version;
//    moduleObject["description"] = m_moduleClass.description;

    QJsonObject rootObject;
    rootObject["user"] = userObject;
    rootObject["paths"]  = pathsObject;
//    rootObject["lastModule"] = moduleObject;

    QJsonDocument document(rootObject);

    QString json(document.toJson());

    QDir appDir(QApplication::applicationDirPath());

    utils::writeStringToFile(json, appDir, "module_generator.json");

    LOG_INFO << "done!";
}

void
ModuleGeneratorSettings::deserializeUserData()
{
    LOG_INSTANCE("deserializing wizard data from json...");

    QDir appDir(QApplication::applicationDirPath());

    QString json = utils::readFile(appDir.absoluteFilePath("module_generator.json"));

    QJsonDocument document(QJsonDocument::fromJson(json.toUtf8()));

    if (document.isNull())
    {
        LOG_WARN << "null json document";
        return;
    }

    QJsonObject userObject = document["user"].toObject();
    m_authorDetails.name  = userObject["author"].toString();
    m_authorDetails.email = userObject["email"].toString();

    QJsonObject pathsObject = document["paths"].toObject();
    m_outputPath   = pathsObject["output"].toString();
    m_devToolsPath = pathsObject["devtools"].toString();
    m_gtlabPath    = pathsObject["gtlab"].toString();

    LOG_INFO << "done!";
}
