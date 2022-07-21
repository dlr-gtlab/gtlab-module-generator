$$SIGNATURE$$

#ifndef $$HEADER_NAME$$_H
#define $$HEADER_NAME$$_H

#include "gt_globals.h"

#include "gt_moduleinterface.h"
$$INCLUDE_FILE$$
#if (GT_VERSION < GT_VERSION_CHECK(2, 0, 0))
#include "gt_initmoduleinterface.h"
#endif

$$CLASS_FORWARD_DECL$$
/**
 * @generated $$GENERATOR_VERSION$$
 * @brief The $$CLASS_NAME$$ class
 */
class $$CLASS_NAME$$ : public QObject,
        public GtModuleInterface$$DERIVE_BASE_CLASS$$
#if (GT_VERSION < GT_VERSION_CHECK(2, 0, 0))
        ,public GtInitModuleInterface
#endif
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.dlr.gtlab.GtModuleInterface/0.1"
                      FILE "$$FILE_NAME$$.json")

    Q_INTERFACES(GtModuleInterface)
#if (GT_VERSION < GT_VERSION_CHECK(2, 0, 0))
    Q_INTERFACES(GtInitModuleInterface)
#endif
    $$INTERFACE_MACRO$$
public:

    /**
     * @brief Returns current version number of module
     * @return version number
     */
    GtVersionNumber version() override;

    /**
     * @brief Returns module identification string.
     * @return identification string
     */
    QString ident() const override;

    /**
     * @brief Returns module description
     * @return description
     */
    QString description() const override;

#if (GT_VERSION < GT_VERSION_CHECK(2, 0, 0))
#else
    /**
     * @brief In some cases, it is necessary to upgrade the data stored in the
     * project when the module version is updated. This function can be used to
     * add your own update routines to the framework.
     * @return List of all upgrade routines of the module.
     */
    QList<VersionUpgradeRoutine> upgradeRoutines() const override {
        return {};
    }

    /**
     * @brief Returns the functions that the module likes to share.
     *
     * A shared function is a special kind of function that can be
     * used by other modules or the core framework without the need
     * to compile against this modules.
     * This is possible since a shared function of type InterfaceFunction
     * has a fixed function signature:
     * QVariantList sharedFunction(const QVariantList&).
     *
     * @return shared functions
     */
    QList<gtlab::InterfaceFunction> sharedFunctions() const override {
        return {};
    }
#endif

    /**
     * @brief Initializes module. Called on application startup.
     */
    void init() override;$$FUNCTION$$
};

#endif // $$HEADER_NAME$$_H
