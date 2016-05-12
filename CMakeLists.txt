cmake_minimum_required(VERSION 3.5.2)
message(${CMAKE_GENERATOR})
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
file(GLOB_RECURSE shaders resources/shaders/ogl/*.vert resources/shaders/ogl/*.frag resources/shaders/ogl/*.geom resources/shaders/ogl/*.tess)

#message(STATUS "file list: ${sources}")

###############################################################################
## target definitions #########################################################
###############################################################################

add_executable(celestial_drift_main ${sources} ${shaders})
SOURCE_GROUP("Shaders" FILES ${shaders})
add_custom_command(TARGET celestial_drift_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${REDISTS}
        $<TARGET_FILE_DIR:celestial_drift_main>)
add_custom_command(TARGET celestial_drift_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        resources
        $<TARGET_FILE_DIR:celestial_drift_main>/resources)	
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

set(VSVER "")
if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013 Win64")
	set(VSVER "_vs13")
elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
	set(VSVER "_vs13")
endif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013 Win64")

set(LIBS2 "glfw3${VSVER}" "glew32" "freetype${VSVER}" "freetype-gl${VSVER}" "fmod" "FreeImage")

foreach(loop_var ${LIBS2})
message (${loop_var})
 set(FOUND_LIB "FOUND_LIB-NOTFOUND")
 find_library (FOUND_LIB 
   ${loop_var}
  PATHS 
	${LIB_SEARCH_PATH}
  NO_DEFAULT_PATH)
 message(${FOUND_LIB})
 TARGET_LINK_LIBRARIES(celestial_drift_main ${FOUND_LIB})
endforeach(loop_var)
 
target_compile_definitions(celestial_drift_main PRIVATE GLEW_STATIC)

TARGET_LINK_LIBRARIES(celestial_drift_main "opengl32") 

exec_program(
    "git"
    ${CMAKE_SOURCE_DIR}
    ARGS " log -n 1 --format=format:\"%h\""
    OUTPUT_VARIABLE GITHASH 
)

exec_program(
    "git"
    ${CMAKE_SOURCE_DIR}
    ARGS " log -n 1 --format=format:\"%ai\""
    OUTPUT_VARIABLE GITDATE
)

target_compile_definitions(celestial_drift_main PUBLIC  GIT_HASH="${GITHASH}")
target_compile_definitions(celestial_drift_main PUBLIC  GIT_DATE="${GITDATE}")

