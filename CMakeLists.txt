# SPDX-License-Identifier: MPL-2.0+
# SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)

#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __
#          / /_/ / / / / / /_/ / /_/ /
#         ____/ /_/ /_/\__,_/_.___/
######################################################################

cmake_minimum_required(VERSION 3.15)
project(GTlab-Module-Generator VERSION 1.4.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_UNITTESTS "Build Unittests" OFF)

find_package(Qt5 COMPONENTS Core Widgets Xml Concurrent REQUIRED)

add_subdirectory(src)

if (BUILD_UNITTESTS)
    enable_testing()
    add_subdirectory(tests/unittests)
endif ()

