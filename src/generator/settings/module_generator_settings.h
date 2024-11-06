#ifndef MODULEGENERATORSETTINGS_H
#define MODULEGENERATORSETTINGS_H


#include <QString>
#include <QRegularExpression>
#include <QList>
#include <QFuture>

#include "module_generator_preloader.h"


class ModuleGeneratorSettings
{

public:

    /// module generator version
    const static QString S_VERSION;
    /// regular expression for common tasks
    const static QRegularExpression REG_PREFIX;
    const static QRegularExpression REG_OBJECT_NAME;
    const static QRegularExpression REG_CLASS_NAME;
    const static QRegularExpression REG_FILE_NAME;
    const static QRegularExpression REG_VERSION;
    const static QRegularExpression REG_AUTHOR;
    const static QRegularExpression REG_AUTHOR_EMAIL;

    /// mono font
    const static QFont F_MONO_FONT;
    /// signature base string
    const static QString S_DEFAULT_SIGNATURE;

    /// exec
    const static QString S_EXEC_SUFFIX;
    const static QString S_GTLAB_APP;
    const static QString S_GTLAB_CONSOLE_APP;

    /**
     * @brief mode enum - enumerations for the two modes of the wizard
     */
    enum WizardMode { Create = 0, Edit = 1 };

    ModuleGeneratorSettings();

    static bool isOsWindows();

    static QString currentYear();
    static QString currentDate();

    QString classNamingScheme(QString const& name, const QString& prefix) const;
    QString classNamingScheme(QString const& name) const;

    QString fileNamingScheme(QString const& name, const QString& prefix) const;
    QString fileNamingScheme(QString const& name) const;

    // wizard mode
    WizardMode wizardMode() const { return m_wizardMode; }
    void setWizardMode(WizardMode mode) { m_wizardMode = mode; }

    int gtlabMajorVersion() const;
    QString const& gtlabVersion() const { return m_version; }
    void setGTlabVersion(QString const& version) { m_version = version; }

    bool useCompatibilityMacros() const  { return m_useCompatibilityMacros; }
    void setUseCompatibilityMacros(bool yes) { m_useCompatibilityMacros = yes; }

    QString const& plainSignature() const { return m_signature; }
    void setPlainSignature(QString const& sig) { m_signature = sig; }

    /// actual signature to use
    QString signature() const;

    bool useSignature() const  { return m_useSignature; }
    void setUseSignature(bool yes) { m_useSignature = yes; }

    static QStringList supportedVersions();

    // settings
    QString const& outputPath() const { return m_outputPath; }
    QString const& gtlabPath() const { return m_gtlabPath; }
    QString gtlabInstallDir() const;

    void setOutputPath(QString const& path) { m_outputPath = path; }
    void setGTlabPath(QString const& path) { m_gtlabPath = path; }

    bool useCMakeGenerator() const { return m_useCMakeGenerator; }
    bool useQMakeGenerator() const { return m_useQMakeGenerator; }

    void setUseCMakeGenerator(bool value) { m_useCMakeGenerator = value; }
    void setUseQMakeGenerator(bool value) { m_useQMakeGenerator = value; }

    // specifications
    QString const& modulePrefix() const;
    void setModulePrefix(QString const& prefix);

    ModuleData const& moduleClass() const;
    void setModuleClass(ModuleData const& module);

    // interfaces
    ClassDataList const& selectedInterfaces() const;
    void setSelectedInterfaces(ClassDataList const& interfaces);

    // dependencies
    DependencyDataList const& selectedDependencies() const;
    void setSelectedDependencies(DependencyDataList const& dependencies);

    // signature
    AuthorDetails const& authorDetails() const;
    void setAuthorDetails(AuthorDetails const& details);

    // summary
    void setCreateGitFiles(bool value);
    bool createGitFiles() const;

    // PreLoader
    void preLoad();

    QStringList const& reservedPrefixes() const;
    ClassDataList const& availableInterfaces() const;
    TypeInfo typeInfo(QString const& returnType) const;
    DependencyDataList const& availableDependencies() const;

    int dependencyResolveStatus() const;

    // serialize user data (name, email, paths etc.)
    void serializeUserData() const;
    void deserializeUserData();

private:

    ModuleGeneratorPreLoader m_preLoader{};
    QString m_lastPreLoadPath{};

    /* settings */
    WizardMode m_wizardMode{Create};

    QString m_version{};

    bool m_useSignature{true};
    bool m_createGitFiles{true};
    bool m_useCompatibilityMacros{true};

    bool m_useCMakeGenerator{true};
    bool m_useQMakeGenerator{false};

    QString m_outputPath{};
    QString m_gtlabPath{};

    QString m_modulePrefix{};
    ModuleData m_moduleClass{};

    ClassDataList m_selectedInterfaces{};
    DependencyDataList m_selectedDependencies{};

    QString m_signature{};
    AuthorDetails m_authorDetails{};

    QFuture<int> m_futureDependencies{};
};

#endif // MODULEGENERATORSETTINGS_H
