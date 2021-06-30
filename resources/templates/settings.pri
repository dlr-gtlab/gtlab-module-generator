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

### PHYSICS ###
INCLUDEPATH += $${GTLAB_PHYSICS_PATH}/include/physics
LIBS        += -L$${GTLAB_PHYSICS_PATH}/lib/physics
DEPENDPATH  += $${GTLAB_PHYSICS_PATH}/lib/physics

### COLLECTIONS ###
INCLUDEPATH += $${GTLAB_FUELCOLLECTION_PATH}/include/fuelcollection
LIBS        += -L$${GTLAB_FUELCOLLECTION_PATH}/lib/fuelcollection
DEPENDPATH  += $${GTLAB_FUELCOLLECTION_PATH}/lib/fuelcollection

INCLUDEPATH += $${GTLAB_FLUIDCOLLECTION_PATH}/include/fluidcollection
LIBS        += -L$${GTLAB_FLUIDCOLLECTION_PATH}/lib/fluidcollection
DEPENDPATH  += $${GTLAB_FLUIDCOLLECTION_PATH}/lib/fluidcollection

INCLUDEPATH += $${GTLAB_MAPCOLLECTION_PATH}/include/mapcollection
LIBS        += -L$${GTLAB_MAPCOLLECTION_PATH}/lib/mapcollection
DEPENDPATH  += $${GTLAB_MAPCOLLECTION_PATH}/lib/mapcollection

INCLUDEPATH += $${GTLAB_MATERIALCOLLECTION_PATH}/include/materialcollection
LIBS        += -L$${GTLAB_MATERIALCOLLECTION_PATH}/lib/materialcollection
DEPENDPATH  += $${GTLAB_MATERIALCOLLECTION_PATH}/lib/materialcollection

INCLUDEPATH += $${GTLAB_AIRFOILCOLLECTION_PATH}/include/airfoilcollection
LIBS        += -L$${GTLAB_AIRFOILCOLLECTION_PATH}/lib/airfoilcollection
DEPENDPATH  += $${GTLAB_AIRFOILCOLLECTION_PATH}/lib/airfoilcollection

### CAD KERNEL ###
INCLUDEPATH += $${GTLAB_CAD_KERNEL_PATH}/include/cadkernel
LIBS        += -L$${GTLAB_CAD_KERNEL_PATH}/lib/cadkernel
DEPENDPATH  += $${GTLAB_CAD_KERNEL_PATH}/lib/cadkernel

### BASIC TOOLS MODULE ###
INCLUDEPATH += $${GTLAB_BASIC_TOOLS_PATH}/include/basictools
LIBS        += -L$${GTLAB_BASIC_TOOLS_PATH}/lib/basictools
DEPENDPATH  += $${GTLAB_BASIC_TOOLS_PATH}/lib/basictools

### POST MODULE ###
INCLUDEPATH += $${GTLAB_POST_MODULE_PATH}/include/post
LIBS        += -L$${GTLAB_POST_MODULE_PATH}/lib/post
DEPENDPATH  += $${GTLAB_POST_MODULE_PATH}/lib/post

### PERFORMANCE MODULE ###
INCLUDEPATH += $${GTLAB_PERFORMANCE_MODULE_PATH}/include/performance
LIBS        += -L$${GTLAB_PERFORMANCE_MODULE_PATH}/lib/performance
DEPENDPATH  += $${GTLAB_PERFORMANCE_MODULE_PATH}/lib/performance

### PRE DESIGN MODULE ###
INCLUDEPATH += $${GTLAB_PRE_DESIGN_MODULE_PATH}/include/predesign
LIBS        += -L$${GTLAB_PRE_DESIGN_MODULE_PATH}/lib/predesign
DEPENDPATH  += $${GTLAB_PRE_DESIGN_MODULE_PATH}/lib/predesign

### PYTHON MODULE ###
INCLUDEPATH += $${GTLAB_PYTHON_MODULE_PATH}/include/python37
LIBS        += -L$${GTLAB_PYTHON_MODULE_PATH}/lib/python37
DEPENDPATH  += $${GTLAB_PYTHON_MODULE_PATH}/lib/python37

### RCE MODULE ###
INCLUDEPATH += $${GTLAB_RCE_MODULE_PATH}/include/rce
LIBS        += -L$${GTLAB_RCE_MODULE_PATH}/lib/rce
DEPENDPATH  += $${GTLAB_RCE_MODULE_PATH}/lib/rce



#### THIRD PARTY LIBRARIES
# MINPACK
INCLUDEPATH += $${GTLAB_MINPACK_PATH}/include
LIBS        += -L$${GTLAB_MINPACK_PATH}/lib
DEPENDPATH  += $${GTLAB_MINPACK_PATH}/lib

## QWT
INCLUDEPATH += $${GTLAB_QWT_PATH}/include
LIBS        += -L$${GTLAB_QWT_PATH}/lib
DEPENDPATH  += $${GTLAB_QWT_PATH}/lib

## NLOPT
INCLUDEPATH += $${GTLAB_NLOPT_PATH}/include
LIBS        += -L$${GTLAB_NLOPT_PATH}/lib
DEPENDPATH  += $${GTLAB_NLOPT_PATH}/lib


######################################################################
