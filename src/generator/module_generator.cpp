#include "module_generator.h"

#include "module_generator_utils.h"
#include "module_generator_settings.h"
#include "module_generator_logger.h"

#include <QTime>
#include <QDir>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>

const QString S_TEMPLATES_PATH = QStringLiteral(":/templates/");
const QString S_SUB_DIR_PATH   = QStringLiteral("/src");

const QString ModuleGenerator::S_ID_SIGNATURE(QStringLiteral("$$SIGNATURE$$"));
const QString ModuleGenerator::S_ID_GENERATOR_VERSION(QStringLiteral("$$GENERATOR_VERSION$$"));
const QString ModuleGenerator::S_ID_INCLUDE_FILE(QStringLiteral("$$INCLUDE_FILE$$"));
const QString ModuleGenerator::S_ID_QT_INCLUDE_FILE(QStringLiteral("$$QT_INCLUDE_FILE$$"));

const QString ModuleGenerator::S_ID_BASE_CLASS(QStringLiteral("$$BASE_CLASS$$"));
const QString ModuleGenerator::S_ID_BASE_FILE_NAME(QStringLiteral("$$BASE_FILE_NAME$$"));
const QString ModuleGenerator::S_ID_DERIVE_BASE_CLASS(QStringLiteral("$$DERIVE_BASE_CLASS$$"));

const QString ModuleGenerator::S_ID_CLASS_NAME(QStringLiteral("$$CLASS_NAME$$"));
const QString ModuleGenerator::S_ID_OBJECT_NAME(QStringLiteral("$$OBJECT_NAME$$"));
const QString ModuleGenerator::S_ID_FILE_NAME(QStringLiteral("$$FILE_NAME$$"));
const QString ModuleGenerator::S_ID_HEADER_NAME(QStringLiteral("$$HEADER_NAME$$"));
const QString ModuleGenerator::S_ID_FUNCTION(QStringLiteral("$$FUNCTION$$"));
const QString ModuleGenerator::S_ID_IMPLEMENTATION(QStringLiteral("$$IMPLEMENTATION$$"));

const QString ModuleGenerator::S_ID_MODULE_NAME(QStringLiteral("$$MODULE_NAME$$"));
const QString ModuleGenerator::S_ID_MODULE_DESCRIPTION(QStringLiteral("$$MODULE_DESCRIPTION$$"));
const QString ModuleGenerator::S_ID_MODULE_VERSION(QStringLiteral("$$MODULE_VERSION$$"));
const QString ModuleGenerator::S_ID_INTERFACE_MACRO(QStringLiteral("$$INTERFACE_MACRO$$"));

const QString ModuleGenerator::S_ID_GTLAB_INSTALL_DIR(QStringLiteral("$$GTLAB_INSTALL_DIR$$"));
const QString ModuleGenerator::S_ID_DEVTOOLS_INSTALL_DIR(QStringLiteral("$$DEVTOOLS_INSTALL_DIR$$"));

const QString ModuleGenerator::S_ID_PRO_INCLUDE_PATH(QStringLiteral("$$PRO_INCLUDEPATH$$"));
const QString ModuleGenerator::S_ID_PRO_HEADER_PATH(QStringLiteral("$$PRO_HEADERPATH$$"));
const QString ModuleGenerator::S_ID_PRO_SOURCE_PATH(QStringLiteral("$$PRO_SOURCEPATH$$"));

const QString ModuleGenerator::S_ID_AUTHOR(QStringLiteral("$$AUTHOR$$"));

const QString S_INTERFACE_MACRO(QStringLiteral("Q_INTERFACES(") +
                                                ModuleGenerator::S_ID_BASE_CLASS +
                                                QStringLiteral(")\n\t"));
const QString S_DERIVE_BASE_CLASS(QStringLiteral(",\n\t\tpublic ") +
                                                 ModuleGenerator::S_ID_BASE_CLASS);


void
ModuleGenerator::onGenerate()
{
    QTime time;

    time.start();

    LOG_INSTANCE("generating...");

    int result = generate();

    LOG_INFO << "time passed: " << QString::number(time.elapsed()) + " (ms)" << ENDL;
    LOG_INFO << "module creation " << (result ? "succeeded!":"failed!");

    emit generationFinished();
}

int ModuleGenerator::generate()
{
    bool result = generateModulePath();

    if (!result) return false;

    result = generateModuleSettingsFiles();

    if (!result) return false;

    result = generateModuleProjectFile();

    if (!result) return false;

    result = generateModuleDependencies();

    if (!result) return false;

    result = generateModule();

    clearProjectFileIdentifiers();

    return result;
}

bool
ModuleGenerator::generateModulePath()
{
    LOG_INSTANCE("generating module path...");

    QString subFolder = m_settings->moduleClass().ident;
    subFolder.replace(" ", "");

    QString path;
    path += m_settings->outputPath() + QDir::separator();
    path += subFolder + QDir::separator();

    m_outputDir = QDir::cleanPath(path);
    m_moduleDir = m_outputDir;
    m_outputDir = QDir::cleanPath(path + S_SUB_DIR_PATH);

    if (m_moduleDir.exists() && m_moduleDir.count() > 0)
    {
        LOG_WARN << "path already exists!" << ENDL;

        QMessageBox msgBox;
        QPushButton* proceedButton = msgBox.addButton(tr("Proceed"), QMessageBox::AcceptRole);
        QPushButton* abortButton   = msgBox.addButton(QMessageBox::Abort);

        msgBox.setWindowTitle(QStringLiteral("Output path does already exists"));
        msgBox.setText(QStringLiteral("The output path does already exists and is not empty!\n"
                                      "Please remove all old files and proceed."));
        msgBox.exec();

        delete proceedButton;
        delete abortButton;

        if (msgBox.clickedButton() == abortButton)
        {
            LOG_ERR << "aborted!";

            return false;
        }

        LOG_INFO << "continued!" << ENDL;
    }

    m_outputDir.mkpath(m_outputDir.absolutePath());

    if (!m_outputDir.exists())
    {
        LOG_ERR << "could not create src path!";

        return false;
    }

    LOG_INFO << "done!";

    return m_moduleDir.exists();
}

bool
ModuleGenerator::generateModuleSettingsFiles()
{
    LOG_INSTANCE("generating .pri files...");

    auto fileString = utils::readFile(S_TEMPLATES_PATH + "settings.pri");

    utils::writeStringToFile(fileString, m_moduleDir, "settings.pri");

    fileString = utils::readFile(S_TEMPLATES_PATH + "local_settings.pri");

    IdentifierPairs identifierPairs;

    identifierPairs.append({ S_ID_GTLAB_INSTALL_DIR, m_settings->gtlabPath() });
    identifierPairs.append({ S_ID_DEVTOOLS_INSTALL_DIR, m_settings->devToolsPath() });

    utils::replaceIdentifier(fileString, identifierPairs);

    utils::writeStringToFile(fileString, m_moduleDir, "local_settings.pri");

    LOG_INFO << "done!";

    return true;
}

bool
ModuleGenerator::generateModuleProjectFile()
{
    LOG_INSTANCE("generating project file...");

    auto fileString = utils::readFile(S_TEMPLATES_PATH + "module.pro");

    IdentifierPairs identifierPairs;

    auto moduleClass = m_settings->moduleClass();

    identifierPairs.append({ S_ID_CLASS_NAME, moduleClass.className });
    identifierPairs.append({ S_ID_FILE_NAME, moduleClass.fileName });

    utils::replaceIdentifier(fileString, identifierPairs);

    utils::writeStringToFile(fileString, m_moduleDir, moduleClass.fileName + ".pro");

    LOG_INFO << "done!";

    return true;
}

bool
ModuleGenerator::generateModuleDependencies()
{
    LOG_INSTANCE("generating dependencies...");

    // for logging purpose
    {
        LOG_INSTANCE("creating json document...");

        QJsonArray dependenciesJsonArray;

        for (auto dependencie : m_settings->selectedDependencies())
        {
            LOG_INFO << dependencie.name << ENDL;

            QJsonObject dependencieJsonObject;

            dependencieJsonObject["name"] = dependencie.name;
            dependencieJsonObject["version"] = dependencie.version;

            dependenciesJsonArray.append(dependencieJsonObject);
        }

        QJsonObject dependenciesJsonObject;

        dependenciesJsonObject["dependencies"] = dependenciesJsonArray;

        QJsonDocument document(dependenciesJsonObject);

        QString fileString = document.toJson(QJsonDocument::Indented);

        utils::writeStringToFile(fileString, m_outputDir,
                                 m_settings->moduleClass().fileName + ".json");

        LOG_INFO << "done!";
    }

    LOG_INFO << "done!";

    return true;
}

bool
ModuleGenerator::generateModule()
{
    LOG_INSTANCE("generating module files...");

    auto headerString = utils::readFile(S_TEMPLATES_PATH + "basic_module_class.h");
    auto sourceString = utils::readFile(S_TEMPLATES_PATH + "basic_module_class.cpp");

    auto moduleClass = m_settings->moduleClass();

    // set the filename for the include to the module header file
    utils::replaceIdentifier(sourceString, { S_ID_FILE_NAME,
                                             moduleClass.fileName });

    IdentifierPairs identifierPairs;

    LOG_INFO << "implementing interfaces..." << ENDL;

    // IMPLEMENT INTERFACES
    for (auto* interface : m_settings->selectedInterfaces())
    {
        if (interface == Q_NULLPTR) continue;

        LOG_INSTANCE(interface->className);

        // HEADER ONLY
        identifierPairs.append({ S_ID_INCLUDE_FILE,
                                 utils::makeInclude(interface->fileName,
                                                    S_ID_INCLUDE_FILE) });
        identifierPairs.append({ S_ID_INTERFACE_MACRO,
                                 S_INTERFACE_MACRO + S_ID_INTERFACE_MACRO });
        identifierPairs.append({ S_ID_DERIVE_BASE_CLASS,
                                 S_DERIVE_BASE_CLASS + S_ID_DERIVE_BASE_CLASS });
        identifierPairs.append({ S_ID_BASE_CLASS, interface->className });

        utils::replaceIdentifier(headerString, identifierPairs);

        identifierPairs.clear();

        // IMPLEMENT FUNCTIONS
        for (auto* function : interface->functions)
        {
            if (function == Q_NULLPTR)
            {
                LOG_WARN << "null function!" << ENDL;
                continue;
            }

            generateFunction(headerString, sourceString, function);
        }

        LOG_INFO << "done!";
    }

    LOG_INFO << "setting identifiers..." << ENDL;

    // GtVersionNumber
    QString moduleVersion = moduleClass.version;
    moduleVersion.replace(".", ", ");
    if (moduleVersion.contains('-'))
    {
        moduleVersion.replace("-", ", \"");
        moduleVersion.append('"');
    }

    // FILL IDENTIFIER
    identifierPairs.append({ S_ID_SIGNATURE, m_settings->signatureString()  });
    identifierPairs.append({ S_ID_HEADER_NAME, moduleClass.fileName.toUpper() });
    identifierPairs.append({ S_ID_CLASS_NAME, moduleClass.className });
    identifierPairs.append({ S_ID_FILE_NAME, moduleClass.fileName });

    identifierPairs.append({ S_ID_MODULE_NAME, moduleClass.ident });
    identifierPairs.append({ S_ID_MODULE_VERSION, moduleVersion });
    identifierPairs.append({ S_ID_MODULE_DESCRIPTION, moduleClass.description });
    identifierPairs.append({ S_ID_AUTHOR, m_settings->authorDetails().name });
    identifierPairs.append({ S_ID_GENERATOR_VERSION, ModuleGeneratorSettings::S_VERSION });

    utils::replaceIdentifier(headerString, identifierPairs);

    utils::replaceIdentifier(sourceString, identifierPairs);

    LOG_INFO << "cleanup..." << ENDL;

    // REMOVE LEFT IDENTIFIERS
    clearIdentifiers(headerString);
    clearIdentifiers(sourceString);
    // REPLACE TABULATORS
    clearTabulators(headerString);
    clearTabulators(sourceString);

    utils::writeStringToFile(headerString, m_outputDir, moduleClass.fileName + ".h");
    utils::writeStringToFile(sourceString, m_outputDir, moduleClass.fileName + ".cpp");

    LOG_INFO << "done!";

    return true;
}

void
ModuleGenerator::generateFunction(QString& headerString,
                                  QString& sourceString,
                                  FunctionStruct* f)
{
    LOG_INSTANCE("generating function '" + f->name + "'...");

    LOG_INFO << "building header function definition..." << ENDL;

    // Build basic function string
    QString functionString;

    functionString += f->returnValue + " ";
    functionString += f->name;
    functionString += "(" + f->parameter + ") ";

    if (!f->qualifier.isEmpty())
    {
        functionString += f->qualifier + " ";
    }

    // HEADER
    utils::replaceIdentifier(headerString, { S_ID_FUNCTION,
                                             QString(f->description + "\n\t" +
                                                     functionString +
                                                     "Q_DECL_OVERRIDE;\n\n" +
                                                     S_ID_FUNCTION) });

    LOG_INFO << "building cpp function definition..." << ENDL;

    // CPP
    // remove default parameters in the cpp function declaration
    QString defaultparamPattern(QStringLiteral("\\s?=\\s?[\\w|:\\d]+"));
    QRegExp defaultparamRegExp(defaultparamPattern);

    functionString.remove(defaultparamRegExp);
    functionString.insert(f->returnValue.size() + 1, "\n" + S_ID_CLASS_NAME + "::");
    functionString += "\n{\n\t" + S_ID_IMPLEMENTATION + "\n}";

    utils::replaceIdentifier(sourceString, { S_ID_FUNCTION,
                                             functionString + ";\n\n" +
                                             S_ID_FUNCTION });

    generateImplementation(sourceString, *f);

    LOG_INFO << "done!";
}

void
ModuleGenerator::generateImplementation(QString& sourceString,
                                        FunctionStruct& function)
{
    LOG_INSTANCE("generating implementation...");

    ImplementationStruct& implementation = function.implementation;

    // function return value
    if (implementation.values.isEmpty())
    {
        LOG_WARN << "empty implementation!" << ENDL;

        return;
    }

    LOG_INFO << "setting return values..." << ENDL;

    QString implementationString;

    QStringList list = implementation.values;

    list.last().prepend("return ");
    list.last().append(';');

    implementationString = list.join(";\n\t");
    implementationString.replace("\t;", ""); // remove empty lines

    utils::replaceIdentifier(sourceString, { S_ID_IMPLEMENTATION,
                                             implementationString });

    generateQtIncludes(sourceString, function.returnValue);

    // classes to generate
    for (ClassStruct& derived : implementation.derivedClasses)
    {
        if (derived.className.isEmpty()) continue;

        generateBasicClass(function.baseClass, derived);

        utils::replaceIdentifier(sourceString,
            { S_ID_INCLUDE_FILE, utils::makeInclude(derived.fileName,
                                                    S_ID_INCLUDE_FILE) });
    }

    LOG_INFO << "done!";
}

void
ModuleGenerator::generateQtIncludes(QString& sourceString, QString& returnValue)
{
    // includes of Qt headers if neccessary
    QString paramPattern(QStringLiteral("<.+?>"));
    QRegExp paramRegExp(paramPattern);

    QString trimmedReturnValue = returnValue;

    // eg. QMap<const char*, QMetaData> -> QMap
    trimmedReturnValue.remove(paramRegExp);

    if (trimmedReturnValue == QStringLiteral("QIcon") ||
        trimmedReturnValue == QStringLiteral("QMap") ||
        trimmedReturnValue == QStringLiteral("QVariant"))
    {


        utils::replaceIdentifier(sourceString,
            { S_ID_INCLUDE_FILE, utils::makeInclude(trimmedReturnValue,
                                                    S_ID_QT_INCLUDE_FILE,
                                                    true) });
    }
}

void
ModuleGenerator::generateBasicClass(ClassStruct* base, ClassStruct& derived)
{
    LOG_INSTANCE("generating class '" + derived.className + "'...");

    LOG_INFO << "creating file strings..." << ENDL;

    auto headerString = utils::readFile(S_TEMPLATES_PATH + QStringLiteral("basic_derived_class.h"));
    auto sourceString = utils::readFile(S_TEMPLATES_PATH + QStringLiteral("basic_derived_class.cpp"));

    // set the filename for the include to the module header file
    utils::replaceIdentifier(sourceString, { S_ID_FILE_NAME, derived.fileName });

    IdentifierPairs identifierPairs;

    // IMPLEMENT INTERFACES
    for (auto* function : base->functions)
    {
        generateFunction(headerString, sourceString, function);
    }

    LOG_INFO << "setting identifiers..." << ENDL;

    // FILL IDENTIFIER
    identifierPairs.append({ S_ID_SIGNATURE, m_settings->signatureString() });
    identifierPairs.append({ S_ID_BASE_FILE_NAME, base->fileName});
    identifierPairs.append({ S_ID_BASE_CLASS,
                             QStringLiteral("public ") + base->className });
    identifierPairs.append({ S_ID_HEADER_NAME, derived.className.toUpper() });
    identifierPairs.append({ S_ID_CLASS_NAME, derived.className });
    identifierPairs.append({ S_ID_OBJECT_NAME, derived.objectName });
    identifierPairs.append({ S_ID_AUTHOR, m_settings->authorDetails().name });
    identifierPairs.append({ S_ID_GENERATOR_VERSION, ModuleGeneratorSettings::S_VERSION });

    utils::replaceIdentifier(headerString, identifierPairs);

    utils::replaceIdentifier(sourceString, identifierPairs);

    LOG_INFO << "cleanup..." << ENDL;

    // REMOVE LEFT IDENTIFIERS
    clearIdentifiers(headerString);
    clearIdentifiers(sourceString);
    // REPLACE TABULATORS
    clearTabulators(headerString);
    clearTabulators(sourceString);

    LOG_INFO << "writing files..." << ENDL;

    QDir targetDir = m_outputDir.cleanPath(
                QString(m_outputDir.absolutePath() + '/' + base->outputPath));

    targetDir.mkpath(targetDir.absolutePath());

    utils::writeStringToFile(headerString, targetDir, derived.fileName + ".h");
    utils::writeStringToFile(sourceString, targetDir, derived.fileName + ".cpp");

    appendProjectFile(derived.fileName, base->outputPath);

    LOG_INFO << "done!";
}

void
ModuleGenerator::appendProjectFile(QString& fileName, const QString& path)
{
    LOG_INSTANCE("adding class to project file...");

    auto moduleClass = m_settings->moduleClass();

    auto fileString = utils::readFile(
                m_moduleDir.absoluteFilePath(moduleClass.fileName + ".pro"));

    IdentifierPairs identifierPairs;

    QString includePath(QStringLiteral("src/") + path);

    if (!m_proFileIncludePaths.contains(includePath))
    {
        m_proFileIncludePaths.append(includePath);

        identifierPairs.append({ S_ID_PRO_INCLUDE_PATH,
                                 QString("\\\n\t" + includePath + " " +
                                         S_ID_PRO_INCLUDE_PATH) });
    }

    identifierPairs.append({ S_ID_PRO_HEADER_PATH,
                             QString("\\\n\t" + includePath + '/' + fileName +
                                     ".h " + S_ID_PRO_HEADER_PATH) });
    identifierPairs.append({ S_ID_PRO_SOURCE_PATH,
                             QString("\\\n\t" + includePath + '/' + fileName +
                                     ".cpp " + S_ID_PRO_SOURCE_PATH) });

    utils::replaceIdentifier(fileString, identifierPairs);

    utils::writeStringToFile(fileString, m_moduleDir, moduleClass.fileName + ".pro");

    LOG_INFO << "done!";
}


void
ModuleGenerator::clearIdentifiers(QString& fileString)
{
    QRegularExpression regExp(QStringLiteral("\\$\\$.+\\$\\$"));

    fileString.remove(regExp);
}

void
ModuleGenerator::clearTabulators(QString &fileString)
{
    fileString.replace("\t", QStringLiteral("    "));
}

void
ModuleGenerator::clearProjectFileIdentifiers()
{
    auto moduleClass = m_settings->moduleClass();

    auto fileString = utils::readFile(
                m_moduleDir.absoluteFilePath(moduleClass.fileName + ".pro"));

    clearIdentifiers(fileString);
    clearTabulators(fileString);

    utils::writeStringToFile(fileString, m_moduleDir, moduleClass.fileName + ".pro");
}






















