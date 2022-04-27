#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include($${PWD}/../settings.pri)

BUILD_DEST     = $${GTLAB_DIR}/$$GTLAB_INSTALL_SUB_DIR$$/modules
MOC_BUILD_DEST = ../build

CONFIG(debug, debug|release) {
    TARGET = $$CLASS_NAME$$-d
} else {
    TARGET = $$CLASS_NAME$$
}

QT += core widgets xml

TEMPLATE = lib
CONFIG += plugin
CONFIG += silent

CONFIG(debug, debug|release) {
    OBJECTS_DIR = $${MOC_BUILD_DEST}/debug-app/obj
    MOC_DIR = $${MOC_BUILD_DEST}/debug-app/moc
    RCC_DIR = $${MOC_BUILD_DEST}/debug-app/rcc
    UI_DIR  = $${MOC_BUILD_DEST}/debug-app/ui
} else {
    OBJECTS_DIR = $${MOC_BUILD_DEST}/release-app/obj
    MOC_DIR = $${MOC_BUILD_DEST}/release-app/moc
    RCC_DIR = $${MOC_BUILD_DEST}/release-app/rcc
    UI_DIR  = $${MOC_BUILD_DEST}/release-app/ui
}

DESTDIR = $${BUILD_DEST}

INCLUDEPATH += . \
    $$PRO_INCLUDEPATH$$

HEADERS += \
    $$FILE_NAME$$.h $$PRO_HEADERPATH$$

SOURCES += \
    $$FILE_NAME$$.cpp $$PRO_SOURCEPATH$$

message(Targeting Major Version: $${MAJOR_VERSION})

CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d
    LIBS += -lGTlabNumerics-d

    greaterThan(MAJOR_VERSION, 1) {
        LIBS += -lGTlabCore-d -lGTlabGui-d -lGTlabDataProcessor-d
    } else {
        LIBS += -lGTlabCalculators-d -lGTlabCore-d
        LIBS += -lGTlabMdi-d -lGTlabDatamodel-d
    }
    # MODULES$$PRO_LIBS_D$$
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging
    LIBS += -lGTlabNumerics

    greaterThan(MAJOR_VERSION, 1) {
        LIBS += -lGTlabCore -lGTlabGui -lGTlabDataProcessor
    } else {
        LIBS += -lGTlabCalculators -lGTlabCore
        LIBS += -lGTlabMdi -lGTlabDatamodel
    }
    # MODULES$$PRO_LIBS$$
}

unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
}

######################################################################
