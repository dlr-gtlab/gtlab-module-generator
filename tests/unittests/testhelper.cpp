/* GTlab - Gas Turbine laboratory
 * copyright 2009-2021 by DLR
 *
 * Created on: 30.08.2021
 * Author: Marius Bröcker
 * Email: marius.broecker@dlr.de
 */

#include "testhelper.h"

#include <QCoreApplication>
#include <QDir>
#include <QUuid>

TestHelper*
TestHelper::instance()
{
    static TestHelper instance{};
    return &instance;
}

QString
TestHelper::tempPath() const
{
    return QCoreApplication::applicationDirPath() + QDir::separator() +
            QStringLiteral("unittest-temp");
}

void
TestHelper::reset()
{
    QDir tempDir(tempPath());

    if (tempDir.exists())
    {
        tempDir.removeRecursively();
    }
}

QStringList
TestHelper::randomStringList(int length) const
{
    QStringList retVal;
    retVal.reserve(length);

    for (int i = 0; i < length; ++i)
    {
        retVal.append(QUuid::createUuid().toString());
    }

    return retVal;
}

QVector<QByteArray>
TestHelper::randomByteArrays(int length) const
{
    QVector<QByteArray> retVal;
    retVal.reserve(length);

    for (int i = 0; i < length; ++i)
    {
        retVal.append(QUuid::createUuid().toString().toUtf8());
    }

    return retVal;
}
