cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# setting a variable equal to the executable name
set(name HEMI)

# name the project
project(HEMI)

# find Geant4 (uses Geant4_DIR)
find_package(Geant4 REQUIRED ui_all vis_all)

# --------------------------------------------------------
# Add in ROOT libraries 
# --------------------------------------------------------
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})
list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)

find_package(ROOT)
if(NOT ROOT_FOUND)
  message(STATUS "ROOT package not found. --> example disabled")  
  return()
endif()

# --------------------------------------------------------

# sets the Geant4_INCLUDE_DIR and Geant4_LIBRARIES
include(${Geant4_USE_FILE})

# include your headers and the Geant4 headers
include_directories(${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR} ${ROOT_INCLUDE_DIR})

# get the implementation files
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# get the header files
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# create the executable
add_executable(${name} ${name}.cc ${sources} ${headers})

# link the Geant4 libraries to the executable
target_link_libraries(${name} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
