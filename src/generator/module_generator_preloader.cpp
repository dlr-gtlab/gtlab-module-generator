/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include "module_generator_preloader.h"
#include "module_generator_utils.h"
#include "module_generator_logger.h"
#include "module_generator_settings.h"
#include "module_generator.h"

#include <QDir>
#include <QDirIterator>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVersionNumber>

#include <QProcess>
#include <QStringList>
#include <QDomDocument>

const QString
ModuleGeneratorPreLoader::S_INCLUDE_CORE_PATH = QStringLiteral("core");
const QString
ModuleGeneratorPreLoader::S_INCLUDE_PATH      = QStringLiteral("include");

const int
ModuleGeneratorPreLoader::I_PROCESS_TIMEOUT_MS = 15 * 1000;

/*
// intended for auto detecting interfaces and functions
const QRegularExpression REGEXP_INTERFACE(
        QStringLiteral("Q_DECLARE_INTERFACE\\((\\w+),"));
const QRegularExpression REGEXP_VIRTUAL_FUNCTION(
        QStringLiteral("(\\/\\*\\*.*\\n(\\s+\\*.*\\n)+\\s+\\*\\/)?\\s+" // comment block
                       "virtual\\s+" // virtual keyword
                       "([\\w\\*\\s,<>]+)\\s+" // return value
                       "(\\w+)" // function name
                       "\\((.*)?\\)\\s+" // parameter
                       "(const)?\\s?=\\s+0")); // const and pure virtual
*/

void
ModuleGeneratorPreLoader::loadInterfaceData()
{
    LOG_INDENT("searching for interfaces...");

    m_interfaces.clear();

    QDir tmpDir{":/interfaces/"};

    auto entries = tmpDir.entryList(QStringList{"*.json"},
                                    QDir::Files, QDir::SortFlag::Name);

    for (auto& entry : entries)
    {
        LOG_INDENT(entry);

        /// interface class
        auto interface = loadClassData(entry);

        if (!interface.isValid()) continue;

        m_interfaces.append(std::move(interface));

        LOG_INFO << " done!";
    }

    LOG_INFO << "done!";
}

void
ModuleGeneratorPreLoader::loadTypeInfo()
{
    LOG_INDENT("searching for type info...");

    m_typeInfo.clear();

    QDir tmpDir{":/interfaces/type_info/"};

    auto entries = tmpDir.entryList(QStringList{"*.json"},
                                    QDir::Files, QDir::SortFlag::Name);

    for (auto const& entry : qAsConst(entries))
    {
        LOG_INDENT(entry);

        auto document = QJsonDocument::fromJson(utils::readFile(
                            tmpDir.absoluteFilePath(entry)).toUtf8());

        if (document.isNull())
        {
            LOG_WARN << "invalid type info document";
            continue;
        }

        auto root = document.object();
        QString returnType  = root["returnType"].toString();
        if (returnType.isEmpty())
        {
            LOG_WARN << "invalid type info: " << returnType;
            continue;
        }

        TypeInfo typeInfo;
        typeInfo.returnType = std::move(returnType);
        typeInfo.widgetType  = root["widgetType"].toString();
        typeInfo.includes = parseStringJArray(root["includes"].toArray());
        typeInfo.values = parseStringJArray(root["values"].toArray());
        typeInfo.implementation = parseStringJArray(root["implementation"]
                                                    .toArray());

        m_typeInfo << std::move(typeInfo);
    }

    LOG_INFO << "done!";
}

ClassData
ModuleGeneratorPreLoader::loadClassData(QJsonObject const& classJObject)
{
    if (classJObject.isEmpty()) return {};

    QString className  = classJObject["name"].toString();
    QString fileName   = classJObject["fileName"].toString();
    QString outputPath = classJObject["outputPath"].toString();
    /// only relevant for interface class
    QString objectName = classJObject["displayName"].toString();
    QString tooltip    = classJObject["tooltip"].toString();

    auto versionJObject = classJObject["version"].toObject();
    auto functionsJArray   = classJObject["functions"].toArray();
    auto constructorJArray = classJObject["constructors"].toArray();

    LOG_INDENT("className  " + className);

    ClassData classStruct;
    // moving because varaibles are no longer needed
    classStruct.className  = std::move(className);
    classStruct.fileName   = std::move(fileName);
    classStruct.objectName = std::move(objectName);
    classStruct.tooltip    = std::move(tooltip);
    classStruct.outputPath = std::move(outputPath);
    classStruct.version      = parseVersionJObject(versionJObject);
    classStruct.functions    = loadFunctionData(functionsJArray);
    classStruct.constructors = loadConstructorData(constructorJArray);

    LOG_INFO << "done!";

    return classStruct;
}

ClassData
ModuleGeneratorPreLoader::loadClassData(QString const& fileName)
{
    QDir tmpDir{":/interfaces"};

    if (!tmpDir.exists(fileName))
    {
        LOG_INDENT_WARN("File does not exist! " + fileName);
        return {};
    }

    auto document = QJsonDocument::fromJson(utils::readFile(
                        tmpDir.absoluteFilePath(fileName)).toUtf8());

    if (document.isNull())
    {
        LOG_INDENT_WARN("Null document! " + fileName);
        return {};
    }

    auto interfaceJObject = document.object();

    return loadClassData(interfaceJObject);
}

Constructors
ModuleGeneratorPreLoader::loadConstructorData(QJsonArray const& constructorJArray)
{
    if (constructorJArray.isEmpty()) return {};

    LOG_INDENT("adding custom constructors...");

    Constructors constructors;

    for (auto const& ctorJObj : constructorJArray)
    {
        auto ctorJObject = ctorJObj.toObject();

        if (ctorJObject.isEmpty())
        {
            LOG_WARN << "empty constructor!";
            continue;
        }

        auto parameters = parseStringJArray(
                    ctorJObject["parameters"].toArray());
        auto initList = parseStringJArray(
                    ctorJObject["initList"].toArray());
        auto implementation = parseStringJArray(
                    ctorJObject["source"].toArray());

        LOG_INFO << "parameters:       " << parameters.join(", ") << ENDL;
        LOG_INFO << "initializer list: " << initList.join(", ") << ENDL;
        LOG_INFO << "implementation:   " << implementation.join(", ") << ENDL;

        constructors << ConstructorData{std::move(parameters),
                                        std::move(implementation),
                                        std::move(initList)};
    }

    LOG_INFO << "done!";

    return constructors;
}

FunctionDataList
ModuleGeneratorPreLoader::loadFunctionData(QJsonArray const& functionsJArray)
{
    LOG_INDENT("searching for functions...");

    if (functionsJArray.isEmpty())
    {
        return {};
    }

    FunctionDataList functions;

    for (auto const& functionJValue : functionsJArray)
    {
        auto functionJObject = functionJValue.toObject();

        if (functionJObject.isEmpty())
        {
            LOG_WARN << "empty function!";
            continue;
        }

        QString name        = functionJObject["name"].toString();
        QString returnType  = functionJObject["returnType"].toString();
        QString inputType   = functionJObject["inputType"].toString();
        QString tooltip     = functionJObject["tooltip"].toString();

        QJsonArray parameter    = functionJObject["parameters"].toArray();
        QJsonArray includes     = functionJObject["includes"].toArray();
        QJsonArray forwardDecls     = functionJObject["forwardDecl"].toArray();
        QJsonArray descriptionArray = functionJObject["description"].toArray();
        auto versionJObject = functionJObject["version"].toObject();

        auto baseClassName = functionJObject["baseClass"].toString();
        auto linkedClassName = functionJObject["linkedClass"].toString();

        LOG_INFO << "function: " << returnType << " " << name << ENDL;

        /* Implementation */
        ImplementationData implementation;

        implementation.includes = parseStringJArray(includes);
        implementation.forwardDeclarations = parseStringJArray(forwardDecls);

        if (inputType.isEmpty()) inputType = returnType;

        /* Function */
        FunctionData function;
        function.name = std::move(name);
        function.returnType  = std::move(returnType);
        function.inputType   = std::move(inputType);
        function.isConst     = functionJObject["const"].toBool();
        function.isProtected = functionJObject["protected"].toBool();
        function.isHidden    = functionJObject["hidden"].toBool();
        function.parameters  = parseStringJArray(parameter);
        function.description = parseDescription(descriptionArray);
        function.tooltip     = std::move(tooltip);
        function.version     = parseVersionJObject(versionJObject);
        function.implementation = std::move(implementation);

        if (!baseClassName.isEmpty())
        {
            baseClassName = "class_definitions/" + baseClassName + ".json";
            function.baseClass   = loadClassData(baseClassName);
        }

        if (!linkedClassName.isEmpty())
        {
            linkedClassName = "class_definitions/" + linkedClassName + ".json";
            function.linkedClass   = loadClassData(linkedClassName);
        }

        functions.append(std::move(function));
    }

    LOG_INFO << "done!";

    return functions;
}

QStringList
ModuleGeneratorPreLoader::parseStringJArray(QJsonArray const& array)
{
    QStringList retVal;
    for (auto const& valueJObj : array)
    {
        retVal << valueJObj.toString();
    }
    return retVal;
}

VersionData
ModuleGeneratorPreLoader::parseVersionJObject(QJsonObject const& version)
{
    if (version.isEmpty()) return {};

    LOG_INDENT("version...");

    auto vMin = version["min"].toString();
    auto vMax = version["max"].toString();

    if (!vMin.isEmpty()) { LOG_INFO << "min: " << vMin << ENDL; }
    if (!vMax.isEmpty()) { LOG_INFO << "max: " << vMax << ENDL; }

    LOG_INFO << "done!";

    return VersionData{vMin, vMax};
}

QString
ModuleGeneratorPreLoader::parseDescription(QJsonArray const& descriptionJArray)
{
    if (descriptionJArray.isEmpty()) return {};

    QString description{"\t/**\n"
                        "\t * "};

    description += parseStringJArray(descriptionJArray).join("\n\t * ");

    // end of description
    return description + "\n"
                         "\t */";
}

void
ModuleGeneratorPreLoader::findPrefixData(QString const& devToolsPath)
{
    LOG_INDENT("searching for occupied prefixes...");    

    m_prefixes.clear();

    QDir devToolsDir(devToolsPath + QDir::separator() + S_INCLUDE_PATH);

    if (!devToolsDir.exists())
    {
        LOG_ERR << "devtools dir does not exists!";
        return;
    }

    QDirIterator dirIterator{
        devToolsDir.absolutePath(), {},
        QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags
    };

    while (dirIterator.hasNext())
    {
        QDir subDir = dirIterator.next();

        QDirIterator iterator{
            subDir.absolutePath(), {"*.h"},
            QDir::Files, QDirIterator::NoIteratorFlags
        };

        while (iterator.hasNext())
        {
            static const QRegularExpression regExp{
                QStringLiteral("(\\w+?)_\\w+\\.h")
            };

            QString fileName = iterator.next();

            auto match = regExp.match(fileName);

            QString prefix = match.captured(1);

            if (prefix.isEmpty()) continue;

            if (!m_prefixes.contains(prefix))
            {
                LOG_INFO << "'" << prefix << "'" << ENDL;

                m_prefixes.append(prefix);
            }
        }
    }

    LOG_INFO << QString::number(m_prefixes.count())
             << " reserved prefixes found!";
}

QString
findVersion(QString const& processPath)
{
    // get version of gtlab
    QProcess versionProcess;
    versionProcess.start(processPath, {"--version"});

    if (!versionProcess.waitForFinished(2 * 1000))
    {
        return {};
    }
    static const QRegularExpression versionRegExp{QStringLiteral(
            R"(\d+\.\d+\.\d+)"
    )};
    QString versionOutput = versionProcess.readAllStandardOutput();
    return versionRegExp.match(versionOutput).captured(0);
}

void
ModuleGeneratorPreLoader::findDependencies(QString const& gtlabPath,
                                           int* status)
{    
    LOG_INDENT("searching for dependencies...");

    int _status;
    if (!status)
    {
        status = &_status;
    }

    *status = -1;

    m_dependencies.clear();

    QDir gtlabDir(gtlabPath);

    QString processName(ModuleGeneratorSettings::S_GTLAB_CONSOLE_APP);

    QString processPath = gtlabDir.absoluteFilePath(processName);

    if (!gtlabDir.exists(processName))
    {
        LOG_ERR << "invalid path to GTlabConsole ("
                << gtlabDir.absolutePath()
                << processName << ")!";
        return;
    }

    // get version of GTlab
    auto version = QVersionNumber::fromString(findVersion(processPath));

    if (version.isNull())
    {
        LOG_ERR << "aux process timeout!";
        *status = -1;
        return;
    }

    LOG_INFO << "GTlab version: " << version.toString() << ENDL;

    // start process
    QProcess process;
    process.start(processPath, {
        version.majorVersion() < 2 ? QString{"--footprint"} :
                                     QString{"footprint"}
    });

    if (!process.waitForFinished(I_PROCESS_TIMEOUT_MS))
    {
        LOG_ERR << "process timeout!";
        *status = process.exitCode();
        return;
    }

    *status = process.exitCode();

    QString output = process.readAllStandardOutput();

    int start = output.indexOf("<modules>");
    int end   = output.indexOf("</modules>");

    // parse output as xml document
    QDomDocument moduleXml;

    moduleXml.setContent(output.mid(start, end));
    QDomElement root = moduleXml.documentElement();

    if (root.isNull())
    {
        LOG_ERR << "parsed xml is null!";
        return;
    }

    QDomNodeList list(root.elementsByTagName(QStringLiteral("module")));

    LOG_INFO << "dependencies:" << ENDL;

    for (int i = 0; i < list.length(); ++i)
    {
        QDomElement module(list.at(i).toElement());
        QDomNodeList entries(module.childNodes());

        if (entries.length() != 2)
        {
            LOG_ERR << "invalid subentry!" << ENDL; continue;
        }

        DependencyStruct dependency;

        for (int j = 0; j < entries.length(); ++j)
        {
            QDomElement entry(entries.at(j).toElement());

            if (entry.tagName() == QStringLiteral("id"))
            {
                dependency.name = entry.text();
            }
            else
            {
                dependency.version = entry.text();
            }
        }

        m_dependencies << dependency;

        LOG_INFO << dependency.name << ": " << dependency.version << ENDL;
    }

    LOG_INFO << QString::number(m_dependencies.count())
             << " dependencies found!";
}
