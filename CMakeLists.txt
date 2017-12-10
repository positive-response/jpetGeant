cmake_minimum_required (VERSION 2.6)
project(jpet_mc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

set(CMAKE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/lib)


#######################################################
# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/doc)
endif()
######################################################



# add geant4 with vis drivers
find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

find_package(ROOT REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})


# Include CADMesh in your project.                
find_package(cadmesh REQUIRED)

include_directories(${CADMESH_INCLUDE_DIRS})

                #                REFLEX_GENERATE_DICTIONARY(JPetMCClasses include/Event.hh)
                #                add_library(JPetMCClassesDict SHARED ${sources} JPetMCClasses.cxx)
                #                set(libsuffix .so)
                #                set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES} SUFFIX ${libsuffix})
                #                set_target_properties(JPetMCClassesDict PROPERTIES ${ROOT_LIBRARY_PROPERTIES})
                #                target_link_libraries(JPetMCClassesDict  ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
                #
                ##include(${ROOT_USE_FILE}) -- below workaround 
                #list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/cmake)
                ##
                ##
                ROOT_GENERATE_DICTIONARY(JPetMCClasses   include/TrkBlock.hh include/ScinBlock.hh)
#---Create a shared library with geneated dictionary
add_library(JPetMCClassesDict SHARED  src/TrkBlock.cc src/ScinBlock.cc JPetMCClasses.cxx )
target_link_libraries(JPetMCClassesDict ${ROOT_LIBRARIES})


#---Create  a main program using the library
add_executable(jpet_mc ${sources} ${headers})
target_link_libraries(jpet_mc ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} JPetMCClassesDict cadmesh)
