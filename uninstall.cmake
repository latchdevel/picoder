# uninstall.cmake 
# Add to CMakeList.txt like as: 
#   add_custom_target( uninstall 
#       "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/uninstall.cmake" 
#   )
# Copyright (c) 2021 Jorge Rivera. All right reserved.
# License GNU Lesser General Public License v3.0.

set(MANIFEST "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")

if(NOT EXISTS ${MANIFEST})
    message(FATAL_ERROR "Cannot find install manifest: '${MANIFEST}'")
endif()

file(STRINGS ${MANIFEST} files)
foreach(file ${files})
    if(EXISTS ${file})
        message(STATUS "Removing file: '${file}'")

        exec_program(
            ${CMAKE_COMMAND} ARGS "-E remove ${file}"
            OUTPUT_VARIABLE stdout
            RETURN_VALUE result
        )
        
        if(NOT "${result}" STREQUAL 0)
            message(FATAL_ERROR "Failed to remove file: '${file}'.")
        endif()
    elseif(IS_SYMLINK ${file})
        message(STATUS "Removing symbolic link: '${file}'")

        exec_program(
            ${CMAKE_COMMAND} ARGS "-E remove ${file}"
            OUTPUT_VARIABLE stdout
            RETURN_VALUE result
        )
        
        if(NOT "${result}" STREQUAL 0)
            message(FATAL_ERROR "Failed to remove symbolic link: '${file}'.")
        endif()
    else()
        MESSAGE(STATUS "File '${file}' does not exist.")
    endif()
endforeach(file)