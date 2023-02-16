#include "module_generator_structs.h"

#include <QVersionNumber>
#include <QRegularExpression>
#include <QDebug>

bool
details::isEnabledHelper(QString const& vMin,
                         QString const& vMax,
                         QString const& vTarget)
{
    auto target = QVersionNumber::fromString(vTarget);
    return (vMin.isEmpty() || QVersionNumber::fromString(vMin) <= target) &&
           (vMax.isEmpty() || QVersionNumber::fromString(vMax) > target);
}

QString
TypeInfo::innerType(QString const& oReturnType, int idx) const
{
    QRegularExpression regExp{"^" + this->returnType + "$"};
    auto match = regExp.match(oReturnType);
    if (!match.hasMatch() || !match.capturedLength(idx))
    {
        return oReturnType;
    }
    return match.captured(idx);
}
