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
ModuleGeneratorSettings::S_VERSION(QStringLiteral("1.2.0"));
const QRegularExpression
ModuleGeneratorSettings::REG_PREFIX(QStringLiteral("[A-Za-z]([A-Za-z_]|\\d)*"));
const QRegularExpression
ModuleGeneratorSettings::REG_OBJECT_NAME(QStringLiteral("\\s?[A-Za-z]"
                                                        "([\\w\\d]*\\s?)+"));
const QRegularExpression
ModuleGeneratorSettings::REG_CLASS_NAME(QStringLiteral("[A-Za-z]([\\w\\d]*?)+"));
const QRegularExpression
ModuleGeneratorSettings::REG_FILE_NAME = REG_CLASS_NAME;
const QRegularExpression
ModuleGeneratorSettings::REG_VERSION(QStringLiteral("\\d+(.\\d+)?"
                                                    "(.\\d+)?(-\\w+)?"));
const QRegularExpression
ModuleGeneratorSettings::REG_AUTHOR(QStringLiteral("[A-Za-z].*"));
const QRegularExpression
ModuleGeneratorSettings::REG_AUTHOR_EMAIL(QStringLiteral(".*"));

const QFont
ModuleGeneratorSettings::F_MONO_FONT{QStringLiteral("Consolas"), 9};

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
                                                       QString{});
const QString
ModuleGeneratorSettings::S_GTLAB_APP(QStringLiteral("GTlab") +
                                     S_EXEC_SUFFIX);
const QString
ModuleGeneratorSettings::S_GTLAB_CONSOLE_APP(QStringLiteral("GTlabConsole") +
                                             S_EXEC_SUFFIX);


ModuleGeneratorSettings::ModuleGeneratorSettings()
{
    QElapsedTimer timer;
    timer.start();

    deserializeUserData();

    m_preLoader.loadInterfaceData();
    m_preLoader.loadTypeInfo();

    // defaults
    if (m_moduleClass.version.isEmpty())
    {
        m_moduleClass.version = QStringLiteral("0.0.1");
    }

    LOG_INDENT("Setup took " + QString::number(timer.elapsed()) + " ms");
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
    if (name.isEmpty())
    {
        return {};
    }

    QStringList stringList;

    if (!prefix.isEmpty())
    {
        stringList << prefix;
    }

    stringList << name.split(" ", QString::SkipEmptyParts);

    QString retString;
    for (auto& string : stringList)
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
    QString retString = classNamingScheme(name, prefix);

    if (retString.isEmpty())
    {
        return {};
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
ModuleGeneratorSettings::supportedVersions()
{
    QStringList retVal;

    retVal << QVersionNumber{1,7,0}.toString();
    retVal << QVersionNumber{2,0,0}.toString()/* + " (DP5)"*/;

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

ModuleData const&
ModuleGeneratorSettings::moduleClass() const {
    return m_moduleClass;
}

void ModuleGeneratorSettings::setModuleClass(const ModuleData &module) {
    m_moduleClass = module;
}

ClassDataList const&
ModuleGeneratorSettings::selectedInterfaces() const
{
    return m_selectedInterfaces;
}

void
ModuleGeneratorSettings::setSelectedInterfaces(ClassDataList const& interfaces)
{
    m_selectedInterfaces = interfaces;
}

DependencyDataList const&
ModuleGeneratorSettings::selectedDependencies() const
{
    return m_selectedDependencies;
}

void
ModuleGeneratorSettings::setSelectedDependencies(DependencyDataList const&
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
    if (m_lastPreLoadPath != gtlabPath() || m_preLoader.prefixes().isEmpty())
    {
        m_preLoader.findPrefixData(devToolsPath());
    }

    if (m_lastPreLoadPath != gtlabPath() || m_preLoader.dependencies().isEmpty())
    {
        // load dependencies in a separate thread
        auto function = [](auto* loader, auto const& path)
        {
            int status = -1;
            loader->findDependencies(path, &status);
            return status;
        };

        m_futureDependencies = QtConcurrent::run(function,
                                                 &m_preLoader,
                                                 gtlabPath());
        m_lastPreLoadPath = gtlabPath();
    }
}

QStringList const&
ModuleGeneratorSettings::reservedPrefixes() const {
    return m_preLoader.prefixes();
}

ClassDataList const&
ModuleGeneratorSettings::availableInterfaces() const {
    return m_preLoader.interfaces();
}

TypeInfo
ModuleGeneratorSettings::typeInfo(QString const& returnType) const
{
    // first check for complete match
    for (auto const& info :  qAsConst(m_preLoader.typeInfo()))
    {
        if (info.returnType == returnType)
        {
            return info;
        }
    }

    // check by reg exp
    for (auto const& info :  qAsConst(m_preLoader.typeInfo()))
    {
        QRegularExpression regExp{"^" + info.returnType + "$"};
        if (regExp.match(returnType).hasMatch())
        {
            return info;
        }
    }

    return {};
}

DependencyDataList const&
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
    moduleObject["target_version"] = m_version;
    moduleObject["use_compatibility_macros"] = m_useCompatibilityMacros;

//    moduleObject["prefix"]  = m_modulePrefix;
//    moduleObject["ident"]  = m_moduleClass.ident;
//    moduleObject["version"]     = m_moduleClass.version;
//    moduleObject["description"] = m_moduleClass.description;
//    moduleObject["class_name"] = m_moduleClass.className;
//    moduleObject["file_name"] = m_moduleClass.fileName;

    QJsonObject rootObject;
    rootObject["user"]   = userObject;
    rootObject["paths"]  = pathsObject;
    rootObject["module"] = moduleObject;

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
    if (moduleObject.contains("target_version"))
    {
        m_version = moduleObject["target_version"].toString();
    }
    if (moduleObject.contains("use_compatibility_macros"))
    {
        m_useCompatibilityMacros = moduleObject["use_compatibility_macros"].toBool();
    }

//    m_modulePrefix = moduleObject["prefix"].toString();
//    m_moduleClass.ident = moduleObject["ident"].toString();
//    m_moduleClass.version = moduleObject["version"].toString();
//    m_moduleClass.description = moduleObject["description"].toString();
//    m_moduleClass.className = moduleObject["class_name"].toString();
//    m_moduleClass.fileName = moduleObject["file_name"].toString();

    LOG_INFO << "done!";
}
