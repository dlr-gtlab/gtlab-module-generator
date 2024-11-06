# GTlab Module Generator
#
# SPDX-License-Identifier: MPL-2.0+
# SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (WIN32)
    set(TYPE WIN32)
endif(WIN32)

add_executable(GtModuleGenerator ${TYPE}

    generator/module_generator.cpp
    generator/module_generator_logger.cpp
    generator/module_generator_preloader.cpp
    generator/module_generator_structs.cpp
    generator/module_generator_utils.cpp
    main.cpp
    generator/settings/module_generator_settings.cpp
    widgets/class_specification/abstract_type_specification_item.cpp
    widgets/class_specification/basic_type_specification_items.cpp
    widgets/class_specification/class_specification_item.cpp
    widgets/class_specification/class_specification_list.cpp
    widgets/class_specification/function_specification_widget.cpp
    widgets/class_specification/class_specification_widget.cpp
    widgets/dependency_selection_widget.cpp
    widgets/interface_selection_widget.cpp
    widgets/widget_list_view.cpp
    wizard/module_wizard.cpp
    wizard/pages/dependency_selection_page.cpp
    wizard/pages/interface_selection_page.cpp
    wizard/pages/interface_specifications_page.cpp
    wizard/pages/intro_page.cpp
    wizard/pages/module_specifications_page.cpp
    wizard/pages/settings_page.cpp
    wizard/pages/signature_page.cpp
    wizard/pages/summary_page.cpp

    generator/module_generator.h
    generator/module_generator_logger.h
    generator/module_generator_preloader.h
    generator/module_generator_structs.h
    generator/module_generator_utils.h
    generator/settings/module_generator_settings.h
    widgets/class_specification/abstract_type_specification_item.h
    widgets/class_specification/basic_type_specification_items.h
    widgets/class_specification/class_specification_item.h
    widgets/class_specification/class_specification_list.h
    widgets/class_specification/function_specification_widget.h
    widgets/class_specification/class_specification_widget.h
    widgets/dependency_selection_widget.h
    widgets/interface_selection_widget.h
    widgets/widget_list_view.h
    wizard/module_wizard.h
    wizard/pages/abstract_wizardpage.h
    wizard/pages/dependency_selection_page.h
    wizard/pages/interface_selection_page.h
    wizard/pages/interface_specifications_page.h
    wizard/pages/intro_page.h
    wizard/pages/module_specifications_page.h
    wizard/pages/settings_page.h
    wizard/pages/signature_page.h
    wizard/pages/summary_page.h

    ${PROJECT_SOURCE_DIR}/resources/resources.qrc
)

if (WIN32)
	target_sources(GtModuleGenerator PRIVATE app.rc)
endif(WIN32)

target_include_directories(GtModuleGenerator
	PRIVATE
	.
	wizard
	wizard/pages
    generator
    generator/settings
    widgets
    widgets/class_specification
)

target_link_libraries(GtModuleGenerator
    PRIVATE Qt5::Xml Qt5::Widgets Qt5::Gui Qt5::Concurrent
)

install(TARGETS GtModuleGenerator
    RUNTIME DESTINATION bin
)
