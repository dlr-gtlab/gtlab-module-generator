#ifndef MODULEGENERATORSETTINGS_H
#define MODULEGENERATORSETTINGS_H


#include <QString>
#include <QRegularExpression>
#include <QList>

#include "preloader.h"


class ModuleGeneratorSettings
{

public:

    const static QString S_VERSION;
    const static QRegularExpression REG_PREFIX;
    const static QRegularExpression REG_OBJECT_NAME;
    const static QRegularExpression REG_VERSION;
    const static QRegularExpression REG_AUTHOR;
    const static QRegularExpression REG_AUTHOR_EMAIL;

    const static QFont F_MONO_FONT;
    const static QString S_SIGNATURE;

    const static QString S_EXEC_SUFFIX;
    const static QString S_GTLAB_APP;
    const static QString S_GTLAB_CONSOLE_APP;

    /**
     * @brief mode enum - enumerations for the two modes of the wizard
     */
    enum WizardMode { Create = 0, Edit = 1 };

    ModuleGeneratorSettings();

    static bool isOsWindows() { return QSysInfo::productType() == "windows"; }

    QString classNamingScheme(const QString& name, const QString& prefix) const;
    QString classNamingScheme(const QString& name) const;

    QString fileNamingScheme(const QString& name, const QString& prefix) const;
    QString fileNamingScheme(const QString& name) const;

    // wizard mode
    inline WizardMode wizardMode() const { return m_wizardMode; }
    inline void setWizardMode(const WizardMode mode) { m_wizardMode = mode; }

    // settings
    inline QString outputPath() const { return m_outputPath; }
    inline QString gtlabPath() const { return m_gtlabPath; }
    inline QString devToolsPath() const { return m_devToolsPath; }

    inline void setOutputPath(const QString& path) { m_outputPath = path; }
    inline void setGTlabPath(const QString& path) { m_gtlabPath = path; }
    inline void setDevToolsPath(const QString& path) { m_devToolsPath = path; }

    // specifications
    inline QString modulePrefix() const { return m_modulePrefix; }
    inline void setModulePrefix(const QString& prefix) { m_modulePrefix = prefix; }

    inline ModuleClass moduleClass() const { return m_moduleClass; }
    inline void setModuleClass(const ModuleClass& module)  { m_moduleClass = module; }

    // interfaces
    inline ClassStructsPtr selectedInterfaces() const { return m_selectedInterfaces; }
    inline void setSelectedInterfaces(const ClassStructsPtr& interfaces) { m_selectedInterfaces = interfaces; }

    // dependencies
    inline DependencieStructs selectedDependencies() const { return m_selectedDependencies; }
    inline void setSelectedDependencies(const DependencieStructs& dependencies) { m_selectedDependencies = dependencies; }

    // signature
    AuthorDetails authorDetails() const { return m_authorDetails; };
    inline void setAuthorDetails(const AuthorDetails& details) { m_authorDetails = details; };

    // PreLoader
    void preLoad();

    inline QStringList reservedPrefixes() const { return m_preLoader.prefixes(); }
    inline ClassStructsPtr availableInterfaces() const { return m_preLoader.interfaces(); }
    inline DependencieStructs availableDependencies() const { return m_preLoader.dependencies(); }

    // serialize user data (name, email, paths etc.)
    void serializeUserData() const;
    void deserializeUserData();

private:

    WizardMode m_wizardMode;

    QString m_outputPath;
    QString m_gtlabPath;
    QString m_devToolsPath;    

    QString m_modulePrefix;
    ModuleClass m_moduleClass;

    ClassStructsPtr m_selectedInterfaces;
    DependencieStructs m_selectedDependencies;

    QString m_signature;
    AuthorDetails m_authorDetails;

    PreLoader m_preLoader;
};

#endif // MODULEGENERATORSETTINGS_H
