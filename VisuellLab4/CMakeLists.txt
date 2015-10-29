cmake_minimum_required(VERSION 2.8)

#########################################################
# Executable file name
#########################################################
SET( Executable_file Hello_world_GL )

#########################################################
# Project Name
#########################################################
PROJECT(${Executable_file})

#########################################################
# Setting up output folder to /bin
#########################################################
set(DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${DIR} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${DIR} CACHE PATH "Build directory" FORCE)

#########################################################
# Linking all source files
#########################################################
FILE( GLOB SOURCE_FILES src/*.cpp src/*.h)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND GLEW
#########################################################
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})
if(NOT GLEW_FOUND)
    message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)

#########################################################
# Include Files
#########################################################
add_executable( ${Executable_file} ${SOURCE_FILES} )

########################################################
# Linking
#########################################################

target_link_libraries(
    ${Executable_file} 
    ${OPENGL_LIBRARIES} 
    ${GLUT_LIBRARY}
    ${GLEW_LIBRARY}
    )