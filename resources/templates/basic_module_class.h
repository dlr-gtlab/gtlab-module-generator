$$SIGNATURE$$#ifndef $$HEADER_NAME$$_H
#define $$HEADER_NAME$$_H

#include "gt_moduleinterface.h"
$$INCLUDE_FILE$$


$$CLASS_FORWARD_DECL$$
/**
 * @generated $$GENERATOR_VERSION$$
 * @brief The $$CLASS_NAME$$ class
 */
class $$CLASS_NAME$$ : public QObject,
        public GtModuleInterface$$DERIVE_BASE_CLASS$$
{
    Q_OBJECT

    GT_MODULE("$$FILE_NAME$$.json")

    Q_INTERFACES(GtModuleInterface)
    $$INTERFACE_MACRO$$
public:

    /**
     * @brief Returns current version number of module
     * @return version number
     */
    GtVersionNumber version() override;

    /**
     * @brief Returns module description
     * @return description
     */
    QString description() const override;

    /**
     * @brief Initializes module. Called on application startup.
     */
    void init() override;

    /**
     * @brief Passes additional module information to the framework.
     *
     * NOTE: A reference to the author can significantly help the user to
     * know who to contact in case of issues or other request.
     * @return module meta information.
     */
    MetaInformation metaInformation() const override;
$$FUNCTION$$
};

#endif // $$HEADER_NAME$$_H
