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
const QRegularExpression REGEXP_PREFIX(QStringLiteral("(\\w+?)_\\w+\\.h"));


void
ModuleGeneratorPreLoader::searchForInterfaces()
{
    LOG_INDENT("searching for interfaces...");

    clearInterfaceStructs();

    QDirIterator iterator(":/interfaces/", QStringList{"*.json"},
                          QDir::Files, QDirIterator::NoIteratorFlags);

    while (iterator.hasNext())
    {
        QString file = iterator.next();

        LOG_INDENT(file);

        auto document = QJsonDocument::fromJson(utils::readFile(file).toUtf8());

        if (document.isNull())
        {
            LOG_ERR << "null json document!";
            continue;
        }

        auto interfaceJObject = document.object();

        auto interfaceLocal = searchForClass(interfaceJObject);

        if (!interfaceLocal.isValid()) continue;

        m_interfaces << interfaceLocal;

        LOG_INFO << "done!";
    }

    LOG_INFO << "done!";
}

ClassStruct
ModuleGeneratorPreLoader::searchForClass(QJsonObject const& classJObject)
{
    if (classJObject.isEmpty()) return ClassStruct();

    QString className  = classJObject["name"].toString();
    QString fileName   = classJObject["fileName"].toString();
    QString outputPath = classJObject["outputPath"].toString();
    QString objectName = classJObject["shortName"].toString();

    auto functionsJArray   = classJObject["functions"].toArray();
    auto constructorJArray = classJObject["constructors"].toArray();

    LOG_INDENT("className  " + className);

    auto functions = searchForFunctions(functionsJArray);

    ClassStruct classStruct;
    classStruct.className  = className;
    classStruct.fileName   = fileName;
    classStruct.objectName = objectName;
    classStruct.outputPath = outputPath;
    classStruct.functions  = functions;

    classStruct.constructors = searchForConstructors(constructorJArray);

    LOG_INFO << "done!";

    return classStruct;
}

Constructors
ModuleGeneratorPreLoader::searchForConstructors(QJsonArray const& constructorJObject)
{
    Constructors constructors;

    if (constructorJObject.isEmpty()) return constructors;

    LOG_INDENT("adding custom constructors...");

    for (auto jsonValueRef : constructorJObject)
    {
        auto constructorJObject = jsonValueRef.toObject();

        if (constructorJObject.isEmpty())
        {
            LOG_WARN << "empty constructor!";
            continue;
        }

        QString parameter = constructorJObject["parameter"].toString();
        auto sourceArray  = constructorJObject["source"].toArray();

        // constructor implementation
        QStringList source;

        for (auto jsonValueRef : sourceArray)
        {
            source << jsonValueRef.toString();
        }

        LOG_INFO << "..." << source.join("; ") << ENDL;

        constructors << Constructor({ parameter, source });
    }

    LOG_INFO << "done!";

    return constructors;
}

FunctionStructs
ModuleGeneratorPreLoader::searchForFunctions(QJsonArray const& functionsJArray)
{
    LOG_INDENT("searching for functions...");

    FunctionStructs functions;

    if (functionsJArray.isEmpty())
    {
        LOG_WARN << "empty function array!";
        return functions;
    }

    for (auto jsonValueRef : functionsJArray)
    {
        auto functionJObject = jsonValueRef.toObject();

        if (functionJObject.isEmpty())
        {
            LOG_WARN << "empty function!";
            continue;
        }

        QString name        = functionJObject["name"].toString();
        QString returnValue = functionJObject["returnValue"].toString();
        QString qualifier   = functionJObject["qualifier"].toString();
        QString parameter   = functionJObject["parameter"].toString();
        QString tooltip     = functionJObject["tooltip"].toString();

        QJsonArray includes     = functionJObject["includes"].toArray();
        QJsonArray forwardDecls = functionJObject["forwardDecl"].toArray();
        QJsonArray descriptionArray = functionJObject["description"].toArray();

        auto baseClassJObject   = functionJObject["baseClass"].toObject();
        auto linkedClassJObject = functionJObject["linkedClass"].toObject();

        // description
        QString description;

        for (auto jsonValueRef : descriptionArray)
        {
            QString line = jsonValueRef.toString();

            if (line.isEmpty()) continue;

            description += "\n\t" + line;
        }

        LOG_INFO << returnValue << " " << name << ENDL;

        FunctionStruct function;

        function.name = name;
        function.returnValue = returnValue;
        function.qualifier   = qualifier;
        function.parameter   = parameter;
        function.description = description;
        function.tooltip     = tooltip;
        function.baseClass   = searchForClass(baseClassJObject);
        function.linkedClass = searchForClass(linkedClassJObject);

        // includes
        for (auto jsonValueRef : includes)
        {
            QString include = jsonValueRef.toString();

            if (!include.isEmpty())
            {
                LOG_INFO << "added inlcude '" << include << "'" << ENDL;

                function.implementation.includes << include;
            }
        }

        // forward declarations
        for (auto jsonValueRef : forwardDecls)
        {
            QString forwardDecl = jsonValueRef.toString();

            if (!forwardDecl.isEmpty())
            {
                LOG_INFO << "added forward declaration '" << forwardDecl << "'" << ENDL;

                function.implementation.forwardDeclarations << forwardDecl;
            }
        }

        functions << function;
    }

    LOG_INFO << "done!";

    return functions;
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

    QDirIterator dirIterator(devToolsDir.absolutePath(), QStringList(),
                             QDir::Dirs, QDirIterator::NoIteratorFlags);

    while (dirIterator.hasNext())
    {
        QDir subDir = dirIterator.next();

        QDirIterator iterator(subDir.absolutePath(), QStringList() << "*.h",
                              QDir::Files, QDirIterator::NoIteratorFlags);

        while (iterator.hasNext())
        {
            QString fileName = iterator.next();

            auto match = REGEXP_PREFIX.match(fileName);

            QString prefix = match.captured(1);

            if (prefix.isEmpty()) continue;

            if (m_prefixes.indexOf(prefix) < 0)
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

    m_searchPath = gtlabPath;

    m_dependencies.clear();

    QDir gtlabDir(gtlabPath);

    QString processName(ModuleGeneratorSettings::S_GTLAB_CONSOLE_APP);

    if (!gtlabDir.exists(processName))
    {
        LOG_ERR << "invlaid path to GTlabConsole ("
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
