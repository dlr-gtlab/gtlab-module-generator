QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

CONFIG += c++11

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
        src/settings \
        src/generator \
        src/widgets \
        src/widgets/class_specification

SOURCES += \
    src/generator/module_generator.cpp \
    src/generator/module_generator_logger.cpp \
    src/generator/module_generator_structs.cpp \
    src/generator/module_generator_utils.cpp \
    src/generator/preloader.cpp \
    src/main.cpp \
    src/settings/module_generator_settings.cpp \
    src/widgets/class_specification/function_specification_widget.cpp \
    src/widgets/class_specification/multiple_class_specification_widget.cpp \
    src/widgets/class_specification/class_specification_widget.cpp \
    src/widgets/class_specification/single_class_specification_widget.cpp \
    src/widgets/dependencie_selection_widget.cpp \
    src/widgets/interface_selection_widget.cpp \
    src/widgets/widget_list_view.cpp \
    src/wizard/dependencie_selection_page.cpp \
    src/wizard/interface_selection_page.cpp \
    src/wizard/interface_specifications_page.cpp \
    src/wizard/intro_page.cpp \
    src/wizard/module_specifications_page.cpp \
    src/wizard/module_wizard.cpp \
    src/wizard/settings_page.cpp \
    src/wizard/signature_page.cpp \
    src/wizard/summary_page.cpp

HEADERS += \
    src/generator/module_generator.h \
    src/generator/module_generator_logger.h \
    src/generator/module_generator_structs.h \
    src/generator/module_generator_utils.h \
    src/settings/module_generator_settings.h \
    src/generator/preloader.h \
    src/widgets/class_specification/abstract_class_specification.h \
    src/widgets/class_specification/function_specification_widget.h \
    src/widgets/class_specification/multiple_class_specification_widget.h \
    src/widgets/class_specification/simple_class_specification_widget.h \
    src/widgets/class_specification/class_specification_widget.h \
    src/widgets/class_specification/single_class_specification_widget.h \
    src/widgets/dependencie_selection_widget.h \
    src/widgets/interface_selection_widget.h \
    src/widgets/widget_list_view.h \
    src/wizard/abstract_wizardpage.h \
    src/wizard/dependencie_selection_page.h \
    src/wizard/interface_selection_page.h \
    src/wizard/interface_specifications_page.h \
    src/wizard/intro_page.h \
    src/wizard/module_specifications_page.h \
    src/wizard/module_wizard.h \
    src/wizard/settings_page.h \
    src/wizard/signature_page.h \
    src/wizard/summary_page.h

RESOURCES += \
    resources/resources.qrc
