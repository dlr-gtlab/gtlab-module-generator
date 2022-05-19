#include "module_generator_preloader.h"
#include "module_generator_utils.h"
#include "module_generator_logger.h"
#include "module_generator_settings.h"

#include <QDir>
#include <QDirIterator>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QProcess>
#include <QStringList>
#include <QDomDocument>

const QString
ModuleGeneratorPreLoader::S_INCLUDE_CORE_PATH = QStringLiteral("core");
const QString
ModuleGeneratorPreLoader::S_INCLUDE_PATH      = QStringLiteral("include");

const QStringList
ModuleGeneratorPreLoader::S_GTLAB_CONSOLE_ARGS{QStringLiteral("--footprint")};

const int
ModuleGeneratorPreLoader::I_PROCESS_TIMEOUT_MS = 15000;

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
const QRegularExpression
ModuleGeneratorPreLoader::REGEXP_PREFIX(QStringLiteral("(\\w+?)_\\w+\\.h"));


void
ModuleGeneratorPreLoader::searchForInterfaces()
{
    LOG_INDENT("searching for interfaces...");

    clearInterfaceStructs();

    QDir tmpDir{":/interfaces/"};

    auto entries = tmpDir.entryList(QStringList{"*.json"},
                                    QDir::Files, QDir::SortFlag::Name);

    for (auto& entry : entries)
    {
        LOG_INDENT(entry);

        /// interface class
        auto interface = searchForClass(entry);

        if (!interface.isValid()) continue;

        m_interfaces.append(std::move(interface));

        LOG_INFO << "done!";
    }

    LOG_INFO << "done!";
}

ClassData
ModuleGeneratorPreLoader::searchForClass(QJsonObject const& classJObject)
{
    if (classJObject.isEmpty()) return {};

    QString className  = classJObject["name"].toString();
    QString fileName   = classJObject["fileName"].toString();
    QString outputPath = classJObject["outputPath"].toString();
    /// only relevant for interface class
    QString objectName = classJObject["displayName"].toString();
    QString tooltip    = classJObject["tooltip"].toString();

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
    classStruct.functions    = searchForFunctions(functionsJArray);
    classStruct.constructors = searchForConstructors(constructorJArray);

    LOG_INFO << "done!";

    return classStruct;
}

ClassData
ModuleGeneratorPreLoader::searchForClass(QString const& fileName)
{
    QDir tmpDir{":/interfaces"};

    if (!tmpDir.exists(fileName))
    {
        return {};
    }

    auto document = QJsonDocument::fromJson(utils::readFile(
                        tmpDir.absoluteFilePath(fileName)).toUtf8());

    if (document.isNull())
    {
        return {};
    }

    auto interfaceJObject = document.object();

    return searchForClass(interfaceJObject);
}

Constructors
ModuleGeneratorPreLoader::searchForConstructors(QJsonArray const& constructorJArray)
{
    if (constructorJArray.isEmpty()) return {};

    LOG_INDENT("adding custom constructors...");

    Constructors constructors;

    for (auto constructorJObj : constructorJArray)
    {
        auto constructorJObject = constructorJObj.toObject();

        if (constructorJObject.isEmpty())
        {
            LOG_WARN << "empty constructor!";
            continue;
        }

        auto parameters = parseStringJArray(constructorJObject["parameters"].toArray());
        auto initList = parseStringJArray(constructorJObject["initList"].toArray());
        auto implementation = parseStringJArray(constructorJObject["source"].toArray());

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
ModuleGeneratorPreLoader::searchForFunctions(QJsonArray const& functionsJArray)
{
    LOG_INDENT("searching for functions...");

    if (functionsJArray.isEmpty())
    {
        return {};
    }

    FunctionDataList functions;

    for (auto functionJValue : functionsJArray)
    {
        auto functionJObject = functionJValue.toObject();

        if (functionJObject.isEmpty())
        {
            LOG_WARN << "empty function!";
            continue;
        }

        QString name        = functionJObject["name"].toString();
        QString returnValue = functionJObject["returnValue"].toString();
        QString tooltip     = functionJObject["tooltip"].toString();

        QJsonArray parameter    = functionJObject["parameters"].toArray();
        QJsonArray includes     = functionJObject["includes"].toArray();
        QJsonArray forwardDecls     = functionJObject["forwardDecl"].toArray();
        QJsonArray descriptionArray = functionJObject["description"].toArray();

        auto baseClassName = functionJObject["baseClass"].toString();
        auto linkedClassName = functionJObject["linkedClass"].toString();

        LOG_INFO << "function: " << returnValue << " " << name << ENDL;

        /* Implementation */
        ImplementationData implementation;

        implementation.includes = parseStringJArray(includes);
        implementation.forwardDeclarations = parseStringJArray(forwardDecls);

//        LOG_INFO << "inlcudes: '"
//                 << implementation.includes.join(", ")
//                 << "'" << ENDL;
//        LOG_INFO << "forward Decl: '"
//                 << implementation.forwardDeclarations.join(", ")
//                 << "'" << ENDL;

        /* Function */
        FunctionData function;
        function.name = std::move(name);
        function.returnType = std::move(returnValue);
        function.isConst     = functionJObject["const"].toBool();
        function.isProtected = functionJObject["protected"].toBool();
        function.parameters  = parseStringJArray(parameter);
        function.description = parseDescription(descriptionArray);
        function.tooltip     = std::move(tooltip);
        function.implementation = std::move(implementation);

        if (!baseClassName.isEmpty())
        {
            baseClassName = "class_definitions/" + baseClassName + ".json";
            function.baseClass   = searchForClass(baseClassName);
        }

        if (!linkedClassName.isEmpty())
        {
            linkedClassName = "class_definitions/" + linkedClassName + ".json";
            function.linkedClass   = searchForClass(linkedClassName);
        }

        functions.append(std::move(function));
    }

    LOG_INFO << "done!";

    return functions;
}

QStringList
ModuleGeneratorPreLoader::parseStringJArray(QJsonArray const& jArray)
{
    QStringList retVal;
    for (auto valueJObj : jArray)
    {
        retVal << valueJObj.toString();
    }
    return retVal;
}

QString
ModuleGeneratorPreLoader::parseDescription(QJsonArray const& descriptionJArray)
{
    if (descriptionJArray.isEmpty()) return {};

    QString description{"\n"
                        "\t/**"};

    description += parseStringJArray(descriptionJArray).join("\n\t * ");

    // end of description
    return description + "\n"
                         "\t */";
}

void
ModuleGeneratorPreLoader::searchForPrefixes(QString const& devToolsPath)
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
            QString fileName = iterator.next();

            auto match = REGEXP_PREFIX.match(fileName);

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

void
ModuleGeneratorPreLoader::searchForDependencies(QString const& gtlabPath,
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

    if (!gtlabDir.exists(processName))
    {
        LOG_ERR << "invalid path to GTlabConsole ("
                << gtlabDir.absolutePath()
                << processName << ")!";
        return;
    }

    // start process
    QProcess process;
    process.start(gtlabDir.absoluteFilePath(processName), S_GTLAB_CONSOLE_ARGS);

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

void
ModuleGeneratorPreLoader::clearInterfaceStructs()
{
    m_interfaces.clear();
}
