#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include(settings.pri)

TEMPLATE = subdirs
CONFIG += ordered console

#### Interface ####
contains(BUILD_MODULE, true) {
    message("BUILD_MODULE = true")
    SUBDIRS += src
}

#### UNIT TESTS ####
contains(BUILD_UNITTESTS, true) {
    message("BUILD_UNITTESTS = true")
    SUBDIRS += tests/unittests
}

######################################################################
