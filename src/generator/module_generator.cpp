#include "module_generator.h"

#include "module_generator_utils.h"
#include "module_generator_settings.h"
#include "module_generator_logger.h"

#include <QElapsedTimer>
#include <QDir>
#include <QDirIterator>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>
#include <QVersionNumber>

const QString
ModuleGenerator::S_ID_SIGNATURE(QStringLiteral("$$SIGNATURE$$"));
const QString
ModuleGenerator::S_ID_GENERATOR_VERSION(QStringLiteral("$$GENERATOR_VERSION$$"));
const QString
ModuleGenerator::S_ID_INCLUDE_FILE(QStringLiteral("$$INCLUDE_FILE$$"));
const QString
ModuleGenerator::S_ID_QT_INCLUDE_FILE(QStringLiteral("$$QT_INCLUDE_FILE$$"));
const QString
ModuleGenerator::S_ID_ADD_INCLUDE_FILE(QStringLiteral("$$ADD_INCLUDE_FILE$$"));
const QString
ModuleGenerator::S_ID_CLASS_FORWARD_DECL(QStringLiteral("$$CLASS_FORWARD_DECL$$"));

const QString
ModuleGenerator::S_ID_BASE_CLASS(QStringLiteral("$$BASE_CLASS$$"));
const QString
ModuleGenerator::S_ID_BASE_FILE_NAME(QStringLiteral("$$BASE_FILE_NAME$$"));
const QString
ModuleGenerator::S_ID_DERIVE_BASE_CLASS(QStringLiteral("$$DERIVE_BASE_CLASS$$"));

const QString
ModuleGenerator::S_ID_PREFIX(QStringLiteral("$$PREFIX$$"));
const QString
ModuleGenerator::S_ID_CLASS_NAME(QStringLiteral("$$CLASS_NAME$$"));
const QString
ModuleGenerator::S_ID_OBJECT_NAME(QStringLiteral("$$OBJECT_NAME$$"));
const QString
ModuleGenerator::S_ID_FILE_NAME(QStringLiteral("$$FILE_NAME$$"));
const QString
ModuleGenerator::S_ID_HEADER_NAME(QStringLiteral("$$HEADER_NAME$$"));
const QString
ModuleGenerator::S_ID_CONSTRUCTOR(QStringLiteral("$$CONSTRUCTOR$$"));
const QString
ModuleGenerator::S_ID_CONSTRUCTOR_INIT_LIST(QStringLiteral("$$CONSTRUCTOR_INIT_LIST$$"));
const QString
ModuleGenerator::S_ID_FUNCTION(QStringLiteral("$$FUNCTION$$"));
const QString
ModuleGenerator::S_ID_IMPLEMENTATION(QStringLiteral("$$IMPLEMENTATION$$"));

const QString
ModuleGenerator::S_ID_MODULE_NAME(QStringLiteral("$$MODULE_NAME$$"));
const QString
ModuleGenerator::S_ID_MODULE_DESCRIPTION(QStringLiteral("$$MODULE_DESCRIPTION$$"));
const QString
ModuleGenerator::S_ID_MODULE_VERSION(QStringLiteral("$$MODULE_VERSION$$"));
const QString
ModuleGenerator::S_ID_INTERFACE_MACRO(QStringLiteral("$$INTERFACE_MACRO$$"));
const QString
ModuleGenerator::S_ID_GTLAB_MAJOR_VERSION(QStringLiteral("$$GTLAB_MAJOR_VERSION$$"));

const QString
ModuleGenerator::S_ID_GTLAB_INSTALL_DIR(QStringLiteral("$$GTLAB_INSTALL_DIR$$"));
const QString
ModuleGenerator::S_ID_GTLAB_INSTALL_BIN_DIR(QStringLiteral("$$GTLAB_INSTALL_BIN_DIR$$"));
const QString
ModuleGenerator::S_ID_DEVTOOLS_INSTALL_DIR(QStringLiteral("$$DEVTOOLS_INSTALL_DIR$$"));
const QString
ModuleGenerator::S_ID_TARGET_DIR_NAME(QStringLiteral("$$TARGET_DIR_NAME$$"));

const QString
ModuleGenerator::S_ID_PRO_INCLUDE_PATH(QStringLiteral("$$PRO_INCLUDEPATH$$"));
const QString
ModuleGenerator::S_ID_PRO_HEADER_PATH(QStringLiteral("$$PRO_HEADERPATH$$"));
const QString
ModuleGenerator::S_ID_PRO_SOURCE_PATH(QStringLiteral("$$PRO_SOURCEPATH$$"));
const QString
ModuleGenerator::S_ID_PRO_LIBS(QStringLiteral("$$PRO_LIBS$$"));
const QString
ModuleGenerator::S_ID_PRO_LIBS_D(QStringLiteral("$$PRO_LIBS_D$$"));

const QString
ModuleGenerator::S_ID_AUTHOR(QStringLiteral("$$AUTHOR$$"));
const QString
ModuleGenerator::S_ID_AUTHOR_EMAIL(QStringLiteral("$$AUTHOR_EMAIL$$"));

const QString
ModuleGenerator::S_ID_INDENT = QStringLiteral("$$INDENT$$");

const QString
ModuleGenerator::S_ID_COMPAT_FILE = QStringLiteral("$$COMPAT_FILE$$");

const QString
ModuleGenerator::S_ID_2_0_INCLUDE_ICON = QStringLiteral("$$2_0_INCLUDE_ICON$$");

const QString
ModuleGenerator::S_1_7_VERSION_CHECK =
        QStringLiteral("#ifdef COMPAT_VERSION_1_X");

const QString
ModuleGenerator::S_2_0_VERSION_CHECK =
        QStringLiteral("#ifdef COMPAT_VERSION_2_0");

const QStringList
ModuleGenerator::S_2_0_INCLUDE_ICON_LIST{
    "$$COMPAT_FILE$$",
    // prior GTlab 2.0
    S_1_7_VERSION_CHECK + "\n"
    "#include \"gt_application.h\"\n"
    "#else\n"
    "#include \"gt_icons.h\"\n"
    "#endif\n",
    "QIcon"
};

const ConstructorData
ModuleGenerator::G_CONSTRUCTOR_DEFAULT{
    {}, // params
    { QStringLiteral("setObjectName(\"") + S_ID_OBJECT_NAME +
      QStringLiteral("\")") },
    {} // init list
};

// paths
const QString
S_TEMPLATES_PATH = QStringLiteral(":/templates/");
const QString
S_SRC_DIR      = QStringLiteral("src");
const QString
S_FEATURES_DIR = QStringLiteral("features");

// for protected methods
const QString
S_PROTECTED_TAG = QStringLiteral("protected:");

// qt makros
const QString
S_INTERFACE_MACRO(
        QStringLiteral("Q_INTERFACES(") + ModuleGenerator::S_ID_BASE_CLASS +
        QStringLiteral(")\n\t"));
const QString
S_OVERRIDE(QStringLiteral(" override;"));
const QString
S_Q_INVOKABLE(QStringLiteral("Q_INVOKABLE "));

// helper strings
const QString
S_DERIVE_BASE_CLASS(
        QStringLiteral(",\n\t\tpublic ") + ModuleGenerator::S_ID_BASE_CLASS);
const QString
S_PRO_ENDL(QStringLiteral("\\\n\t"));

ModuleGeneratorSettings const*
ModuleGenerator::settings() const
{
    return &m_settings;
}

ModuleGeneratorSettings*
ModuleGenerator::settings()
{
    return &m_settings;
}

bool
ModuleGenerator::generate()
{
    QElapsedTimer timer;
    timer.start();

    LOG_INDENT("generating...");

    bool result = generateHelper();

    LOG_INFO << "time passed: " << QString::number(timer.elapsed()) + " (ms)"
             << ENDL;
    LOG_INFO << "module creation " << (result ? "succeeded!":"failed!");

    emit generationFinished();

    return result;
}

bool
ModuleGenerator::generateHelper()
{
    bool result(generateModulePath());
    if (!result) return false;

    result = generateModuleSettingsFiles();
    if (!result) return false;

    result = generateModuleProjectFile();
    if (!result) return false;

    result = generateGitFiles();
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
    LOG_INDENT("generating module path...");

    QString subFolder = settings()->moduleClass().ident;
    subFolder.replace(" ", "");

    QString path;
    path += settings()->outputPath() + QDir::separator();
    path += subFolder + QDir::separator();

    m_srcDir = QDir::cleanPath(path);
    m_moduleDir = m_srcDir;
    m_srcDir = QDir::cleanPath(path + QDir::separator() + S_SRC_DIR);
    m_featuresDir = QDir::cleanPath(path + QDir::separator() + S_FEATURES_DIR);

    // module dir
    if (m_moduleDir.exists() && m_moduleDir.count() > 0)
    {
        LOG_WARN << "path '" << m_moduleDir.path() << "' already exists!" << ENDL;

        QMessageBox msgBox;
        QScopedPointer<QPushButton> proceedButton{msgBox.addButton(
                        tr("Proceed"), QMessageBox::AcceptRole)};
        QScopedPointer<QPushButton> abortButton{msgBox.addButton(
                        QMessageBox::Abort)};

        msgBox.setWindowTitle(QStringLiteral("Output path already exists"));
        msgBox.setText(QStringLiteral("The output path already exists and "
                                      "is not empty!\nPlease remove all old "
                                      "files and proceed."));
        msgBox.exec();

        if (msgBox.clickedButton() == abortButton.get())
        {
            LOG_ERR << "aborted!";

            return false;
        }

        LOG_INFO << "continued!" << ENDL;
    }

    // src dir
    m_srcDir.mkpath(m_srcDir.absolutePath());

    if (!m_srcDir.exists())
    {
        LOG_ERR << "could not create '" << S_SRC_DIR << "/' path!" << ENDL;

        return false;
    }

    // features dir
    m_moduleDir.mkdir(m_featuresDir.absolutePath());

    if (!m_featuresDir.exists())
    {
        LOG_ERR << "could not create '" << S_FEATURES_DIR << "/' path!" << ENDL;
        // not returning false, as issue can be fixed by hand afterwards
    }

    LOG_INFO << "done!";

    return m_srcDir.exists();
}

bool
ModuleGenerator::generateModuleSettingsFiles()
{
    LOG_INDENT("generating .pri files...");


    // settings.pri
    auto fileString = utils::readFile(S_TEMPLATES_PATH + "settings.pri");

    auto const& prefix = settings()->modulePrefix();
    auto targetDirName = prefix.isEmpty() ? settings()->moduleClass().ident :
                                            prefix;

    utils::replaceIdentifier(fileString,
                             { S_ID_TARGET_DIR_NAME, targetDirName.remove(' ') });

    utils::writeStringToFile(fileString, m_moduleDir, "settings.pri");

    // deployment.pri
    fileString = utils::readFile(S_TEMPLATES_PATH + "deployment.pri");

    utils::writeStringToFile(fileString, m_moduleDir, "deployment.pri");

    // features/local_setting.pri
    fileString = utils::readFile(S_TEMPLATES_PATH + "local_settings.pri");

    QString featuresString = fileString;
    clearFileString(featuresString);

    utils::writeStringToFile(featuresString, m_featuresDir, "local_settings.pri");

    // local_setting.pri
    IdentifierPairs identifierPairs;

    QDir gtlabDir{settings()->gtlabPath()};

    identifierPairs.append({ S_ID_GTLAB_INSTALL_BIN_DIR, gtlabDir.dirName() });
    identifierPairs.append({ S_ID_DEVTOOLS_INSTALL_DIR,
                             QDir::cleanPath(settings()->devToolsPath()) });
    identifierPairs.append({ S_ID_GTLAB_MAJOR_VERSION,
                             QString::number(settings()->gtlabMajorVersion())});

    if (!(gtlabDir.cdUp() && gtlabDir.exists()))
    {
        LOG_ERR  << "could not set a valid path to GTlab install dir!" << ENDL;
        LOG_WARN << "continuing!" << ENDL;
        // not returning false, as issue can be fixed by hand afterwards
    }

    identifierPairs.append({ S_ID_GTLAB_INSTALL_DIR, gtlabDir.absolutePath() });

    utils::replaceIdentifier(fileString, identifierPairs);

    utils::writeStringToFile(fileString, m_moduleDir, "local_settings.pri");

    LOG_INFO << "done!";

    return true;
}

bool
ModuleGenerator::generateModuleProjectFile()
{
    LOG_INDENT("generating project file...");

    auto fileStringPro = utils::readFile(S_TEMPLATES_PATH + "module.pro");
    auto fileStringSrc = utils::readFile(S_TEMPLATES_PATH + "module_src.pro");

    IdentifierPairs identifierPairs;

    auto const& moduleClass = settings()->moduleClass();

    identifierPairs.append({ S_ID_MODULE_NAME, moduleClass.ident });
    identifierPairs.append({ S_ID_CLASS_NAME, moduleClass.className });
    identifierPairs.append({ S_ID_FILE_NAME, moduleClass.fileName });

    utils::replaceIdentifier(fileStringPro, identifierPairs);
    utils::replaceIdentifier(fileStringSrc, identifierPairs);

    utils::writeStringToFile(fileStringPro, m_moduleDir,
                             moduleClass.fileName + ".pro");
    utils::writeStringToFile(fileStringSrc, m_srcDir, "src.pro");

    LOG_INFO << "done!";

    return true;
}

bool
ModuleGenerator::generateGitFiles()
{
    if (!settings()->createGitFiles())
    {
        return true;
    }

    LOG_INDENT("generating git files...");

    // gitignore
    LOG_INFO << ".gitignore" << ENDL;
    auto fileString = utils::readFile(S_TEMPLATES_PATH + ".gitignore");

    utils::writeStringToFile(fileString, m_moduleDir, ".gitignore");

    // changelog
    LOG_INFO << "CHANGELOG.md" << ENDL;
    fileString = utils::readFile(S_TEMPLATES_PATH + "CHANGELOG.md");

    utils::writeStringToFile(fileString, m_moduleDir, "CHANGELOG.md");

    // readme
    LOG_INFO << "README.md" << ENDL;
    fileString = utils::readFile(S_TEMPLATES_PATH + "README.md");
    IdentifierPairs identifierPairs;

    auto moduleClass = settings()->moduleClass();

    identifierPairs.append({ S_ID_PREFIX, settings()->modulePrefix() });
    identifierPairs.append({ S_ID_MODULE_NAME, moduleClass.ident });
    identifierPairs.append({ S_ID_MODULE_DESCRIPTION, moduleClass.description });
    identifierPairs.append({ S_ID_MODULE_VERSION, moduleClass.version });
    identifierPairs.append({ S_ID_GENERATOR_VERSION, ModuleGeneratorSettings::S_VERSION });

    utils::replaceIdentifier(fileString, identifierPairs);

    utils::writeStringToFile(fileString, m_moduleDir, "README.md");

    LOG_INFO << "done!";

    return true;
}

bool
ModuleGenerator::generateModuleDependencies()
{
    LOG_INDENT("generating dependencies...");

    // for logging purpose
    {
        LOG_INDENT("creating json document...");

        QJsonArray dependenciesJsonArray;

        for (auto const& dependency : settings()->selectedDependencies())
        {
            LOG_INFO << dependency.name << ENDL;

            QJsonObject dependencyJsonObject;

            dependencyJsonObject["name"] = dependency.name;
            dependencyJsonObject["version"] = dependency.version;

            dependenciesJsonArray.append(dependencyJsonObject);
        }

        QJsonObject dependenciesJsonObject;

        dependenciesJsonObject["dependencies"] = dependenciesJsonArray;

        QJsonDocument document(dependenciesJsonObject);

        QString fileString = document.toJson(QJsonDocument::Indented);

        utils::writeStringToFile(fileString, m_srcDir,
                                 settings()->moduleClass().fileName + ".json");

        LOG_INFO << "done!";
    }

    for (auto const& dependency : settings()->selectedDependencies())
    {
        appendLibToProjectFile(dependency.name);
    }

    LOG_INFO << "done!";

    return true;
}

bool
ModuleGenerator::generateModule()
{
    LOG_INDENT("generating module files...");

    auto headerString = utils::readFile(S_TEMPLATES_PATH +
                                        "basic_module_class.h");
    auto sourceString = utils::readFile(S_TEMPLATES_PATH +
                                        "basic_module_class.cpp");

    auto moduleClass = settings()->moduleClass();

    // set the filename for the include to the module header file
    utils::replaceIdentifier(sourceString, { S_ID_FILE_NAME,
                                             moduleClass.fileName });

    QString compatFileName = settings()->fileNamingScheme("compat");

    // only generate file if compatibility is enabled or 1.7 is targeted
    if (settings()->gtlabMajorVersion() < 2 ||
        settings()->useCompatibilityMacros())
    {
        LOG_INFO << "generating compatibility file..." << ENDL;

        auto compatFileString = utils::readFile(S_TEMPLATES_PATH + "compat.h");

        utils::replaceIdentifier(compatFileString,
                                 { S_ID_HEADER_NAME, compatFileName.toUpper() });

        appendFileToProjectFile(compatFileName, true);

        clearFileString(compatFileString);

        utils::writeStringToFile(compatFileString, m_srcDir, compatFileName + ".h");
    }

    IdentifierPairs identifierPairs;

    LOG_INFO << "implementing interfaces..." << ENDL;

    // IMPLEMENT INTERFACES
    for (auto const& interface : settings()->selectedInterfaces())
    {
        if (!interface.isValid()) continue;

        LOG_INDENT(interface.className);

        // HEADER ONLY
        identifierPairs.append({ S_ID_INCLUDE_FILE,
                                 utils::makeInclude(interface.fileName,
                                                    S_ID_INCLUDE_FILE) });
        identifierPairs.append({ S_ID_INTERFACE_MACRO,
                                 S_INTERFACE_MACRO + S_ID_INTERFACE_MACRO });
        identifierPairs.append({ S_ID_DERIVE_BASE_CLASS,
                                 S_DERIVE_BASE_CLASS + S_ID_DERIVE_BASE_CLASS });
        identifierPairs.append({ S_ID_BASE_CLASS, interface.className });

        utils::replaceIdentifier(headerString, identifierPairs);

        identifierPairs.clear();

        // IMPLEMENT FUNCTIONS
        for (auto const& function : interface.functions)
        {
            if (!function.isValid())
            {
                LOG_WARN << "skipping invalid function!" << ENDL; continue;
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
    identifierPairs.append({ S_ID_SIGNATURE, ModuleGeneratorSettings::S_SIGNATURE });
    identifierPairs.append({ S_ID_HEADER_NAME, moduleClass.fileName.toUpper() });
    identifierPairs.append({ S_ID_CLASS_NAME, moduleClass.className });
    identifierPairs.append({ S_ID_FILE_NAME, moduleClass.fileName });
    identifierPairs.append({ S_ID_COMPAT_FILE, compatFileName });

    identifierPairs.append({ S_ID_MODULE_NAME, moduleClass.ident });
    identifierPairs.append({ S_ID_MODULE_VERSION, moduleVersion });
    identifierPairs.append({ S_ID_MODULE_DESCRIPTION, moduleClass.description });
    identifierPairs.append({ S_ID_AUTHOR, settings()->authorDetails().name });
    identifierPairs.append({ S_ID_AUTHOR_EMAIL, settings()->authorDetails().email });
    identifierPairs.append({ S_ID_GENERATOR_VERSION, ModuleGeneratorSettings::S_VERSION });

    utils::replaceIdentifier(headerString, identifierPairs);

    utils::replaceIdentifier(sourceString, identifierPairs);

    LOG_INFO << "cleanup..." << ENDL;

    // CLEANUP
    clearFileString(headerString);
    clearFileString(sourceString);

    utils::writeStringToFile(headerString, m_srcDir, moduleClass.fileName + ".h");
    utils::writeStringToFile(sourceString, m_srcDir, moduleClass.fileName + ".cpp");

    LOG_INFO << "done!";

    return true;
}

void
ModuleGenerator::generateFunction(QString& headerString,
                                  QString& sourceString,
                                  FunctionData const& function,
                                  bool isConstructor)
{
    auto fname = isConstructor ? "constructor":
                                 "function '" + function.name +"'";

    LOG_INDENT("generating " + fname + "...");

    // Build basic function string
    auto functionString = function.name;

    // add ident for each parameter
    int indent = 2 + function.name.length();
    indent += isConstructor ? S_Q_INVOKABLE.length() - 1 :
                              function.returnType.length();

    functionString += "(";
    functionString += function.parameters.join(",\n\t" +
                                               QString{" "}.repeated(indent));
    functionString += ")";

    if (function.isConst)
    {
        functionString += " const";
    }

    /* HEADER */
    LOG_INFO << "building header function definition..." << ENDL;

    QString functionHeader("\n");

    if (function.isProtected)
    {
        functionHeader += "\n" + S_PROTECTED_TAG + "\n";
    }

    // description
    if (!function.description.isEmpty())
    {
        functionHeader += function.description;
    }

    functionHeader += "\n\t";

    // function declaration
    if (!isConstructor) {
        functionHeader += function.returnType + " ";
        functionHeader += functionString;
        functionHeader += S_OVERRIDE;
    }
    else {
        functionHeader += S_Q_INVOKABLE;
        functionHeader += functionString +";";
    }

    auto indentifier = isConstructor ? S_ID_CONSTRUCTOR : S_ID_FUNCTION;
    functionHeader += indentifier;

    utils::replaceIdentifier(headerString, { indentifier, functionHeader});


    /* CPP */
    LOG_INFO << "building cpp function definition..." << ENDL;

    // remove default parameters in the cpp function declaration
    static const QRegularExpression defaultParamReg{
        "\\s?=\\s?(\\w|:|\\d|\\(\\)|\\{\\})+"
    };
    functionString.remove(defaultParamReg);

    // replace indent with identifier
    functionString.replace("\t" + QString{" "}.repeated(indent), S_ID_INDENT);

    // function base
    if (!isConstructor) {
        functionString.prepend(function.returnType + "\n" +
                               S_ID_CLASS_NAME + "::");
    }
    else {
        functionString.prepend(S_ID_CLASS_NAME + "::");
        functionString += S_ID_CONSTRUCTOR_INIT_LIST;
    }

    // implementation
    functionString += "\n{\n\t" + S_ID_IMPLEMENTATION + "\n}";
    functionString += indentifier;
    functionString.prepend("\n\n");

    utils::replaceIdentifier(sourceString, { indentifier, functionString});

    generateImplementation(headerString, sourceString, function, isConstructor);

    LOG_INFO << "done!";
}

void
ModuleGenerator::generateImplementation(QString& headerString,
                                        QString& sourceString,
                                        FunctionData const& function,
                                        bool isConstructor)
{
    LOG_INDENT("generating implementation...");

    auto& implementation = function.implementation;

    // function return value
    if (implementation.lines.isEmpty())
    {
        LOG_WARN << "skipping invalid implementation!";
        // clear identifier
        utils::replaceIdentifier(sourceString, { S_ID_IMPLEMENTATION, {} });
        return;
    }

//    LOG_INFO << "creating implementation string..." << ENDL;

    QString implementationString;

    QStringList lines = implementation.lines;

    for (auto& line : lines)
    {
        if (line.startsWith("#") || line.startsWith("//")) continue;
        line.append(";");
    }

    if (!isConstructor)
    {
        auto& last = lines.last();
        if (last.endsWith(";"))
        {
            last.prepend("return ");
        }
    }

    implementationString = lines.join("\n\t");
    // remove empty statements
    implementationString.remove("\t;");
    // dont indent preprocessor
    sourceString.replace("\t#", "#");

//    LOG_INFO << "setting implementation string..." << ENDL;

    utils::replaceIdentifier(sourceString, { S_ID_IMPLEMENTATION,
                                             implementationString });

    if (isConstructor) {
//        LOG_INFO << "done!";
        return;
    }

    generateIncludes(sourceString, implementation.includes);
    generateForwardDeclarations(headerString, implementation.forwardDeclarations);

    LOG_INFO << "generating derived classes..." << ENDL;
    generateImplementationHelper(sourceString, function.baseClass,
                                 implementation.derivedClasses);
    LOG_INFO << "generating linked classes..." << ENDL;
    generateImplementationHelper(sourceString, function.linkedClass,
                                 implementation.linkedClasses);

//    LOG_INFO << "done!";
}

void
ModuleGenerator::generateImplementationHelper(QString& sourceString,
                                              ClassData const& baseClass,
                                              QList<ClassData> const& classes)
{
    // linked classes to generate
    for (auto const& classStruct : classes)
    {
        if (!baseClass.isValid()) continue;

        if (!classStruct.isValid())
        {
            LOG_INDENT_WARN("skipping invalid class struct!");
            continue;
        }

        IdentifierPair pair{
            S_ID_INCLUDE_FILE,
            utils::makeInclude(classStruct.fileName, S_ID_INCLUDE_FILE)
        };

        utils::replaceIdentifier(sourceString, pair);

        generateBasicClass(baseClass, classStruct);
    }
}

void
ModuleGenerator::generateBasicClass(ClassData const& base,
                                    ClassData const& derived)
{
    LOG_INDENT("generating class '" + derived.className + "'...");

    LOG_INFO << "creating file strings..." << ENDL;

    auto headerString = utils::readFile(S_TEMPLATES_PATH +
                                        QStringLiteral("basic_derived_class.h"));
    auto sourceString = utils::readFile(S_TEMPLATES_PATH +
                                        QStringLiteral("basic_derived_class.cpp"));

    // set the filename for the include to the module header file
    utils::replaceIdentifier(sourceString, { S_ID_FILE_NAME, derived.fileName });

    generateConstructors(headerString, sourceString, base, derived.className);

    IdentifierPairs identifierPairs;

    // IMPLEMENT FUNCTIONS
    for (auto& function : derived.functions)
    {
        if (!function.isValid())
        {
            LOG_WARN << "skipping invalid function!" << ENDL; continue;
        }

        generateFunction(headerString, sourceString, function);

        // indent for parameters
        int indent = derived.className.length() + 3 + function.name.length();
        utils::replaceIdentifier(sourceString, {S_ID_INDENT,
                                                QString{" "}.repeated(indent)});
    }

    LOG_INFO << "setting identifiers..." << ENDL;

    // FILL IDENTIFIER
    identifierPairs.append({ S_ID_SIGNATURE, ModuleGeneratorSettings::S_SIGNATURE });
    identifierPairs.append({ S_ID_BASE_FILE_NAME, base.fileName});
    identifierPairs.append({ S_ID_BASE_CLASS,
                             QStringLiteral("public ") + base.className });
    identifierPairs.append({ S_ID_HEADER_NAME, derived.className.toUpper() });
    identifierPairs.append({ S_ID_CLASS_NAME, derived.className });
    identifierPairs.append({ S_ID_COMPAT_FILE, settings()->fileNamingScheme("compat") });

    identifierPairs.append({ S_ID_OBJECT_NAME, derived.objectName });
    identifierPairs.append({ S_ID_AUTHOR, settings()->authorDetails().name });
    identifierPairs.append({ S_ID_AUTHOR_EMAIL, settings()->authorDetails().email });
    identifierPairs.append({ S_ID_GENERATOR_VERSION, ModuleGeneratorSettings::S_VERSION });

    utils::replaceIdentifier(headerString, identifierPairs);
    utils::replaceIdentifier(sourceString, identifierPairs);

    LOG_INFO << "cleanup..." << ENDL;

    // CLEANUP
    clearFileString(headerString);
    clearFileString(sourceString);

    LOG_INFO << "writing files..." << ENDL;

    QDir targetDir = m_srcDir.cleanPath(m_srcDir.absolutePath() +
                                        QDir::separator() +
                                        base.outputPath);

    targetDir.mkpath(targetDir.absolutePath());

    utils::writeStringToFile(headerString, targetDir, derived.fileName + ".h");
    utils::writeStringToFile(sourceString, targetDir, derived.fileName + ".cpp");

    appendFileToProjectFile(derived.fileName, base.outputPath);

    LOG_INFO << "done!";
}

void
ModuleGenerator::generateIncludes(QString& sourceString,
                                  QStringList includes)
{
    if (includes.isEmpty()) return;

    LOG_INDENT("adding additional includes...");

    if (includes.contains(S_ID_2_0_INCLUDE_ICON))
    {
        includes.removeAll(S_ID_2_0_INCLUDE_ICON);
        includes.append(S_2_0_INCLUDE_ICON_LIST);
    }

    IdentifierPairs identifiers;

    for (auto& include : includes)
    {
        if (include.startsWith("#"))
        {
            LOG_INFO << "adding macro" << ENDL;

            IdentifierPair identifierPair{S_ID_ADD_INCLUDE_FILE,
                                          S_ID_ADD_INCLUDE_FILE + include};

            if (!sourceString.contains(include) &&
                !identifiers.contains(identifierPair))
            {
                identifiers.append(std::move(identifierPair));
            }
            else
            {
                LOG_WARN << "duplicate!" << ENDL;
            }
            continue;
        }

        LOG_INFO << include << ENDL;

        bool isQtInclude = include.startsWith("Q");

        QString includeString = utils::makeInclude(include, {}, isQtInclude);
        QString identifier    = isQtInclude ? S_ID_QT_INCLUDE_FILE:
                                              S_ID_ADD_INCLUDE_FILE;
        IdentifierPair identifierPair{identifier, includeString + identifier};

        if (!sourceString.contains(includeString) &&
            !identifiers.contains(identifierPair))
        {
            identifiers.append(std::move(identifierPair));
        }
        else
        {
            LOG_WARN << "duplicate!" << ENDL;
        }
    }

    for (auto const& ids : identifiers)
    {
        qDebug() << "IDS:" << ids.identifier << ids.value;
    }

    utils::replaceIdentifier(sourceString, identifiers);

    LOG_INFO << "done!";
}

void
ModuleGenerator::generateForwardDeclarations(QString& headerString,
                                             QStringList const& forwardDecls)
{
    if (forwardDecls.isEmpty()) return;

    LOG_INDENT("adding forward declarations...");

    IdentifierPairs identifiers;

    for (QString const& decl : forwardDecls)
    {
        LOG_INFO << decl << ENDL;

        IdentifierPair identifierPair;

        QString declString = "\nclass " + decl + ";";

        identifierPair.identifier = S_ID_CLASS_FORWARD_DECL;
        identifierPair.value = declString + S_ID_CLASS_FORWARD_DECL;

        if (!headerString.contains(declString) &&
            !identifiers.contains(identifierPair))
        {
            identifiers.append(identifierPair);
        }
        else
        {
            LOG_WARN << "duplicate!" << ENDL;
        }
    }

    utils::replaceIdentifier(headerString, identifiers);

    LOG_INFO << "done!";
}

void
ModuleGenerator::generateConstructors(QString& headerString,
                                      QString& sourceString,
                                      ClassData const& base,
                                      QString const& derivedName)
{
    LOG_INDENT("generating constructors...");

    auto constructors = base.constructors;

    if (constructors.isEmpty())
    {
        constructors.append(G_CONSTRUCTOR_DEFAULT);
    }

    for (auto& ctor : qAsConst(constructors))
    {
        FunctionData fCtor;
        fCtor.name = derivedName;
        fCtor.parameters = ctor.parameters;

        fCtor.implementation.lines = ctor.implementation;

        generateFunction(headerString, sourceString, fCtor, true);

        // indent for cpp parameters
        int indent = fCtor.name.length() * 2 + 3;
        utils::replaceIdentifier(sourceString, {
                S_ID_INDENT, QString{" "}.repeated(indent)});

        if (ctor.initilizerList.isEmpty())
        {
            continue;
        }

        // initializer list
        QString initList = " :\n\t" + ctor.initilizerList.join(",\n\t");
        utils::replaceIdentifier(sourceString, { S_ID_CONSTRUCTOR_INIT_LIST,
                                                 initList });
    }

    LOG_INFO << "done!";
}

void
ModuleGenerator::appendFileToProjectFile(QString const& fileName,
                                         QString const& path,
                                         bool headerOnly)
{
    LOG_INDENT("adding class to project file...");

    auto moduleClass = settings()->moduleClass();

    auto fileString = utils::readFile(m_srcDir.absoluteFilePath("src.pro"));

    IdentifierPairs identifierPairs;

    QString includePath = path;

    if (!m_proFileIncludePaths.contains(includePath))
    {
        m_proFileIncludePaths.append(includePath);

        identifierPairs.append({ S_ID_PRO_INCLUDE_PATH,
                                 S_PRO_ENDL + includePath + " " +
                                 S_ID_PRO_INCLUDE_PATH });
    }

    // no need to write "./my_file"
    if (includePath.isEmpty() || includePath == QStringLiteral("."))
    {
        includePath.clear();
    }
    else
    {
        includePath.append(QStringLiteral("/"));
    }

    // append identifieres
    identifierPairs.append({ S_ID_PRO_HEADER_PATH,
                             S_PRO_ENDL + includePath + fileName + ".h " +
                             S_ID_PRO_HEADER_PATH });

    if (!headerOnly)
    {
        identifierPairs.append({ S_ID_PRO_SOURCE_PATH,
                                 S_PRO_ENDL + includePath + fileName + ".cpp " +
                                 S_ID_PRO_SOURCE_PATH });
    }

    utils::replaceIdentifier(fileString, identifierPairs);

    utils::writeStringToFile(fileString, m_srcDir, "src.pro");

//    LOG_INFO << "done!";
}

void
ModuleGenerator::appendLibToProjectFile(const QString& name)
{
    LOG_INDENT("adding lib '" + name + "' to project file...");

    auto moduleClass(settings()->moduleClass());
    auto fileString = utils::readFile(m_srcDir.absoluteFilePath("src.pro"));

    IdentifierPairs identifierPairs;

    QDirIterator iterator(settings()->gtlabPath() + "/modules",
                          QDir::Files, QDirIterator::NoIteratorFlags);

    QStringList nameParts(name.split(QChar{' '}, QString::SkipEmptyParts));

    bool success(false);

    while (iterator.hasNext() && !success)
    {
        QFileInfo fileInfo(iterator.next());
        QString lib(fileInfo.baseName());

        // strip -d in case of debug build
        if (lib.endsWith("-d"))
        {
            lib = lib.mid(0, lib.size() - 2);
        }

        // e.g. "Post Processing": libname is GTlabPost
        // 1. check if postprocessing is in libname
        // 2. check if post is in libname
        for (int i = nameParts.length(); i > 0; --i)
        {
            QString part = nameParts.mid(0, i).join("");

            if (!lib.contains(part, Qt::CaseInsensitive)) continue;

            if (!ModuleGeneratorSettings::isOsWindows() &&
                lib.startsWith("lib"))
            {
                lib = lib.remove(0, 3); // remove lib prefix on unix
            }

            identifierPairs.append({ S_ID_PRO_LIBS, "\n\tLIBS += -l" + lib +
                                     S_ID_PRO_LIBS});
            identifierPairs.append({ S_ID_PRO_LIBS_D, "\n\tLIBS += -l" + lib +
                                     "-d" + S_ID_PRO_LIBS_D});

            success = true;
            break;
        }
    }

    if (!success)
    {
        LOG_WARN << "could not find library file!";
        return;
    }

    utils::replaceIdentifier(fileString, identifierPairs);

    utils::writeStringToFile(fileString, m_srcDir, "src.pro");
}

void
ModuleGenerator::clearFileString(QString& fileString)
{
    static const QRegularExpression idsRegExp{"\\$\\$.+\\$\\$"};
    static const QRegularExpression nlRegExp{"[\\n\\r]{3,}"};

    clearCompatibilityMacros(fileString);

    // remove identifiers
    fileString.remove(idsRegExp);

    // preprocessor should start at line beginning
    fileString.replace("\t#", "#");

    // convert tab to blanks
    fileString.replace("\t", "    ");

    // condense new lines
    fileString.replace(nlRegExp, "\n\n");
}

void
clearCompatibilityMacroHelper(QRegularExpression const& regExp,
                              QString& fileString,
                              int group2keep)
{
    auto matchIter = regExp.globalMatch(fileString);
    while (matchIter.hasNext())
    {
        auto match = matchIter.next();
        // use if block
        int start = match.capturedStart(0);
        int end = match.capturedEnd(0);

        // find text to keep
        QString text = match.captured(group2keep);
        text.replace(QString{" "}.repeated(8), // fix indentation
                     QString{" "}.repeated(4));

        fileString.replace(start, end - start, text);

        // update iter
        matchIter = regExp.globalMatch(fileString);
    }
}

void
ModuleGenerator::clearCompatibilityMacros(QString& fileString)
{
    if (settings()->useCompatibilityMacros())
    {
        return;
    }

    // gt_lobals.h for version dependent code
    fileString.remove("#include \"gt_globals.h\"\n");

    // first block  = group 1
    // second block = group 4
    static const QRegularExpression regExp_1_7{QStringLiteral(
            R"(#ifdef\sCOMPAT_VERSION_1_X\n((.*\n)+?)(#else\n((.*\n)+?))?#endif\n)"
    )};
    static const QRegularExpression regExp_2_0{QStringLiteral(
            R"(#ifdef\sCOMPAT_VERSION_2_0\n((.*\n)+?)(#else\n((.*\n)+?))?#endif\n)"
    )};

    bool is_1_7 = settings()->gtlabMajorVersion() < 2;

    clearCompatibilityMacroHelper(regExp_1_7, fileString,  is_1_7 ? 1 : 4);
    clearCompatibilityMacroHelper(regExp_2_0, fileString, !is_1_7 ? 1 : 4);

    if (!is_1_7)
    {
        static const QRegularExpression compatRegExp{QStringLiteral(
                R"(#include "\w*?compat\.h"\n)"
        )};
        fileString.remove(compatRegExp);

//        // const ref macros
//        fileString.replace("#GT_CONST_REF", "const&");
    }
}

void
ModuleGenerator::clearProjectFileIdentifiers()
{
    // src.pro
    auto fileString = utils::readFile(m_srcDir.absoluteFilePath("src.pro"));

    bool is_1_7 = settings()->gtlabMajorVersion() < 2;

    // bit hacky and currently only works for major version 1 and 2
    // remove version dependent code blocks
    if (!settings()->useCompatibilityMacros())
    {
        static const QRegularExpression regExp{
            "greaterThan\\(MAJOR_VERSION, 1\\)\\s{\\s+" // version check
            "((\\s|.)+?)" // if block (group 1)
            "\\s+}\\selse\\s{\\s+"
            "((\\s|.)+?)" // else block (group 3)
            "\\s+}"
        };

        clearCompatibilityMacroHelper(regExp, fileString, is_1_7 ? 3 : 1);
    }

    clearFileString(fileString);

    utils::writeStringToFile(fileString, m_srcDir, "src.pro");

    // module.pro
    auto moduleClass = settings()->moduleClass();

    fileString = utils::readFile(m_moduleDir.absoluteFilePath(
                                     moduleClass.fileName + ".pro"));

    clearFileString(fileString);

    utils::writeStringToFile(fileString, m_moduleDir,
                             moduleClass.fileName + ".pro");
}
