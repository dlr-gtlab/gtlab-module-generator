QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS

BUILD_DEST = ./build

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
    RC_FILE = src/app.rc
}

DESTDIR = $${BUILD_DEST}

INCLUDEPATH += . \
    src \
    src/wizard \
    src/wizard/pages \
    src/generator \
    src/generator/settings \
    src/widgets \
    src/widgets/class_specification

SOURCES += \
    src/generator/module_generator.cpp \
    src/generator/module_generator_logger.cpp \
    src/generator/module_generator_preloader.cpp \
    src/generator/module_generator_utils.cpp \
    src/main.cpp \
    src/generator/settings/module_generator_settings.cpp \
    src/widgets/class_specification/function_specification_widget.cpp \
    src/widgets/class_specification/multiple_class_specification_widget.cpp \
    src/widgets/class_specification/class_specification_widget.cpp \
    src/widgets/class_specification/single_class_specification_widget.cpp \
    src/widgets/dependency_selection_widget.cpp \
    src/widgets/interface_selection_widget.cpp \
    src/widgets/widget_list_view.cpp \
    src/wizard/module_wizard.cpp \
    src/wizard/pages/dependency_selection_page.cpp \
    src/wizard/pages/interface_selection_page.cpp \
    src/wizard/pages/interface_specifications_page.cpp \
    src/wizard/pages/intro_page.cpp \
    src/wizard/pages/module_specifications_page.cpp \
    src/wizard/pages/settings_page.cpp \
    src/wizard/pages/signature_page.cpp \
    src/wizard/pages/summary_page.cpp

HEADERS += \
    src/generator/module_generator.h \
    src/generator/module_generator_logger.h \
    src/generator/module_generator_preloader.h \
    src/generator/module_generator_structs.h \
    src/generator/module_generator_utils.h \
    src/generator/settings/module_generator_settings.h \
    src/widgets/class_specification/abstract_class_specification.h \
    src/widgets/class_specification/function_specification_widget.h \
    src/widgets/class_specification/multiple_class_specification_widget.h \
    src/widgets/class_specification/simple_class_specification_widget.h \
    src/widgets/class_specification/class_specification_widget.h \
    src/widgets/class_specification/single_class_specification_widget.h \
    src/widgets/dependency_selection_widget.h \
    src/widgets/interface_selection_widget.h \
    src/widgets/widget_list_view.h \
    src/wizard/module_wizard.h \
    src/wizard/pages/abstract_wizardpage.h \
    src/wizard/pages/dependency_selection_page.h \
    src/wizard/pages/interface_selection_page.h \
    src/wizard/pages/interface_specifications_page.h \
    src/wizard/pages/intro_page.h \
    src/wizard/pages/module_specifications_page.h \
    src/wizard/pages/settings_page.h \
    src/wizard/pages/signature_page.h \
    src/wizard/pages/summary_page.h

RESOURCES += \
    resources/resources.qrc

unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
}
