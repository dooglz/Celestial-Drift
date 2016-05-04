cmake_minimum_required(VERSION 3.0)
project(celestial_drift)

SET(OUTPUT_DIRECTORY "bin")

foreach (config ${CMAKE_CONFIGURATION_TYPES})
    string (TOUPPER ${config} config)
    set_target_properties(${_project} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}"
        ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}"
        LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}"
        #PDB_OUTPUT_DIRECTORY_${config} ${sym_path}
        VS_INTERMEDIATE_DIRECTORY_${config} "temp"
    )
	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
	SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
	SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
	SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
	SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
	SET( VS_INTERMEDIATE_DIRECTORY_${config} "temp/${config}")
	
endforeach ()


###############################################################################
## file globbing ##############################################################
###############################################################################

add_subdirectory(src)

###############################################################################
## target definitions #########################################################
###############################################################################
#message(STATUS "file list: ${sources}")
# add the data to the target, so it becomes visible in some IDE
add_executable(celestial_drift_main ${sources})

add_custom_command(TARGET celestial_drift_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/redist/"
        $<TARGET_FILE_DIR:celestial_drift_main>)
		
# just for example add some compiler flags
target_compile_options(celestial_drift_main PUBLIC -std=c++1y)
if(NOT MSVC)
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif(COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif(COMPILER_SUPPORTS_CXX11)

endif(NOT MSVC)

# this lets me include files relative to the root src dir with a <> pair
target_include_directories(celestial_drift_main PUBLIC "include")

find_library (GLFW glfw3 
 PATHS 
 "${CMAKE_SOURCE_DIR}/lib/x64/Release/"
 "${CMAKE_SOURCE_DIR}/lib/x64/Debug/" 
 "${CMAKE_SOURCE_DIR}/lib/x64/" 
 NO_DEFAULT_PATH)
 
 message(${GLFW})
 
TARGET_LINK_LIBRARIES(celestial_drift_main ${GLFW}) 

if (MSVC)
# set the path to the library folder
#link_directories("${CMAKE_SOURCE_DIR}/lib/x64/" "${CMAKE_SOURCE_DIR}/lib/x64/${config}")
# link the libraries to the executable
#target_link_libraries (celestial_drift_main "${CMAKE_SOURCE_DIR}/lib/x64/Release/mpir.lib")
#target_link_libraries (celestial_drift_main "${CMAKE_SOURCE_DIR}/lib/x64/Release/libeay32MD.lib")
#target_link_libraries (celestial_drift_main "${CMAKE_SOURCE_DIR}/lib/x64/Release/ssleay32MD.lib")
endif(MSVC)
