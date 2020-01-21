cmake_minimum_required (VERSION 3.12)
project (celestial_drift VERSION 1.0 LANGUAGES CXX)
cmake_policy(SET CMP0079 NEW)

find_package(OpenGL REQUIRED)
message(STATUS "openGL @ ${OPENGL_gl_LIBRARY}, ${OPENGL_INCLUDE_DIR}")


file(GLOB_RECURSE sources src/*.cpp src/*.h src/*.cl src/*.cu src/*.vert src/*.frag src/*.geom src/*.tess)
file(GLOB_RECURSE shaders resources/shaders/ogl/*.vert resources/shaders/ogl/*.frag resources/shaders/ogl/*.geom resources/shaders/ogl/*.tess)

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

add_dependencies(celestial_drift_main copy_resources)


if(MSVC)
#MultiThreaded build for that speedy compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif(MSVC)

add_subdirectory(deps/glm EXCLUDE_FROM_ALL)

add_subdirectory(deps/glew/ EXCLUDE_FROM_ALL)
target_include_directories(libglew_static PRIVATE ${OPENGL_INCLUDE_DIR})
target_compile_definitions(libglew_static INTERFACE GLEW_NO_GLU)
target_compile_definitions(libglew_static INTERFACE GLEW_STATIC)

add_subdirectory(deps/glfw EXCLUDE_FROM_ALL)

add_subdirectory(deps/freetype EXCLUDE_FROM_ALL)

set(freetype-gl_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_APIDOC OFF CACHE BOOL ""  FORCE)

#freetype-gl needs some serious cmake work, the following is a workaround.
set(GLEW_LIBRARY libglew_static)
get_target_property(GLEW_INCLUDE_PATH libglew_static INCLUDE_DIRECTORIES)
set(FREETYPE_LIBRARY freetype)
get_target_property(FREETYPE_INCLUDE_DIRS freetype INCLUDE_DIRECTORIES)
add_subdirectory(deps/freetype-gl EXCLUDE_FROM_ALL)
target_include_directories(freetype-gl INTERFACE deps/freetype-gl)
target_compile_definitions(freetype-gl PUBLIC GLEW_NO_GLU)
target_compile_definitions(freetype-gl PUBLIC GLEW_STATIC)
add_dependencies(freetype-gl freetype OpenGL::GL libglew_static)
target_link_libraries(freetype-gl freetype libglew_static)

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
target_link_libraries_system (celestial_drift_main OpenGL::GL glfw glm::glm libglew_static stb freetype freetype-gl)


option(celestial_drift_BUILD_AUDIO "Build audio" OFF)
if(celestial_drift_BUILD_AUDIO)
#todo fmod
endif()



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
target_compile_definitions(celestial_drift_main PUBLIC _PLATFORM_X64)

#Install directory within build folder, so it doesn't go into /lib or program-files
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/")
SET(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/installer/")
INCLUDE(InstallRequiredSystemLibraries)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/resources" DESTINATION "bin/")
INSTALL(TARGETS celestial_drift_main)

#Tell CPack about the libs and resources install components
set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "celestial_drift_main")
set(CPACK_PACKAGE_VENDOR "Dooglz")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")

INCLUDE(CPack)


