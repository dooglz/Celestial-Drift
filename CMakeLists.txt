cmake_minimum_required(VERSION 3.3)

project(celestial_drift)

SET(OUTPUT_DIRECTORY "bin/")
if(MSVC)
	if(${CMAKE_CL_64})
		SET(PLATFORM "x64")
		SET(REDISTS "redist/windows" "redist/x64")
	else(${CMAKE_CL_64})
		SET(PLATFORM "win32")
		SET(REDISTS "redist/windows" "redist/win32")
	endif(${CMAKE_CL_64})
else(MSVC)
	SET(PLATFORM "other")
	SET(REDISTS "redist/other")
endif(MSVC)

 message("platform is " ${PLATFORM})
 
foreach (config ${CMAKE_CONFIGURATION_TYPES})
    string (TOUPPER ${config} config)
    set_target_properties(${_project} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${PLATFORM}/${config}"
        ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${PLATFORM}/${config}"
        LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${PLATFORM}/${config}"
        #PDB_OUTPUT_DIRECTORY_${config} ${sym_path}
        VS_INTERMEDIATE_DIRECTORY_${config} "temp"
    )
	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${PLATFORM}/${config}")
	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${PLATFORM}/${config}")
	SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${PLATFORM}/${config}")
	SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${PLATFORM}/${config}")
	SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${PLATFORM}/${config}")
	SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${PLATFORM}/${config}")
	SET( VS_INTERMEDIATE_DIRECTORY_${config} "temp/${config}")
	
endforeach ()


###############################################################################
## file globbing ##############################################################
###############################################################################

file(GLOB_RECURSE sources src/*.cpp src/*.h src/*.cl src/*.cu src/*.vert src/*.frag src/*.geom src/*.tess)
#message(STATUS "file list: ${sources}")

###############################################################################
## target definitions #########################################################
###############################################################################

add_executable(celestial_drift_main ${sources})

add_custom_command(TARGET celestial_drift_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${REDISTS}
        $<TARGET_FILE_DIR:celestial_drift_main>)
		
if(MSVC)
#MultiThreaded build for that speedy compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
else(MSVC)
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
endif(MSVC)

# this lets me include files relative to the root src dir with a <> pair
target_include_directories(celestial_drift_main PUBLIC "include")

link_directories("${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/" "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/${config}")
set(LIB_SEARCH_PATH "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/Release/" "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/Debug/" "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/" )

find_library (GLFW 
  glfw3
 PATHS 
	${LIB_SEARCH_PATH}
 NO_DEFAULT_PATH)
 message(${GLFW})
 
 find_library (GLEW 
   glew32
 PATHS 
	${LIB_SEARCH_PATH}
 NO_DEFAULT_PATH)
 
 find_library (FRTYP 
   freetype
 PATHS 
	${LIB_SEARCH_PATH}
 NO_DEFAULT_PATH)
 
 find_library (FRYTPGL 
   freetype-gl
 PATHS 
	${LIB_SEARCH_PATH}
 NO_DEFAULT_PATH)
 
  find_library (FMOD 
   fmod
 PATHS 
	${LIB_SEARCH_PATH}
 NO_DEFAULT_PATH)
 
 target_compile_definitions(celestial_drift_main PRIVATE GLEW_STATIC)
 
TARGET_LINK_LIBRARIES(celestial_drift_main ${GLFW} ${GLEW} ${FRTYP} ${FRYTPGL} ${FMOD} "opengl32") 



