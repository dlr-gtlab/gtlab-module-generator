#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include($${PWD}/../../local_settings.pri)

TARGET_DIR_NAME = unittests
BUILD_DEST      = ../../build
MOC_BUILD_DEST  = $${BUILD_DEST}

TARGET = UnitTest

QT += core xml gui widgets concurrent

TEMPLATE = app
CONFIG += console

CONFIG(debug, debug|release){
    MOC_BUILD_DEST = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}
} else {
    MOC_BUILD_DEST = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}
}

OBJECTS_DIR = $${MOC_BUILD_DEST}/obj
MOC_DIR = $${MOC_BUILD_DEST}/moc
RCC_DIR = $${MOC_BUILD_DEST}/rcc
UI_DIR  = $${MOC_BUILD_DEST}/ui

DESTDIR = $${BUILD_DEST}

INCLUDEPATH += .\
    ../../src \
    ../../src/generator \
    ../../src/generator/settings \

HEADERS += \
    testhelper.h

SOURCES += \
    main.cpp \
    testhelper.cpp \
    test/test.cpp \
    #../../src/generator/module_generator.cpp \
    ../../src/generator/module_generator_logger.cpp \
    #../../src/generator/module_generator_preloader.cpp \
    ../../src/generator/module_generator_structs.cpp \
    ../../src/generator/module_generator_utils.cpp \
    #../../src/generator/settings/module_generator_settings.cpp \

### Module GeneratorF5
INCLUDEPATH += ../../src
INCLUDEPATH += ../../src/generator
INCLUDEPATH += ../../src/generator/settings

#### THIRD PARTY LIBRARIES
# Google Test
INCLUDEPATH += $${GOOGLE_TEST_PATH}/include
LIBS        += -L$${GOOGLE_TEST_PATH}/lib
DEPENDPATH  += $${GOOGLE_TEST_PATH}/lib

CONFIG(debug, debug|release) {
    # THIRD PARTY
    win32: LIBS += -lgtestd
    unix:  LIBS += -lgtest
} else {
    # THIRD PARTY
    LIBS += -lgtest
}

unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
}
