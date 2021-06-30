#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include(settings.pri)

BUILD_DEST     = $${GTLAB_DIR}/$$GTLAB_INSTALL_SUB_DIR$$/modules
MOC_BUILD_DEST = ./build

CONFIG(debug, debug|release) {
    TARGET = $$CLASS_NAME$$-d
} else {
    TARGET = $$CLASS_NAME$$
}

QT += core widgets xml

TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++11

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
    src $$PRO_INCLUDEPATH$$

HEADERS += \
    src/$$FILE_NAME$$.h $$PRO_HEADERPATH$$

SOURCES += \
    src/$$FILE_NAME$$.cpp $$PRO_SOURCEPATH$$

CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d
    LIBS += -lGTlabDatamodel-d
    LIBS += -lGTlabNumerics-d
    LIBS += -lGTlabCalculators-d
    LIBS += -lGTlabCore-d
    LIBS += -lGTlabMdi-d
    LIBS += -lGTlabNetwork-d
    # MODULES$$PRO_LIBS_D$$
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging
    LIBS += -lGTlabDatamodel
    LIBS += -lGTlabNumerics
    LIBS += -lGTlabCalculators
    LIBS += -lGTlabCore
    LIBS += -lGTlabMdi
    LIBS += -lGTlabNetwork
    # MODULES$$PRO_LIBS$$
}

unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
}

######################################################################
