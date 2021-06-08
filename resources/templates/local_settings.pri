#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/              

# path to dev tools
DEV_TOOLS = $$DEVTOOLS_INSTALL_DIR$$

#  path to GTlab
GTLAB_DIR = $$GTLAB_INSTALL_DIR$$

# First Party
GTLAB_CORE_PATH = $${DEV_TOOLS}
GTLAB_LOGGING_PATH = $${DEV_TOOLS}
GTLAB_NUMERICS_PATH = $${DEV_TOOLS}

# Third Party
MINPACK_PATH = $${DEV_TOOLS}\ThirdPartyLibraries\minpack
SPLINE_LIB_PATH = $${DEV_TOOLS}\ThirdPartyLibraries\SplineLib
QWT_PATH = $${DEV_TOOLS}\ThirdPartyLibraries\Qwt
NLOPT_PATH = $${DEV_TOOLS}\ThirdPartyLibraries\NLopt

# Deployment
GTLAB_ENVIRONMENT_PATH =
