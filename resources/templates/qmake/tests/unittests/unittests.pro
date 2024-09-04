#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( $${PWD}/../../settings.pri )

BUILD_DEST = $${PWD}/../../build

TARGET = $$CLASS_NAME$$UnitTest

QT += core xml gui widgets concurrent

TEMPLATE = app
CONFIG += console
CONFIG += c++14

CONFIG(debug, debug|release){
    MOC_BUILD_DEST = $${BUILD_DEST}/debug-unittests
} else {
    MOC_BUILD_DEST = $${BUILD_DEST}/release-unittests
}

OBJECTS_DIR = $${MOC_BUILD_DEST}/obj
MOC_DIR = $${MOC_BUILD_DEST}/moc
RCC_DIR = $${MOC_BUILD_DEST}/rcc
UI_DIR  = $${MOC_BUILD_DEST}/ui

DESTDIR = $${BUILD_DEST}

#### INCLUDES
INCLUDEPATH += $${PWD}/../../include/$$TARGET_DIR_NAME$$

#### THIRD PARTY LIBRARIES
# Google Test
INCLUDEPATH += $${GOOGLE_TEST_PATH}/include
LIBS        += -L$${GOOGLE_TEST_PATH}/lib
DEPENDPATH  += $${GOOGLE_TEST_PATH}/lib

#### HEADERS
HEADERS += $$files(*.h)

#### SOURCES
SOURCES += main.cpp
SOURCES += $$files(*.cpp)

#### LIBS
CONFIG(debug, debug|release) {
    LIBS += -lGTlabLogging-d	
    LIBS += -L$$CLASS_NAME$$-d
    # THIRD PARTY
    win32: LIBS += -lgtestd
    unix:  LIBS += -lgtest
} else {
    LIBS += -lGTlabLogging
    LIBS += -L$$CLASS_NAME$$
    # THIRD PARTY
    LIBS += -lgtest
}

####################################################

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/modules\''

######################################################################
