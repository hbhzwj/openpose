# - Try to find FREENECT2
#
# The following variables are optionally searched for defaults
#  FREENECT2_ROOT_DIR:            Base directory where all FREENECT2 components are found
#
# The following are set after configuration is done:
#  FREENECT2_FOUND
#  FREENECT2_INCLUDE_DIRS
#  FREENECT2_LIBRARIES
#  FREENECT2_LIBRARYRARY_DIRS

include(FindPackageHandleStandardArgs)

set(FREENECT2_ROOT_DIR "" CACHE PATH "Folder contains freenect2")

# We are testing only a couple of files in the include directories
if(WIN32)
    find_path(FREENECT2_INCLUDE_DIR libfreenect2/libfreenect2.hpp
        PATHS ${FREENECT2_ROOT_DIR}/src/windows)
else()
    find_path(FREENECT2_INCLUDE_DIR libfreenect2.hpp
        PATHS ${FREENECT2_ROOT_DIR})
endif()

if(MSVC)
    find_library(FREENECT2_LIBRARY_RELEASE
        NAMES freenect2
        PATHS ${FREENECT2_ROOT_DIR}
        PATH_SUFFIXES Release)

    find_library(FREENECT2_LIBRARY_DEBUG
        NAMES freenect2-debug
        PATHS ${FREENECT2_ROOT_DIR}
        PATH_SUFFIXES Debug)

    set(FREENECT2_LIBRARY optimized ${FREENECT2_LIBRARY_RELEASE} debug ${FREENECT2_LIBRARY_DEBUG})
else()
  find_library(FREENECT2_LIBRARY freenect2)
endif()

find_package_handle_standard_args(Freenect2 DEFAULT_MSG FREENECT2_INCLUDE_DIR FREENECT2_LIBRARY)


if(FREENECT2_FOUND)
    set(FREENECT2_INCLUDE_DIRS ${FREENECT2_INCLUDE_DIR})
    set(FREENECT2_LIBRARIES ${FREENECT2_LIBRARY})
    message(STATUS "Found freenect2 (include: ${FREENECT2_INCLUDE_DIR}, library: ${FREENECT2_LIBRARY})")
    mark_as_advanced(FREENECT2_LIBRARY_DEBUG FREENECT2_LIBRARY_RELEASE
                     FREENECT2_LIBRARY FREENECT2_INCLUDE_DIR FREENECT2_ROOT_DIR)
endif()
