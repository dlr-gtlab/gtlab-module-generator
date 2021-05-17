$$SIGNATURE$$

#ifndef $$HEADER_NAME$$_H
#define $$HEADER_NAME$$_H

#include "gt_moduleinterface.h"
$$INCLUDE_FILE$$


/**
 * @generated $$GENERATOR_VERSION$$
 * @brief The $$CLASS_NAME$$ class
 */
class $$CLASS_NAME$$ : public QObject,
        public GtModuleInterface$$DERIVE_BASE_CLASS$$
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.dlr.gtlab.GtModuleInterface/0.1"
                      FILE "$$FILE_NAME$$.json")
					  
    Q_INTERFACES(GtModuleInterface)
    $$INTERFACE_MACRO$$	
public:

	/**
     * @brief Returns current version number of module
     * @return version number
     */
    GtVersionNumber version() Q_DECL_OVERRIDE;

    /**
     * @brief Returns module identification string.
     * @return identification string
     */
    QString ident() const Q_DECL_OVERRIDE;

    /**
     * @brief Returns module description
     * @return description
     */
    QString description() const Q_DECL_OVERRIDE;

	$$FUNCTION$$	
};

#endif // $$HEADER_NAME$$_H
