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

#include <QtConcurrent>
#include <QFont>

const QString
ModuleGeneratorSettings::S_VERSION(QStringLiteral("1.0.5"));
const QRegularExpression
ModuleGeneratorSettings::REG_PREFIX(QStringLiteral("[A-Za-z]([A-Za-z]|\\d)*"));
const QRegularExpression
ModuleGeneratorSettings::REG_OBJECT_NAME(QStringLiteral("\\s?[A-Za-z]"
                                                        "([\\w\\d]*\\s?)+"));
const QRegularExpression
ModuleGeneratorSettings::REG_VERSION(QStringLiteral("\\d+(.\\d+)?"
                                                    "(.\\d+)?(-\\w+)?"));
const QRegularExpression
ModuleGeneratorSettings::REG_AUTHOR(QStringLiteral("[A-Za-z].*"));
const QRegularExpression
ModuleGeneratorSettings::REG_AUTHOR_EMAIL(QStringLiteral(".*"));
const QFont
ModuleGeneratorSettings::F_MONO_FONT = QFont(QStringLiteral("Consolas"), 9);

const QString S_YEAR = QDate::currentDate().toString("yyyy");
const QString S_DATE = QDate::currentDate().toString("dd.MM.yyyy");

const QString ModuleGeneratorSettings::S_SIGNATURE = QStringLiteral(
        "/* GTlab - Gas Turbine laboratory\n"
//        " * Source File: $$FILE_NAME$$\n"
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
    {
        m_moduleClass.version = QStringLiteral("0.0.1");
    }
}

bool
ModuleGeneratorSettings::isOsWindows()
{
    return QSysInfo::productType() == "windows";
}

QString
ModuleGeneratorSettings::classNamingScheme(const QString& name,
                                           const QString& prefix) const
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
ModuleGeneratorSettings::fileNamingScheme(const QString& name,
                                          const QString& prefix) const
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

int
ModuleGeneratorSettings::gtlabMajorVersion() const
{
    return QVersionNumber::fromString(m_version).majorVersion();
}

QStringList
ModuleGeneratorSettings::supportedVersions() const
{
    QStringList retVal;

    retVal << QVersionNumber{1,7,0}.toString();
    retVal << QVersionNumber{2,0,0}.toString() + "-dp4";

    return retVal;
}

QString const&
ModuleGeneratorSettings::modulePrefix() const
{
    return m_modulePrefix;
}

void
ModuleGeneratorSettings::setModulePrefix(QString const& prefix)
{
    m_modulePrefix = prefix;
}

ModuleClass const&
ModuleGeneratorSettings::moduleClass() const {
    return m_moduleClass;
}

void ModuleGeneratorSettings::setModuleClass(const ModuleClass &module) {
    m_moduleClass = module;
}

ClassStructs const&
ModuleGeneratorSettings::selectedInterfaces() const
{
    return m_selectedInterfaces;
}

void
ModuleGeneratorSettings::setSelectedInterfaces(ClassStructs const& interfaces)
{
    m_selectedInterfaces = interfaces;
}

DependencyStructs const&
ModuleGeneratorSettings::selectedDependencies() const
{
    return m_selectedDependencies;
}

void
ModuleGeneratorSettings::setSelectedDependencies(DependencyStructs const&
                                                 dependencies)
{
    m_selectedDependencies = dependencies;
}

AuthorDetails const& ModuleGeneratorSettings::authorDetails() const
{
    return m_authorDetails;
}

void
ModuleGeneratorSettings::setAuthorDetails(AuthorDetails const& details)
{
    m_authorDetails = details;
}

void
ModuleGeneratorSettings::setCreateGitFiles(bool value)
{
    m_createGitFiles = value;
}

bool
ModuleGeneratorSettings::createGitFiles() const
{
    return m_createGitFiles;
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
        auto function = [](ModuleGeneratorPreLoader* loader, QString path) -> int
        {
            int status = -1;
            loader->searchForDependencies(path, &status);
            return status;
        };

        m_futureDependencies = QtConcurrent::run(function,
                                                 &m_preLoader,
                                                 gtlabPath());
    }
}

QStringList const&
ModuleGeneratorSettings::reservedPrefixes() const {
    return m_preLoader.prefixes();
}

ClassStructs const&
ModuleGeneratorSettings::availableInterfaces() const {
    return m_preLoader.interfaces();
}

DependencyStructs const&
ModuleGeneratorSettings::availableDependencies() const
{
    return m_preLoader.dependencies();
}

int
ModuleGeneratorSettings::dependencyResolveStatus() const
{
    if (m_futureDependencies.isRunning())
    {
        return 2;
    }
    if (m_futureDependencies.isFinished())
    {
        return m_futureDependencies.result();
    }

    return -1;
}

void
ModuleGeneratorSettings::serializeUserData() const
{
    LOG_INDENT("serializing wizard data to json...");

    QJsonObject userObject;
    userObject["author"] = m_authorDetails.name;
    userObject["email"]  = m_authorDetails.email;

    QJsonObject pathsObject;
    pathsObject["output"]   = m_outputPath;
    pathsObject["devtools"] = m_devToolsPath;
    pathsObject["gtlab"]    = m_gtlabPath;

    QJsonObject moduleObject;
    moduleObject["target_version"]  = m_version;

//    QJsonObject moduleObject;
//    moduleObject["prefix"]  = m_modulePrefix;
//    moduleObject["ident"]  = m_moduleClass.ident;
//    moduleObject["version"]     = m_moduleClass.version;
//    moduleObject["description"] = m_moduleClass.description;

    QJsonObject rootObject;
    rootObject["user"]   = userObject;
    rootObject["paths"]  = pathsObject;
    rootObject["module"] = moduleObject;
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
    LOG_INDENT("deserializing wizard data from json...");

    QDir appDir(QApplication::applicationDirPath());

    QString json = utils::readFile(appDir.absoluteFilePath(
                                   "module_generator.json"));

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

    QJsonObject moduleObject = document["module"].toObject();
    m_version = moduleObject["target_version"].toString();

    LOG_INFO << "done!";
}
