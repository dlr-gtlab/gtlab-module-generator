#include "preloader.h"
#include "module_generator_utils.h"
#include "module_generator_logger.h"

#include <QDir>
#include <QDirIterator>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

PreLoader::~PreLoader()
{
    for (auto* interface : m_interfaces)
    {
        if (interface == Q_NULLPTR) continue;

        delete interface;
    }
}

const QString S_INCLUDE_CORE_PATH = QStringLiteral("core");
const QString S_INCLUDE_PATH      = QStringLiteral("include");

/* intended for auto detecting interfaces and functions

const QRegularExpression REGEXP_INTERFACE(QStringLiteral("Q_DECLARE_INTERFACE\\((\\w+),"));
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
PreLoader::searchForInterfaces(/*const QString& devToolsPath*/)
{
    LOG_INSTANCE("searching for interfaces...");

    clearInterfaceStructs();

    QDirIterator iterator(":/interfaces/", QStringList() << "*.json", QDir::Files, QDirIterator::NoIteratorFlags);

    while (iterator.hasNext())
    {
        QString file = iterator.next();

        LOG_INSTANCE(file);
//        LOG_INFO << file << ENDL;

        auto document = QJsonDocument::fromJson(utils::readFile(file).toUtf8());

        if (document.isNull())
        {
            LOG_ERR << "null json document!";
            continue;
        }

        auto interfaceJObject = document.object();

        auto* interface = searchForClass(interfaceJObject);

        if (interface == Q_NULLPTR)
        {
            LOG_ERR << "null interface!";
            continue;
        }

        m_interfaces << interface;

        LOG_INFO << "done!";
    }

    LOG_INFO << "done!";
}

ClassStruct*
PreLoader::searchForClass(const QJsonObject& classJObject)
{
//    LOG_INSTANCE("searching for class...");

    if (classJObject.isEmpty()) return Q_NULLPTR;

    QString className  = classJObject["name"].toString();
    QString fileName   = classJObject["fileName"].toString();
    QString outputPath = classJObject["outputPath"].toString();

    auto functionsJArray = classJObject["functions"].toArray();


    LOG_INSTANCE("className  " + className);
//    LOG_INFO << "fileName   " << fileName   << ENDL;
//    LOG_INFO << "outputpath "  << outputPath << ENDL;

    auto functions = searchForFunctions(functionsJArray);
    auto* classStruct = new ClassStruct;

    classStruct->className = className;
    classStruct->fileName = fileName;
    classStruct->outputPath = outputPath;
    classStruct->functions = functions;

    LOG_INFO << "done!";

    return classStruct;
}

FunctionStructsPtr
PreLoader::searchForFunctions(const QJsonArray& functionsJArray)
{
    LOG_INSTANCE("searching for functions...");

    FunctionStructsPtr functions;

    if (functionsJArray.isEmpty())
    {
        LOG_WARN << "empty function array!";
        return functions;
    }

    for (auto jsonValueRef : functionsJArray)
    {
//        LOG_INSTANCE("function...");

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

        auto baseClassJObject = functionJObject["baseClass"].toObject();

        LOG_INFO << returnValue << " " << name << ENDL;

//        LOG_INFO << "name        " << name        << ENDL;
//        LOG_INFO << "returnValue " << returnValue << ENDL;
//        LOG_INFO << "qualifier   " << qualifier   << ENDL;
//        LOG_INFO << "parameter   " << parameter   << ENDL;

        auto* baseClass  = searchForClass(baseClassJObject);

        auto* function = new FunctionStruct(*new ImplementationStruct);

        function->name = name;
        function->returnValue = returnValue;
        function->qualifier = qualifier;
        function->parameter = parameter;
        function->description = description;

        function->baseClass  = baseClass;

        functions << function;

//        LOG_INFO << "done!";
    }

    LOG_INFO << "done!";

    return functions;
}

void
PreLoader::searchForPrefixes(const QString& devToolsPath)
{
    LOG_INSTANCE("searching for occupied prefixes...");

    m_prefixes.clear();

    LOG_INFO << "chaning to dev tools dir..." << ENDL;

    QDir devToolsDir(devToolsPath);

    devToolsDir.cd(S_INCLUDE_PATH);

    if (!devToolsDir.exists())
    {
        LOG_ERR << "devtools dir does not exists!";

        return;
    }

    QDirIterator iterator(devToolsDir.absolutePath(), QStringList() << "*.h", QDir::Files, QDirIterator::Subdirectories);

    while (iterator.hasNext())
    {
        QString fileName = iterator.next();

        auto match = REGEXP_PREFIX.match(fileName);

        QString prefix = match.captured(1);

        if (prefix.isEmpty()) continue;

        m_prefixes.append(prefix);
    }

    m_prefixes.removeDuplicates();

    LOG_INFO << QString::number(m_prefixes.count()) << " reserved prefixes found";
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
