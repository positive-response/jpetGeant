cmake_minimum_required (VERSION 2.6)
project(jpet_mc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


# add geant4 with vis drivers
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

find_package(ROOT REQUIRED)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})

add_executable(jpet_mc ${sources} ${headers})
target_link_libraries(jpet_mc ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
