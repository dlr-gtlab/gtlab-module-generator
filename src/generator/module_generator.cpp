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
ModuleGenerator::S_ID_2_0_INCLUDE_ICON = QStringLiteral("$$2_0_INCLUDE_ICON$$");

const QString
ModuleGenerator::S_ID_CMAKE_SOURCE_FILES = QStringLiteral("$$CMAKE_SOURCE_FILES$$");
const QString
ModuleGenerator::S_ID_CMAKE_ADDITIONAL_FILES = QStringLiteral("$$CMAKE_ADDITIONAL_FILES$$");
const QString
ModuleGenerator::S_ID_CMAKE_MODULE_ALIAS = QStringLiteral("$$CMAKE_MODULE_ALIAS$$");
const QString
ModuleGenerator::S_ID_CMAKE_INCLUDE_DIRS = QStringLiteral("$$CMAKE_INCLUDE_DIRS$$");
const QString
ModuleGenerator::S_ID_CMAKE_FIND_PACKAGE_DEPS = QStringLiteral("$$CMAKE_FIND_PACKAGE_DEPS$$");
const QString
ModuleGenerator::S_ID_CMAKE_TARGET_LINK_LIBRARIES = QStringLiteral("$$CMAKE_TARGET_LINK_LIBRARIES$$");

const QString
ModuleGenerator::S_1_7_VERSION_CHECK =
        QStringLiteral("#if GT_VERSION < GT_VERSION_CHECK(2, 0, 0)");

const QString
ModuleGenerator::S_2_0_VERSION_CHECK =
        QStringLiteral("#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)");

const QStringList
ModuleGenerator::S_2_0_INCLUDE_ICON_LIST{
    "gt_compat",
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
S_CMAKE_PATH = QStringLiteral("/lib/cmake/");
const QString
S_SRC_DIR      = QStringLiteral("src");
const QString
S_FEATURES_DIR = QStringLiteral("features");

// for protected methods
const QString
S_PROTECTED_TAG = QStringLiteral("protected:");

// qt makros
const QString
S_INTERFACE_MACRO(QStringLiteral("Q_INTERFACES(") +
                  ModuleGenerator::S_ID_BASE_CLASS +
                  QStringLiteral(")\n\t"));
const QString
S_OVERRIDE(QStringLiteral(" override;"));
const QString
S_Q_INVOKABLE(QStringLiteral("Q_INVOKABLE "));

// helper strings
const QString
S_DERIVE_BASE_CLASS(QStringLiteral(",\n\t\tpublic ") +
                    ModuleGenerator::S_ID_BASE_CLASS);
const QString
S_QMAKE_ENDL(QStringLiteral("\\\n\t"));
const QString
S_CMAKE_ENDL(QStringLiteral("\n\t"));

// cmake specifics
const QString
S_CMAKE_GIT_SOURCE_FILES(QStringLiteral(
R"(
    README_FILE "${PROJECT_SOURCE_DIR}/README.md"
    CHANGELOG_FILE "${PROJECT_SOURCE_DIR}/CHANGELOG.md")"));

struct ModuleGenerator::CMake
{

static QString
getCMakePackageName(const ModuleGeneratorSettings& settings,
                    const QString& moduleName)
{
    QDirIterator iterator(settings.devToolsPath() + S_CMAKE_PATH,
                          QDir::Dirs, QDirIterator::NoIteratorFlags);

    QString normalizedName = moduleName;
    normalizedName.replace(" ", "");

    // some well-known exceptions
    if (normalizedName.startsWith(QStringLiteral("PythonModule")) ||
        normalizedName.startsWith(QStringLiteral("PythonSetup")))
    {
        normalizedName = QStringLiteral("GTlabPython");
    }

    LOG_INDENT(QObject::tr("Searching cmake package for module '%1'").arg(moduleName));

    bool success = false;
    while (iterator.hasNext() && !success)
    {
        QString const& dirname =  iterator.fileName();
        if (dirname.contains(normalizedName))
        {
            return dirname;
        }

        iterator.next();
    }

    LOG_WARN << QObject::tr("CMake package for module %1 not found, assuming package name '%2'")
                    .arg(moduleName, normalizedName);

    return normalizedName;
}

/**
 * @brief Reads in the *-targets.cmake file to find, which library targets exist
 * @param packageName Name of the cmake package
 * @return
 */
static QStringList
getCMakePackageTargets(const ModuleGeneratorSettings& settings,
                       const QString& packageName)
{
    LOG_INDENT(QObject::tr("Searching cmake targets for package '%1'").arg(packageName));

    QDir pkgDir(settings.devToolsPath() + S_CMAKE_PATH + packageName);

    if (!pkgDir.exists())
    {
        LOG_WARN << QObject::tr("CMake package not found. Assuming target '%1'").arg(packageName);
        return {packageName};
    }

    auto targetsFileStr = utils::readFile(pkgDir.absolutePath() + QStringLiteral("/%1-targets.cmake").arg(packageName));

    static const QRegularExpression targetRegex{R"(add_library\((\S+) SHARED IMPORTED\))"};

    QStringList targets;

    auto iter = targetRegex.globalMatch(targetsFileStr);
    while(iter.hasNext())
    {
        auto match = iter.next();
        targets.push_back(match.captured(1));
    }

    if (targets.empty())
    {
        LOG_WARN << QObject::tr("No cmake targets found for package '%1'. Assuming target '%1'").arg(packageName);
        return {packageName};
    }

    return targets;
}

static QString
buildFindPackageStr(QString const& targetName, QString const& packageName)
{
    return QStringLiteral(
R"(
if(NOT TARGET %1)
   find_package(%2 REQUIRED)
endif()
)").arg(targetName, packageName);
}

static void
registerCMakeSourceFiles(ModuleGeneratorSettings const& settings,
                         QStringList sourceFiles,
                         IdentifierPairs& identifierPairs)
{
    identifierPairs.append({ S_ID_CMAKE_SOURCE_FILES, sourceFiles.join(S_CMAKE_ENDL) });

    if (settings.createGitFiles())
    {
        // add git source files
        identifierPairs.append({ S_ID_CMAKE_ADDITIONAL_FILES, S_CMAKE_GIT_SOURCE_FILES});
    }
}

static void
registerCMakeModuleAlias(ModuleGeneratorSettings const& settings,
                         IdentifierPairs& identifierPairs)
{
    auto const& moduleClass = settings.moduleClass();

    if (!settings.modulePrefix().isEmpty())
    {
        QString classWithoutPrefix = moduleClass.className;
        classWithoutPrefix = classWithoutPrefix.remove(settings.modulePrefix());

        QString alias = QStringLiteral("\nadd_library(%1::%2 ALIAS %3)\n")
                            .arg(settings.modulePrefix(),
                                 classWithoutPrefix,
                                 moduleClass.className);

        identifierPairs.append({ S_ID_CMAKE_MODULE_ALIAS, alias});
    }
}

static void
registerCMakePackageDependencies(ModuleGeneratorSettings const& settings,
                                 IdentifierPairs& identifierPairs)
{
    QString targetsToLink;
    QString findPackageStr;

    for (auto const& dep : settings.selectedDependencies())
    {
        auto const& pkgName = getCMakePackageName(settings, dep.name);
        auto const& targets = getCMakePackageTargets(settings, pkgName);

        for (auto const& target : targets)
        {
            targetsToLink += QChar{' '} + target;
        }

        assert(!targets.empty());

        findPackageStr += CMake::buildFindPackageStr(targets[0], pkgName);
    }

    if (settings.useCompatibilityMacros())
    {
        findPackageStr += CMake::buildFindPackageStr(QStringLiteral("GTlab::Compat"),
                                                     QStringLiteral("GTlabCompat"));
        targetsToLink += QChar{' '} + QStringLiteral("GTlab::Compat");
    }

    identifierPairs.append({ S_ID_CMAKE_TARGET_LINK_LIBRARIES, targetsToLink});
    identifierPairs.append({ S_ID_CMAKE_FIND_PACKAGE_DEPS, findPackageStr});
}

static void
registerCMakeIncludes(QStringList const& includePaths,
                      IdentifierPairs& identifierPairs)
{
    QStringList paths;

    std::transform(includePaths.begin(), includePaths.end(),
                   std::back_inserter(paths),
                   [](QString const& path) {
        return QStringLiteral(R"($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/%1>)")
            .arg(path);
    });

    identifierPairs.append({ S_ID_CMAKE_INCLUDE_DIRS, paths.join(S_CMAKE_ENDL) });
}

}; // struct cmake

struct ModuleGenerator::QMake
{

static QString
getDependencyLibName(ModuleGeneratorSettings& settings,
                     QString const& name)
{
    LOG_INDENT("adding dependency '" + name + "' to project file...");

    QDirIterator iterator(settings.gtlabPath() + "/modules",
                          QDir::Files, QDirIterator::NoIteratorFlags);

    QStringList nameParts(name.split(QChar{' '}, QString::SkipEmptyParts));

    while (iterator.hasNext())
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

            return lib;
        }
    }

    LOG_WARN << "could not find library file!";
    return {};
}

static void
registerDependencies(ModuleGeneratorSettings& settings,
                     IdentifierPairs& identifierPairs)
{
    QString releaseLibs, debugLibs;

    for (auto const& dependency : settings.selectedDependencies())
    {
        QString lib = getDependencyLibName(settings, dependency.name);
        if (!lib.isEmpty())
        {
            static QString pattern = QStringLiteral("\n\tLIBS += -l%1");
            releaseLibs += pattern.arg(lib);
            debugLibs   += (pattern + QStringLiteral("-d")).arg(lib);
        }
    }

    identifierPairs.append({ S_ID_PRO_LIBS, releaseLibs });
    identifierPairs.append({ S_ID_PRO_LIBS_D, debugLibs });
}

};

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
    bool result(generateModuleDirs());
    if (!result) return false;

    result = generateModuleMetaFile();
    if (!result) return false;

    result = generateModuleFiles();
    if (!result) return false;

    result = generateModuleProjectFiles();
    if (!result) return false;

    // ok to continue
    generateGitFiles();

    return result;
}

bool
ModuleGenerator::generateModuleDirs()
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
//    m_featuresDir = QDir::cleanPath(path + QDir::separator() + S_FEATURES_DIR);

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

    LOG_INFO << "done!";

    return m_srcDir.exists();
}

bool
ModuleGenerator::generateModuleProjectFiles()
{
    bool success = false;
    // either one should succeed
    success |= generateQMakeFiles();

    success |= generateCMakeFiles();

    return success;
}

bool
ModuleGenerator::generateQMakeFiles()
{
    LOG_INDENT("generating qmake project files...");

    auto const& moduleClass = settings()->moduleClass();
    auto const& prefix = settings()->modulePrefix();

    IdentifierPairs identifierPairs;

    identifierPairs.append({ S_ID_MODULE_NAME, moduleClass.ident });
    identifierPairs.append({ S_ID_CLASS_NAME, moduleClass.className });

    auto targetDirName = prefix.isEmpty() ? settings()->moduleClass().ident : prefix;
    identifierPairs.append({ S_ID_TARGET_DIR_NAME, targetDirName.remove(' ') });

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
        // not aborting here as issue is non critical
    }

    identifierPairs.append({ S_ID_GTLAB_INSTALL_DIR, gtlabDir.absolutePath() });

    // append generated files
    QStringList sourceFiles, headerFiles;
    auto const& generatedSourceFiles = this->generatedSourceFiles();

    std::copy_if(generatedSourceFiles.begin(), generatedSourceFiles.end(),
                 std::back_inserter(sourceFiles), [](QString const& fileName){
        return fileName.endsWith(".cpp");
    });
    std::copy_if(generatedSourceFiles.begin(), generatedSourceFiles.end(),
                 std::back_inserter(headerFiles), [](QString const& fileName){
        return fileName.endsWith(".h");
    });

    identifierPairs.append({ S_ID_PRO_HEADER_PATH, headerFiles.join(S_QMAKE_ENDL) });
    identifierPairs.append({ S_ID_PRO_SOURCE_PATH, sourceFiles.join(S_QMAKE_ENDL) });

    // append include paths
    auto const& includePaths = this->generatedIncludePaths();
    identifierPairs.append({ S_ID_PRO_INCLUDE_PATH, includePaths.join(S_QMAKE_ENDL) });

    QMake::registerDependencies(*settings(), identifierPairs);
    
    applyTemplate(S_TEMPLATES_PATH + QStringLiteral("qmake"),
                   m_moduleDir,
                   identifierPairs);

    // rename main .pro file
    QFile file(m_moduleDir.absoluteFilePath("module.pro"));
    assert(file.exists());
    file.rename(m_moduleDir.absoluteFilePath(moduleClass.fileName + ".pro"));

    return true;
}

bool
ModuleGenerator::generateCMakeFiles()
{
    LOG_INDENT("generating cmake files...");

    IdentifierPairs identifierPairs;

    auto const& moduleClass = settings()->moduleClass();

    identifierPairs.append({ S_ID_MODULE_NAME, moduleClass.ident });
    identifierPairs.append({ S_ID_CLASS_NAME, moduleClass.className });

    CMake::registerCMakeModuleAlias(*settings(), identifierPairs);
    CMake::registerCMakeSourceFiles(*settings(), generatedSourceFiles(), identifierPairs);
    CMake::registerCMakePackageDependencies(*settings(), identifierPairs);
    CMake::registerCMakeIncludes(generatedIncludePaths(), identifierPairs);
    
    applyTemplate(S_TEMPLATES_PATH + QStringLiteral("cmake"),
                   m_moduleDir,
                   identifierPairs);

    LOG_INFO << "done!";

    return true;
}

bool
ModuleGenerator::generateGitFiles()
{
    if (!settings()->createGitFiles()) return true;

    LOG_INDENT("generating git files...");

    IdentifierPairs identifierPairs;

    auto moduleClass = settings()->moduleClass();

    identifierPairs.append({ S_ID_PREFIX, settings()->modulePrefix() });
    identifierPairs.append({ S_ID_MODULE_NAME, moduleClass.ident });
    identifierPairs.append({ S_ID_MODULE_DESCRIPTION, moduleClass.description });
    identifierPairs.append({ S_ID_MODULE_VERSION, moduleClass.version });
    identifierPairs.append({ S_ID_GENERATOR_VERSION, ModuleGeneratorSettings::S_VERSION });
    
    applyTemplate(S_TEMPLATES_PATH + QStringLiteral("git"),
                   m_moduleDir,
                   identifierPairs);

    LOG_INFO << "done!";

    return true;
}

bool
ModuleGenerator::generateModuleMetaFile()
{
    LOG_INDENT("generating module json file...");

    // for logging purpose
    {
        LOG_INDENT("setting dependencies...");

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

    LOG_INFO << "done!";

    return true;
}

bool
ModuleGenerator::generateModuleFiles()
{
    LOG_INDENT("generating module files...");

    auto headerString = utils::readFile(S_TEMPLATES_PATH +
                                        "basic_module_class.h");
    auto sourceString = utils::readFile(S_TEMPLATES_PATH +
                                        "basic_module_class.cpp");

    auto moduleClass = settings()->moduleClass();

    // append module files
    appendGeneratedSourceFile(moduleClass.fileName);

    // set the filename for the include to the module header file
    utils::replaceIdentifier(sourceString, { S_ID_FILE_NAME,
                                             moduleClass.fileName });

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

QString
beginCompatibilityMacro(VersionData const& version)
{
    if (!version.min.isEmpty())
    {
        auto vers = QVersionNumber::fromString(version.min);
        return QStringLiteral(
            "#if GT_VERSION >= GT_VERSION_CHECK(%1, %2, %3)\n")
            .arg(vers.majorVersion())
            .arg(vers.minorVersion())
            .arg(vers.microVersion());
    }
    else if (!version.max.isEmpty())
    {
        auto vers = QVersionNumber::fromString(version.max);
        return QStringLiteral(
            "#if GT_VERSION < GT_VERSION_CHECK(%1, %2, %3)\n")
            .arg(vers.majorVersion())
            .arg(vers.minorVersion())
            .arg(vers.microVersion());
    }
    return {};
}

QString
endCompatibilityMacro(VersionData const& version)
{
    if (!version.min.isEmpty() || !version.max.isEmpty())
    {
        return QStringLiteral("\n#endif");
    }
    return {};
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
    QString functionString;
    functionString += function.name;

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
        functionHeader += '\n';
        functionHeader += beginCompatibilityMacro(function.version);
        functionHeader += function.description;
    }
    else
    {
        functionHeader += beginCompatibilityMacro(function.version);
    }

    functionHeader += "\n\t";

    // function declaration
    if (!isConstructor)
    {
        functionHeader += function.returnType + " ";
        functionHeader += functionString;
        functionHeader += S_OVERRIDE;
    }
    else
    {
        functionHeader += S_Q_INVOKABLE;
        functionHeader += functionString +";";
    }

    functionHeader += endCompatibilityMacro(function.version);

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
    if (isConstructor)
    {
        functionString = beginCompatibilityMacro(function.version) +
                         S_ID_CLASS_NAME + "::" +
                         functionString +
                         S_ID_CONSTRUCTOR_INIT_LIST;
    }
    else
    {
        functionString = beginCompatibilityMacro(function.version) +
                         function.returnType + "\n" + S_ID_CLASS_NAME + "::" +
                         functionString;
    }

    // implementation
    functionString += "\n{\n\t" + S_ID_IMPLEMENTATION + "\n}";

    functionString += endCompatibilityMacro(function.version);

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

    utils::replaceIdentifier(sourceString, { S_ID_IMPLEMENTATION,
                                             implementationString });

    if (isConstructor) return;

    // hacky way to insert compat file in header and source string
    // if its missing
    if (headerString.contains("GT_VERSION"))
    {
        generateIncludes(headerString, QStringList{"gt_compat"});
    }
    else if (sourceString.contains("GT_VERSION"))
    {
        generateIncludes(sourceString, QStringList{"gt_compat"});
    }

    generateIncludes(sourceString, implementation.includes);
    generateForwardDeclarations(headerString, implementation.forwardDeclarations);

    LOG_INFO << "generating derived classes..." << ENDL;
    generateBasicClassHelper(sourceString, function.baseClass,
                             implementation.derivedClasses);
    LOG_INFO << "generating linked classes..." << ENDL;
    generateBasicClassHelper(sourceString, function.linkedClass,
                             implementation.linkedClasses);

//    LOG_INFO << "done!";
}

void
ModuleGenerator::generateBasicClassHelper(QString& sourceString,
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

    // append module files
    appendGeneratedSourceFile(derived.fileName, base.outputPath);

    utils::writeStringToFile(headerString, targetDir, derived.fileName + ".h");
    utils::writeStringToFile(sourceString, targetDir, derived.fileName + ".cpp");

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
ModuleGenerator::appendGeneratedSourceFile(QString const& fileName,
                                           QString const& dir,
                                           bool headerOnly)
{
    GeneratedSourceFile generatedFile;
    generatedFile.fileName = fileName;
    generatedFile.dir = dir;
    generatedFile.headerOnly = headerOnly;
    m_generatedSourceFiles.push_back(generatedFile);
}

QStringList
ModuleGenerator::generatedSourceFiles() const
{
    QStringList generatedFiles;
    for (auto& data : m_generatedSourceFiles)
    {
        QString filePath;

        // no need to write "./my_file"
        if (!data.dir.isEmpty() && data.dir != QStringLiteral("."))
        {
            filePath += data.dir + QChar{'/'};
        }

        filePath += data.fileName;

        generatedFiles.push_back(filePath + QStringLiteral(".h"));
        if (!data.headerOnly)
        {
            generatedFiles.push_back(filePath + QStringLiteral(".cpp"));
        }
    }
    return utils::unique(generatedFiles);
}

QStringList
ModuleGenerator::generatedIncludePaths() const
{
    QStringList includePaths;
    for (auto& data : m_generatedSourceFiles)
    {
        // no need to write "./my_file"
        if (!data.dir.isEmpty() && data.dir != QStringLiteral("."))
        {
            includePaths.push_back(data.dir);
        }
    }
    qDebug() << __FUNCTION__ << utils::unique(includePaths);
    return utils::unique(includePaths);
}

namespace
{

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
clearCompatibilityMacros(QString& fileString, bool is_1_7)
{
    // gt_lobals.h for version dependent code
    fileString.remove("#include \"gt_globals.h\"\n");

    // first block  = group 1
    // second block = group 4
    static const QRegularExpression regExp_1_7{QStringLiteral(
        R"(#if GT_VERSION < GT_VERSION_CHECK\(2, 0, 0\)\n((.*\n)+?)(#else\n((.*\n)+?))?#endif\n)"
        )};
    static const QRegularExpression regExp_2_0{QStringLiteral(
        R"(#if GT_VERSION >= GT_VERSION_CHECK\(2, 0, 0\)\n((.*\n)+?)(#else\n((.*\n)+?))?#endif\n)"
        )};

    clearCompatibilityMacroHelper(regExp_1_7, fileString,  is_1_7 ? 1 : 4);
    clearCompatibilityMacroHelper(regExp_2_0, fileString, !is_1_7 ? 1 : 4);

    if (!is_1_7)
    {
        static const QRegularExpression compatRegExp{QStringLiteral(
            R"(#include "gt_compat\.h"\n)"
            )};
        fileString.remove(compatRegExp);
    }

    // remove qmake specific compatibility macros
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
}

} // namespace

void
ModuleGenerator::clearFileString(QString& fileString)
{
    static const QRegularExpression idsRegExp{"\\$\\$[\\w\\d_]+\\$\\$"};
    static const QRegularExpression nlRegExp{"[\\n\\r]{3,}"};

    if (!settings()->useCompatibilityMacros())
    {
        bool is_1_7 = settings()->gtlabMajorVersion() < 2;
        clearCompatibilityMacros(fileString, is_1_7);
    }

    // remove identifiers
    fileString.remove(idsRegExp);

    // preprocessor should start at line beginning
    fileString.replace("\t#", "#");

    // convert tab to blanks
    fileString.replace("\t", "    ");

    // condense new lines
    fileString.replace(nlRegExp, "\n\n");
}

bool
ModuleGenerator::applyTemplate(QDir const& originDirectory,
                               QDir const& destinationDirectory,
                               IdentifierPairs const& identifierPairs,
                               int flags,
                               QRegularExpression const& filter)
{
    LOG_INDENT(QStringLiteral("applying template '%1'...").arg(originDirectory.dirName()));

    if (!originDirectory.exists())
    {
        LOG_ERR << "applying template failed, source directory not found!";
        return false;
    }

    if (!destinationDirectory.exists())
    {
        destinationDirectory.mkpath(".");
    }

    auto const& files = utils::listFiles(originDirectory, (flags & Recursive), filter);

    for (QString const& fileName : files)
    {
        QString targetFileName = destinationDirectory.path() + "/" + fileName;

        LOG_INFO << "applying " << fileName << "..." << ENDL;

        QFileInfo targetFileInfo(targetFileName);

        // qfile does not overwrite files
        if (targetFileInfo.exists() && (flags & OverwriteFiles))
        {
            QFile(targetFileName).remove();
        }

        if (!targetFileInfo.exists() || (flags & OverwriteFiles))
        {
            auto subDir = targetFileInfo.dir();
            if (!subDir.exists()) QDir{}.mkpath(subDir.absolutePath());

            auto fileString = utils::readFile(originDirectory.path() + "/" + fileName);

            utils::replaceIdentifier(fileString, identifierPairs);

            clearFileString(fileString);

            utils::writeStringToFile(fileString,
                                     targetFileInfo.absoluteDir(),
                                     targetFileInfo.fileName());
        }
    }

    /*! Possible race-condition mitigation? */
    QDir finalDestination(destinationDirectory);
    finalDestination.refresh();

    if (finalDestination.exists())
    {
        LOG_INFO << "done!";
        return true;
    }

    LOG_WARN << "failed!";
    return false;
}
