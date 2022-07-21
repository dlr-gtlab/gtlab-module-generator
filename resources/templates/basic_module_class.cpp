$$SIGNATURE$$

/*
 * generated $$GENERATOR_VERSION$$
 */
 
#include "$$FILE_NAME$$.h"

$$INCLUDE_FILE$$
$$ADD_INCLUDE_FILE$$
$$QT_INCLUDE_FILE$$

GtVersionNumber
$$CLASS_NAME$$::version()
{
    return GtVersionNumber{$$MODULE_VERSION$$};
}

QString
$$CLASS_NAME$$::ident() const
{
    return QStringLiteral("$$MODULE_NAME$$");
}

QString
$$CLASS_NAME$$::description() const
{
    return QStringLiteral("$$MODULE_DESCRIPTION$$");
}

void
$$CLASS_NAME$$::init()
{
    // TODO: code to execute on init
}$$FUNCTION$$
