set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (NOT TARGET gtest)
    include(AddGoogleTest)
endif()

add_executable(unittests
    testhelper.h

    main.cpp
    testhelper.cpp
    test/test.cpp
    ../../src/generator/module_generator_logger.cpp
    ../../src/generator/module_generator_structs.cpp
    ../../src/generator/module_generator_utils.cpp
)

target_include_directories(unittests
	PRIVATE
	.
    ../../src
    ../../src/generator
    ../../src/generator/settings
)

target_link_libraries(unittests
    PRIVATE Qt5::Xml Qt5::Widgets Qt5::Gui Qt5::Concurrent gtest
)

