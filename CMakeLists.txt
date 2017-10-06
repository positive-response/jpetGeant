cmake_minimum_required (VERSION 2.6)
project(jpet_mc)

SET (CMAKE_C_COMPILER             "/usr/bin/clang")
SET (CMAKE_C_FLAGS                "-Wall -std=c++11")
SET (CMAKE_C_FLAGS_DEBUG          "-g")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS                "-Wall -std=c++11")
SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

set(CMAKE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/lib)

# add geant4 with vis drivers
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

find_package(ROOT REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})

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
ROOT_GENERATE_DICTIONARY(JPetMCClasses   include/Event.hh include/VtxBlock.hh include/ParticleBlock.hh)
#---Create a shared library with geneated dictionary
add_library(JPetMCClassesDict SHARED src/Event.cc src/VtxBlock.cc src/ParticleBlock.cc JPetMCClasses.cxx )
target_link_libraries(JPetMCClassesDict ${ROOT_LIBRARIES})


#---Create  a main program using the library
add_executable(jpet_mc ${sources} ${headers})
target_link_libraries(jpet_mc ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} JPetMCClassesDict)
