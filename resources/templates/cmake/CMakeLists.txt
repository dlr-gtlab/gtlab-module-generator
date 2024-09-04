#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __
#          / /_/ / / / / / /_/ / /_/ /
#         ____/ /_/ /_/\__,_/_.___/
######################################################################

cmake_minimum_required(VERSION 3.15)
project($$CLASS_NAME$$ VERSION 2.3.3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

option(BUILD_UNITTESTS "Build Unittests" OFF)

include(GTlab)
enable_gtlab_devtools()
gtlab_standard_setup()

find_package(Qt5 COMPONENTS Core Widgets Xml REQUIRED)

# support for monobuilds
if (NOT TARGET GTlab::Core)
  find_package(GTlab REQUIRED)
endif()
$$CMAKE_FIND_PACKAGE_DEPS$$
add_subdirectory(src)

if (BUILD_UNITTESTS)
    enable_testing()
    add_subdirectory(tests/unittests)
endif ()


