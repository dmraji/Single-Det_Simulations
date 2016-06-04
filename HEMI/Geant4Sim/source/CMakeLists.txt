cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# setting a variable equal to the executable name
set(name CodedAperture)

# name the project
project(CodedAperture)

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

#------------------------------------------------------------------------------#
#   By default, use the generic physics list
option(CUSTOM_PHYSICS_LIST "If enabled, physics list is PhysicsList.
                            If disabled, physics list is G4GenericPhysicsList" OFF)
if(NOT CUSTOM_PHYSICS_LIST)
    set(_physics_file PhysicsList)
    list(REMOVE_ITEM sources ${PROJECT_SOURCE_DIR}/src/${_physics_file}.cc)
    list(REMOVE_ITEM headers ${PROJECT_SOURCE_DIR}/include/${_physics_file}.hh)
else()
    add_definitions(-DCUSTOM_PHYSICS_LIST)
endif()
#------------------------------------------------------------------------------#

# create the executable
add_executable(${name} ${name}.cc ${sources} ${headers})

# link the Geant4 libraries to the executable
target_link_libraries(${name} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# copy the macro files to the build directory
#set(macros run.mac vis.mac)
#foreach(_macro ${macros})
#    configure_file(${PROJECT_SOURCE_DIR}/${_macro}
#                   ${PROJECT_BINARY_DIR}/${_macro}
#                   COPY_ONLY
#    )
#endforeach()

# copy over geometry and macro folders
file(GLOB macrofiles RELATIVE ${PROJECT_SOURCE_DIR}/macros ${PROJECT_SOURCE_DIR}/macros/*.mac)
foreach(macrofile ${macrofiles})
	set (macropath ${PROJECT_SOURCE_DIR}/macros/${macrofile})
	configure_file(${macropath}
                    ${PROJECT_BINARY_DIR}/macros/${macrofile}
                    @ONLY)
endforeach()
