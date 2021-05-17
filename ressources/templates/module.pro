#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include(settings.pri)

BUILD_DEST = $${GTLAB_DIR}/bin/modules

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

INCLUDEPATH += . \
    src $$PRO_INCLUDEPATH$$

DESTDIR = $${GTLAB_DIR}/bin/modules

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
    # THIRD PARTY
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging
    LIBS += -lGTlabDatamodel
    LIBS += -lGTlabNumerics
    LIBS += -lGTlabCalculators
    LIBS += -lGTlabCore
    LIBS += -lGTlabMdi
    LIBS += -lGTlabNetwork
    # THIRD PARTY
}

######################################################################

######################################################################


