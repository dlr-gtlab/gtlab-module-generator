#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/              

# path to dev tools
DEV_TOOLS = $$DEVTOOLS_INSTALL_DIR$$

#  path to GTlab
GTLAB_DIR = $$GTLAB_INSTALL_DIR$$

# target version
MAJOR_VERSION = $$GTLAB_MAJOR_VERSION$$

# First Party
GTLAB_CORE_PATH                 = $${DEV_TOOLS}
GTLAB_LOGGING_PATH              = $${DEV_TOOLS}
GTLAB_NUMERICS_PATH             = $${DEV_TOOLS}
GTLAB_PHYSICS_PATH              = $${DEV_TOOLS}
GTLAB_H5_PATH                   = $${DEV_TOOLS}

# Collections
GTLAB_FUELCOLLECTION_PATH       = $${DEV_TOOLS}
GTLAB_FLUIDCOLLECTION_PATH      = $${DEV_TOOLS}
GTLAB_MAPCOLLECTION_PATH        = $${DEV_TOOLS}
GTLAB_MATERIALCOLLECTION_PATH   = $${DEV_TOOLS}
GTLAB_AIRFOILCOLLECTION_PATH    = $${DEV_TOOLS}

# Modules
GTLAB_POST_MODULE_PATH          = $${DEV_TOOLS}
GTLAB_PERFORMANCE_MODULE_PATH   = $${DEV_TOOLS}
GTLAB_PRE_DESIGN_MODULE_PATH    = $${DEV_TOOLS}
GTLAB_PYTHON_MODULE_PATH        = $${DEV_TOOLS}
GTLAB_RCE_MODULE_PATH           = $${DEV_TOOLS}
GTLAB_BASIC_TOOLS_PATH          = $${DEV_TOOLS}

GTLAB_CAD_KERNEL_PATH           = $${DEV_TOOLS}

# Third Party
QWT_PATH            = $${DEV_TOOLS}/ThirdPartyLibraries/Qwt
MINPACK_PATH        = $${DEV_TOOLS}/ThirdPartyLibraries/minpack
NLOPT_PATH          = $${DEV_TOOLS}/ThirdPartyLibraries/NLopt
HDF5_PATH           = $${DEV_TOOLS}/ThirdPartyLibraries/hdf5

# Deployment
GTLAB_ENVIRONMENT_PATH =
