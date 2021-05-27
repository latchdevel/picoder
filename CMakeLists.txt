# "picoder"
# Simple standalone command line tool to manage OOK protocols 
# supported by "pilight" project, PiCode library based.
#
# Copyright (c) 2021 Jorge Rivera. All right reserved.
# License GNU Lesser General Public License v3.0.

cmake_minimum_required(VERSION 3.1)

project(picoder)

# Set C/C++ Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

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

# Set complier identification
SET(BUILD_COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}" )
MESSAGE( STATUS "Compiler: " ${BUILD_COMPILER} )

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
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -DDEBUG")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
endif()

# Set C/C++ flags
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wconversion -Woverloaded-virtual")

# Add directory for PiCode library 
add_subdirectory( libs/PiCode/ )

# Set source directory for picoder sources
AUX_SOURCE_DIRECTORY( src/ SRC ) 

# Add picoder as executable
add_executable( ${PROJECT_NAME} ${SRC} )

# Add include directory to use #include <PiCode.h>
target_include_directories( ${PROJECT_NAME} PRIVATE libs/PiCode/src/ )

# Add PiCode static library (libpicode.a) to link picoder executable
target_link_libraries( ${PROJECT_NAME} PUBLIC picode )

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