/*
 * GTlab Module Generator
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include "gtest/gtest.h"
#include "testhelper.h"

#include <QDebug>

#include "module_generator_structs.h"

struct Tests : public testing::Test
{

};

TEST_F(Tests, VersionData)
{
    VersionData vers;
    vers.min = "1.2.0";
    vers.max = "2.0.0";

    EXPECT_TRUE(vers.isEnabled(vers.min));
    EXPECT_TRUE(vers.isEnabled("1.5.1"));
    EXPECT_TRUE(vers.isEnabled("1.99.99"));
    EXPECT_FALSE(vers.isEnabled("1.1.99"));
    EXPECT_FALSE(vers.isEnabled(vers.max));
}
