#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

#### LOCAL SETTINGS
include(local_settings.pri)

#### GTlab - LIBRARIES
### CORE ###
INCLUDEPATH += $${GTLAB_CORE_PATH}/include/core
LIBS        += -L$${GTLAB_CORE_PATH}/lib/core
DEPENDPATH  += $${GTLAB_CORE_PATH}/lib/core

### LOGGING ###
INCLUDEPATH += $${GTLAB_LOGGING_PATH}/include/logging
LIBS        += -L$${GTLAB_LOGGING_PATH}/lib/logging
DEPENDPATH  += $${GTLAB_LOGGING_PATH}/lib/logging

### NUMERICS ###
INCLUDEPATH += $${GTLAB_NUMERICS_PATH}/include/numerics
LIBS        += -L$${GTLAB_NUMERICS_PATH}/lib/numerics
DEPENDPATH  += $${GTLAB_NUMERICS_PATH}/lib/numerics


#### THIRD PARTY LIBRARIES


######################################################################
