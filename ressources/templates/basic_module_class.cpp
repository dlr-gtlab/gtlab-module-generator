$$SIGNATURE$$

/**
 * generated $$GENERATOR_VERSION$$
 */
 
#include "$$FILE_NAME$$.h"

$$INCLUDE_FILE$$$$QT_INCLUDE_FILE$$

GtVersionNumber
$$CLASS_NAME$$::version()
{
    return GtVersionNumber($$MODULE_VERSION$$);
}

QString
$$CLASS_NAME$$::ident() const
{
    return "$$MODULE_NAME$$";
}

QString
$$CLASS_NAME$$::description() const
{
    return "$$MODULE_DESCRIPTION$$";
}

$$FUNCTION$$


















