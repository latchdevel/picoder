# "picoder"
# Simple standalone command line tool to manage OOK protocols 
# supported by "pilight" project, PiCode library based.
#
# Copyright (c) 2021 Jorge Rivera. All right reserved.
# License GNU Lesser General Public License v3.0.

cmake_minimum_required(VERSION 3.14) # Update to prevent policy CMP0080 warning

project(picoder C) # Pure C version

# Set C compiler standard
set(CMAKE_C_STANDARD 11)    # Needed by glibc getopt in MSVC

# Check for git repository information
IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
  FIND_PACKAGE(Git)
  IF(GIT_FOUND)
    EXECUTE_PROCESS(
      COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      OUTPUT_VARIABLE "BUILD_VERSION"
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    SET(BUILD_VERSION "${BUILD_VERSION}" )
    MESSAGE( STATUS "Git commit: ${BUILD_VERSION}")
  ENDIF(GIT_FOUND)
ENDIF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)

# Force re-run CMake to set BUILD_VERSION on any git commit
set_property(
            DIRECTORY
            APPEND
            PROPERTY CMAKE_CONFIGURE_DEPENDS
            .git/index
)

# Set C complier identification
SET(BUILD_COMPILER "${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}" )
MESSAGE( STATUS "C Compiler: " ${BUILD_COMPILER} )

# Check if set CMAKE_BUILD_TYPE var
if(NOT CMAKE_BUILD_TYPE)
  # Set default build type to "release" set -O3 -DNDEBUG
  set(DEFAULT_BUILD_TYPE "release")
  SET(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE})
  MESSAGE( STATUS "Build type set to default: " ${CMAKE_BUILD_TYPE} )
else()
  # Check if set and valid CMAKE_BUILD_TYPE var
  STRING( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE )
  if((CMAKE_BUILD_TYPE STREQUAL "debug") OR (CMAKE_BUILD_TYPE STREQUAL "release"))
    # If no has parent directory show message
    if(NOT hasParent)
      MESSAGE( STATUS "Build type set to: " ${CMAKE_BUILD_TYPE} )
    endif()
  else()
    MESSAGE( FATAL_ERROR "If set CMAKE_BUILD_TYPE it must be 'release' or 'debug'")
  endif()
endif()

# Setting build type to "debug" add only -g 
if(CMAKE_BUILD_TYPE STREQUAL "debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG")
endif()

# Set C compiler flags
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter -fcommon") # Add -fcommon CFLAG to prevent multiple-definition
elseif(MSVC)
  set(MSVC_DISABLED_WARNINGS_LIST
    "C4996" # warning C4996: 'may be unsafe/disable deprecation'
            # To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
    "C4244" # warning C4244: '=': conversion from '__int64' to 'int', possible loss of data
    "C4267" # warning C4267: '=': conversion from 'size_t' to 'int', possible loss of data
  # "C4305" # warning C4305: '=': truncation from 'int' to 'uint16_t'
    "C5105" # warning C5105: macro expansion producing 'defined' has undefined behavior
    "C4201" # warning C4201: nonstandard extension used: nameless struct/union
    "C4473" # warning C4473: 'printf' : not enough arguments passed for format string
    "C4100" # warning C4100: unreferenced formal parameter
    "C4706" # warning C4706: assignment within conditional expression
  )
  string(REPLACE "C" " -wd" MSVC_DISABLED_WARNINGS_STR ${MSVC_DISABLED_WARNINGS_LIST})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W4 ${MSVC_DISABLED_WARNINGS_STR}")
endif()

# If macOS builds a Mach-O universal binary with 2 architectures: x86_64 and arm64 for Apple M processors
if (APPLE) 
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

# Add directory for cPiCode library only
add_subdirectory( libs/PiCode/ EXCLUDE_FROM_ALL )

# Set source directory for picoder sources
AUX_SOURCE_DIRECTORY( src/ SRC ) 

# getopt is not part of ANSI C, is POSIX, not Windows
if(MSVC)
  # Sorry for the static builds of getopt violate the Lesser GNU Public License
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSTATIC_GETOPT -Dstrcasecmp=_stricmp")

  # Add sources for getopt() and getsubopt() friendly derivative for Microsoft Visual C
  AUX_SOURCE_DIRECTORY( libs/glibc/stdlib/ STDLIB ) 

  # Add picoder as executable static link
  add_executable( ${PROJECT_NAME} ${SRC} ${STDLIB} )

  # Add include directory to use #include <getopt.h> and <getsubopt.h>
  target_include_directories( ${PROJECT_NAME} PRIVATE libs/glibc/stdlib/ )

else()
  # Add picoder as executable
  add_executable( ${PROJECT_NAME} ${SRC} )
endif()

# Add executable export symbols for loadable modules to prevent policy CMP0065 warning
set_target_properties( ${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS TRUE )

set_target_properties( ${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE ) # Add -fPIC

set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_SEARCH_START_STATIC TRUE ) # Add "-Bstatic"

# Build static executable if possible
if( NOT (CMAKE_SYSTEM_NAME MATCHES "Darwin" OR MSVC) )
  set( CMAKE_EXE_LINKER_FLAGS "-static" )
endif()

# Checking for math library 'libm' used when including <math.h> in pilight sources
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.lib;.tbd;.so;.dylib;.dll.a")
find_library(MATH_LIBRARY NAMES m )
if(MATH_LIBRARY)
  MESSAGE( STATUS "Math library for ${PROJECT_NAME}: " ${MATH_LIBRARY} )
else()
  if(NOT MSVC)
    message(FATAL_ERROR "Cannot find math library 'libm' ")
  else()
    set(MATH_LIBRARY "")
  endif()
endif()

# Add include directory to use #include <cPiCode.h>
target_include_directories( ${PROJECT_NAME} PRIVATE libs/PiCode/src/ )

# Add cPiCode and Math static libraries to link picoder executable
target_link_libraries( ${PROJECT_NAME} PRIVATE cpicode ${MATH_LIBRARY})

# If git info available adds to picoder executable as environment var
if(DEFINED BUILD_VERSION)
    target_compile_definitions( ${PROJECT_NAME} PRIVATE BUILD_VERSION=${BUILD_VERSION} )
endif(DEFINED BUILD_VERSION)

# Add complier identification to picoder executable as environment var
target_compile_definitions( ${PROJECT_NAME} PRIVATE BUILD_COMPILER=${BUILD_COMPILER} )

MESSAGE(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
MESSAGE(STATUS "Install prefix:   ${CMAKE_INSTALL_PREFIX}")

install(TARGETS picoder DESTINATION bin)

add_custom_target( uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/uninstall.cmake"
)