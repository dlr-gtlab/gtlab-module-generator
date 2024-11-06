/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include "gtest/gtest.h"

#include <QCoreApplication>
#include <QDebug>

#include "testhelper.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    auto* helper = TestHelper::instance();
    Q_UNUSED(helper)

    testing::InitGoogleTest(&argc, argv);

    int retval = RUN_ALL_TESTS();

    return retval;
}
