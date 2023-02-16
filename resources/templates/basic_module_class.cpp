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
$$CLASS_NAME$$::description() const
{
    return QStringLiteral("$$MODULE_DESCRIPTION$$");
}

void
$$CLASS_NAME$$::init()
{
    // TODO: code to execute on init
}

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
$$CLASS_NAME$$::MetaInformation
$$CLASS_NAME$$::metaInformation() const
{
    MetaInformation m;

    m.author =        QStringLiteral("$$AUTHOR$$");
    m.authorContact = QStringLiteral("$$AUTHOR_EMAIL$$");

    // TODO: set license
    // m.licenseShort = ...;

    return m;
}
#endif$$FUNCTION$$
