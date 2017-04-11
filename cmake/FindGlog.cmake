
# - Try to find Glog
#
# The following variables are optionally searched for defaults
#  GLOG_ROOT:            Base directory where all GLOG components are found
#
# The following are set after configuration is done:
#  GLOG_FOUND
#  GLOG_INCLUDE_DIRS
#  GLOG_LIBRARIES

include(FindPackageHandleStandardArgs)

set(GLOG_ROOT "" CACHE PATH "Root dir where Google glog is installed.")

if(WIN32)
  find_path(GLOG_INCLUDE_DIR glog/logging.h
    HINTS ${GLOG_ROOT}/src/windows)
else()
  find_path(GLOG_INCLUDE_DIR glog/logging.h
    HINTS
    $ENV{GLOG_ROOT}/include
    ${GLOG_ROOT}/include)
endif()

if(MSVC)
  find_library(GLOG_LIBRARY_RELEASE libglog_static
    PATHS ${GLOG_ROOT}
    PATH_SUFFIXES Release)

  find_library(GLOG_LIBRARY_DEBUG libglog_static
    PATHS ${GLOG_ROOT}
    PATH_SUFFIXES Debug)

  set(GLOG_LIBRARY optimized ${GLOG_LIBRARY_RELEASE} debug ${GLOG_LIBRARY_DEBUG})
else()
  find_library(GLOG_LIBRARY glog
    PATHS ${GLOG_ROOT}
    PATH_SUFFIXES
    lib
    lib64)
endif()

find_package_handle_standard_args(GLOG DEFAULT_MSG
  GLOG_INCLUDE_DIR GLOG_LIBRARY)

if(GLOG_FOUND)
  set(GLOG_INCLUDE_DIRS ${GLOG_INCLUDE_DIR})
  set(GLOG_LIBRARIES ${GLOG_LIBRARY})
endif()
