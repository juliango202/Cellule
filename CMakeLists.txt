PROJECT(cellule)

# Set a default build type for single-configuration
# CMake generators if no build type is set.
#
IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

# Warning: GCC SPECIFIC
SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

SUBDIRS(base nodework scene appli)
