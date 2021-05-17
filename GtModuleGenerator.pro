QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
    src/main.cpp \
    src/settings/module_generator_settings.cpp \
    src/widgets/class_specification/abstract_class_specification_widget.cpp \
    src/widgets/class_specification/function_specification_widget.cpp \
    src/widgets/class_specification/multiple_class_specification_widget.cpp \
    src/widgets/class_specification/simple_class_specification_widget.cpp \
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
    src/generator/preloader.cpp \
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
    src/widgets/class_specification/abstract_class_specification_widget.h \
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
    src/generator/preloader.h \
    src/wizard/settings_page.h \
    src/wizard/signature_page.h \
    src/wizard/summary_page.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources/ressources.qrc
