# Unit tests for the $$MODULE_NAME$$ gtlab module 

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (NOT TARGET gtest)
    include(AddGoogleTest)
endif()

add_executable($$CLASS_NAME$$UnitTests
    main.cpp
    test.cpp
)

target_link_libraries($$CLASS_NAME$$UnitTests PRIVATE $$CLASS_NAME$$ gtest)

include(GoogleTest)
gtest_discover_tests($$CLASS_NAME$$UnitTests
                     TEST_PREFIX "$$CLASS_NAME$$."
                     DISCOVERY_MODE PRE_TEST)
