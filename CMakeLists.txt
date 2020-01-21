cmake_minimum_required (VERSION 3.12)
project (celestial_drift VERSION 1.0 LANGUAGES CXX)




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
target_compile_features(celestial_drift_main PUBLIC cxx_std_17)
set_target_properties(celestial_drift_main PROPERTIES CXX_EXTENSIONS ON)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "celestial_drift_main" )
set_target_properties(celestial_drift_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<CONFIG>)

set_target_properties(celestial_drift_main PROPERTIES 
	VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:celestial_drift_main>"
	VS_DEBUGGER_COMMAND           "$<TARGET_FILE:celestial_drift_main>"
	VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin"
)


SOURCE_GROUP("Shaders" FILES ${shaders})

#add_custom_command(TARGET celestial_drift_main POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${REDISTS}
#        $<TARGET_FILE_DIR:celestial_drift_main>
#)

if(APPLE)
  add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources" 
	${CMAKE_BINARY_DIR}/$<CONFIGURATION>/resources
  )
else()
  add_custom_target(copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources" 
	${CMAKE_BINARY_DIR}/$<CONFIGURATION>/resources
  )
endif()

if(MSVC)
#MultiThreaded build for that speedy compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif(MSVC)


add_subdirectory(deps/glfw EXCLUDE_FROM_ALL)
add_subdirectory(deps/freetype EXCLUDE_FROM_ALL)
set(freetype-gl_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_APIDOC OFF CACHE BOOL ""  FORCE)
add_subdirectory(deps/freetype-gl EXCLUDE_FROM_ALL)
target_include_directories(freetype-gl INTERFACE deps/freetype-gl)
add_subdirectory(deps/glm EXCLUDE_FROM_ALL)
add_subdirectory(deps/glew/ EXCLUDE_FROM_ALL)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/deps/stb)

#target_include_directorie, but marks libs as SYSTEM, so no warnings are shown for them
function(target_link_libraries_system target)
  set(libs ${ARGN})
  foreach(lib ${libs})
    get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(${target} SYSTEM PRIVATE ${lib_include_dirs})
    target_link_libraries(${target} ${lib})
  endforeach(lib)
endfunction(target_link_libraries_system)
target_link_libraries_system (celestial_drift_main glfw glm::glm libglew_static stb freetype freetype-gl)
#target_link_libraries(celestial_drift_main freetype-gl SYSTEM ${lib})


option(celestial_drift_BUILD_AUDIO "Build audio" OFF)
if(celestial_drift_BUILD_AUDIO)
#todo fmod
endif()

# this lets me include files relative to the root src dir with a <> pair
#target_include_directories(celestial_drift_main PUBLIC "include")

#link_directories("${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/" "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/${config}")
#set(LIB_SEARCH_PATH "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/Release/" "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/Debug/" "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}/" )

#set(LIBS2 "glfw3${VSVER}" "glew32" "freetype${VSVER}" "freetype-gl${VSVER}" "fmod" "FreeImage")

#foreach(loop_var ${LIBS2})
#message (${loop_var})
# set(FOUND_LIB "FOUND_LIB-NOTFOUND")
# find_library (FOUND_LIB 
#   ${loop_var}
#  PATHS 
#	${LIB_SEARCH_PATH}
#  NO_DEFAULT_PATH)
# message(${FOUND_LIB})
# TARGET_LINK_LIBRARIES(celestial_drift_main ${FOUND_LIB})
#endforeach(loop_var)
 
target_compile_definitions(celestial_drift_main PRIVATE GLEW_STATIC)

#TARGET_LINK_LIBRARIES(celestial_drift_main "opengl32") 

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

