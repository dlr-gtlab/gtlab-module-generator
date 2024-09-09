#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include($${PWD}/../settings.pri)

CONFIG(debug, debug|release) {
    TARGET = $$CLASS_NAME$$-d
} else {
    TARGET = $$CLASS_NAME$$
}

QT += core widgets xml

# global define for module id
GT_MODULE_ID = $$MODULE_NAME$$
DEFINES += GT_MODULE_ID='"\\\"$${GT_MODULE_ID}\\\""'

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++14

CONFIG(debug, debug|release) {
    MOC_BUILD_DEST = ../build/debug
} else {
    MOC_BUILD_DEST = ../build/release
}

OBJECTS_DIR = $${MOC_BUILD_DEST}/obj
MOC_DIR = $${MOC_BUILD_DEST}/moc
RCC_DIR = $${MOC_BUILD_DEST}/rcc
UI_DIR  = $${MOC_BUILD_DEST}/ui
DESTDIR = ../$${LIB_BUILD_DEST}

INCLUDEPATH += . $$PRO_INCLUDEPATH$$

HEADERS += \
    $$PRO_HEADERPATH$$

SOURCES += \
    $$PRO_SOURCEPATH$$

message(Targeting Major Version: $${MAJOR_VERSION})

CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d
    #LIBS += -lGTlabNumerics-d

    LIBS += -lGTlabDataProcessor-d -lGTlabCore-d -lGTlabGui-d

    # Other$$PRO_LIBS_D$$
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging
    #LIBS += -lGTlabNumerics

    LIBS += -lGTlabDataProcessor -lGTlabCore -lGTlabGui

    # Other$$PRO_LIBS$$
}

# HDF5
#CONFIG(debug, debug|release) {
#    # C/C+ API
#    win32:LIBS += -lhdf5_D -lhdf5_cpp_D
#    unix:LIBS += -lhdf5 -lhdf5_cpp
#    # Wrapper
#    LIBS += -lGenH5-d
#} else {
#    # C/C+ API
#    LIBS += -lhdf5 -lhdf5_cpp
#    # Wrapper
#    LIBS += -lGenH5
#}

unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/..\''
}

######################################################################

copyHeaders($$HEADERS)
copyToEnvironmentPathModules()

######################################################################
