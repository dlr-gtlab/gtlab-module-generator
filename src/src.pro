QT += core gui concurrent
QT += widgets xml

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = GtModuleGenerator

BUILD_DEST = ../build
DESTDIR = $${BUILD_DEST}

CONFIG(debug, debug|release){
    OBJECTS_DIR = $${BUILD_DEST}/debug-app/obj
    MOC_DIR = $${BUILD_DEST}/debug-app/moc
    RCC_DIR = $${BUILD_DEST}/debug-app/rcc
    UI_DIR = $${BUILD_DEST}/debug-app/ui
} else {
    OBJECTS_DIR = $${BUILD_DEST}/release-app/obj
    MOC_DIR = $${BUILD_DEST}/release-app/moc
    RCC_DIR = $${BUILD_DEST}/release-app/rcc
    UI_DIR = $${BUILD_DEST}/release-app/ui
}

win32 {
    RC_FILE = ./app.rc
}

INCLUDEPATH += . \
    wizard \
    wizard/pages \
    generator \
    generator/settings \
    widgets \
    widgets/class_specification

SOURCES += \
    generator/module_generator.cpp \
    generator/module_generator_logger.cpp \
    generator/module_generator_preloader.cpp \
    generator/module_generator_utils.cpp \
    main.cpp \
    generator/settings/module_generator_settings.cpp \
    widgets/class_specification/class_specification_item.cpp \
    widgets/class_specification/class_specification_list.cpp \
    widgets/class_specification/function_specification_widget.cpp \
    widgets/class_specification/class_specification_widget.cpp \
    widgets/class_specification/type_specification_widget.cpp \
    widgets/dependency_selection_widget.cpp \
    widgets/interface_selection_widget.cpp \
    widgets/widget_list_view.cpp \
    wizard/module_wizard.cpp \
    wizard/pages/dependency_selection_page.cpp \
    wizard/pages/interface_selection_page.cpp \
    wizard/pages/interface_specifications_page.cpp \
    wizard/pages/intro_page.cpp \
    wizard/pages/module_specifications_page.cpp \
    wizard/pages/settings_page.cpp \
    wizard/pages/signature_page.cpp \
    wizard/pages/summary_page.cpp

HEADERS += \
    generator/module_generator.h \
    generator/module_generator_logger.h \
    generator/module_generator_preloader.h \
    generator/module_generator_structs.h \
    generator/module_generator_utils.h \
    generator/settings/module_generator_settings.h \
    widgets/class_specification/abstract_type_specifier.h \
    widgets/class_specification/class_specification_item.h \
    widgets/class_specification/class_specification_list.h \
    widgets/class_specification/function_specification_widget.h \
    widgets/class_specification/class_specification_widget.h \
    widgets/class_specification/type_specification_widget.h \
    widgets/dependency_selection_widget.h \
    widgets/interface_selection_widget.h \
    widgets/widget_list_view.h \
    wizard/module_wizard.h \
    wizard/pages/abstract_wizardpage.h \
    wizard/pages/dependency_selection_page.h \
    wizard/pages/interface_selection_page.h \
    wizard/pages/interface_specifications_page.h \
    wizard/pages/intro_page.h \
    wizard/pages/module_specifications_page.h \
    wizard/pages/settings_page.h \
    wizard/pages/signature_page.h \
    wizard/pages/summary_page.h

RESOURCES += \
    ../resources/resources.qrc

unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
}
