#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __
#          / /_/ / / / / / /_/ / /_/ /
#         ____/ /_/ /_/\__,_/_.___/
#
######################################################################

add_gtlab_module($$CLASS_NAME$$ MODULE_ID "$$MODULE_NAME$$"
    SOURCES
    $$CMAKE_SOURCE_FILES$$
    $$CMAKE_ADDITIONAL_FILES$$
)
$$CMAKE_MODULE_ALIAS$$
target_link_libraries($$CLASS_NAME$$
    PUBLIC GTlab::Gui
    PRIVATE Qt5::Xml Qt5::Widgets$$CMAKE_TARGET_LINK_LIBRARIES$$
)

set_property(TARGET $$CLASS_NAME$$ PROPERTY AUTOMOC ON)

target_include_directories($$CLASS_NAME$$ PUBLIC $$CMAKE_INCLUDE_DIRS$$
)
