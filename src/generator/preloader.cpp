#include "preloader.h"
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

PreLoader::~PreLoader()
{
    clearInterfaceStructs();
}

const QString S_INCLUDE_CORE_PATH = QStringLiteral("core");
const QString S_INCLUDE_PATH      = QStringLiteral("include");

const QStringList S_GTLAB_CONSOLE_ARGS = QStringList(QStringLiteral("--footprint"));

const int I_PROCESS_TIMEOUT_MS = 15000;

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
PreLoader::searchForInterfaces()
{
    LOG_INSTANCE("searching for interfaces...");

    clearInterfaceStructs();

    QDirIterator iterator(":/interfaces/", QStringList() << "*.json", QDir::Files, QDirIterator::NoIteratorFlags);

    while (iterator.hasNext())
    {
        QString file = iterator.next();

        LOG_INSTANCE(file);

        auto document = QJsonDocument::fromJson(utils::readFile(file).toUtf8());

        if (document.isNull())
        {
            LOG_ERR << "null json document!";
            continue;
        }

        auto interfaceJObject = document.object();

        auto interfaceLocal = searchForClass(interfaceJObject);

        if (!interfaceLocal.isValid()) continue;

        m_interfaces << new ClassStruct(interfaceLocal);

        LOG_INFO << "done!";
    }

    LOG_INFO << "done!";
}

ClassStruct
PreLoader::searchForClass(const QJsonObject& classJObject)
{
    if (classJObject.isEmpty()) return ClassStruct();

    QString className  = classJObject["name"].toString();
    QString fileName   = classJObject["fileName"].toString();
    QString outputPath = classJObject["outputPath"].toString();
    QString objectName = classJObject["shortName"].toString();

    auto functionsJArray = classJObject["functions"].toArray();

    LOG_INSTANCE("className  " + className);

    auto functions = searchForFunctions(functionsJArray);

    ClassStruct classStruct;
    classStruct.className  = className;
    classStruct.fileName   = fileName;
    classStruct.objectName = objectName;
    classStruct.outputPath = outputPath;
    classStruct.functions  = functions;

    LOG_INFO << "done!";

    return classStruct;
}

FunctionStructs
PreLoader::searchForFunctions(const QJsonArray& functionsJArray)
{
    LOG_INSTANCE("searching for functions...");

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
        QString description = functionJObject["description"].toString();
        QJsonArray includes     = functionJObject["includes"].toArray();
        QJsonArray forwardDecls = functionJObject["forwardDecl"].toArray();

        auto baseClassJObject = functionJObject["baseClass"].toObject();

        LOG_INFO << returnValue << " " << name << ENDL;

        FunctionStruct function;

        function.name = name;
        function.returnValue = returnValue;
        function.qualifier   = qualifier;
        function.parameter   = parameter;
        function.description = description;
        function.baseClass   = searchForClass(baseClassJObject);

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
PreLoader::searchForPrefixes(const QString& devToolsPath)
{
    LOG_INSTANCE("searching for occupied prefixes...");    

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
PreLoader::searchForDependencies(const QString& gtlabPath)
{    
    LOG_INSTANCE("searching for dependencies...");

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
        process.terminate();
        return;
    }

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

        DependencieStruct dependencie;

        for (int j = 0; j < entries.length(); ++j)
        {
            QDomElement entry(entries.at(j).toElement());

            if (entry.tagName() == QStringLiteral("id"))
            {
                dependencie.name = entry.text();
            }
            else
            {
                dependencie.version = entry.text();
            }
        }

        m_dependencies << dependencie;

        LOG_INFO << dependencie.name << ": " << dependencie.version << ENDL;
    }

    LOG_INFO << QString::number(m_dependencies.count())
             << " reserved dependencies found!";
}

void
PreLoader::clearInterfaceStructs()
{
    for (auto interfaceStruct : m_interfaces)
    {
        if (interfaceStruct == Q_NULLPTR) continue;

        delete interfaceStruct;
    }

    m_interfaces.clear();
}
