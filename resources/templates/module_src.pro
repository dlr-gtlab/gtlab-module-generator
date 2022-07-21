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

INCLUDEPATH += . $$PRO_INCLUDEPATH$$

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
        LIBS += -lGTlabDataProcessor-d -lGTlabCore-d -lGTlabGui-d
    } else {
        LIBS += -lGTlabDatamodel-d -lGTlabCore-d -lGTlabCalculators-d
        LIBS += -lGTlabMdi-d  -lGTlabNetwork-d
    }
    # MODULES$$PRO_LIBS_D$$
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging
    LIBS += -lGTlabNumerics

    greaterThan(MAJOR_VERSION, 1) {
        LIBS += -lGTlabDataProcessor -lGTlabCore -lGTlabGui
    } else {
        LIBS += -lGTlabDatamodel -lGTlabCore -lGTlabCalculators
        LIBS += -lGTlabMdi -lGTlabNetwork
    }
    # MODULES$$PRO_LIBS$$
}

# HDF5
#CONFIG(debug, debug|release) {
#    win32:LIBS += -lhdf5_D -lhdf5_cpp_D
#    unix:LIBS += -lhdf5 -lhdf5_cpp
#    LIBS += -lGenH5-d
#} else {
#    LIBS += -lhdf5 -lhdf5_cpp
#    LIBS += -lGenH5
#}

unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
}

######################################################################
