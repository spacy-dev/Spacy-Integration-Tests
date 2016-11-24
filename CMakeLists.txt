project(Spacy)

cmake_minimum_required(VERSION 3.1)

# Set path to cmake scripts
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake
    )

# Do not allow in-source builds
include(ConfigSafeGuards)

option(Coverage "Coverage" OFF)
option(Kaskade "Kaskade" OFF)
option(Dolfin "Dolfin" OFF)
option(Eigen "Eigen" OFF)

if(Coverage)
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -coverage")
endif()

find_package(Spacy CONFIG REQUIRED)
include_directories(${Spacy_INCLUDE_DIRS})
set(LIBS "${Spacy_LIBRARIES}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Spacy_CXX_FLAGS}")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(LIBS "${LIBS} ${GTEST_LIBRARIES} pthread")

if(Dolfin)
  # Tell cmake where to find DOLFINConfig.cmake
  find_package(DOLFIN CONFIG REQUIRED)
  include_directories(${DOLFIN_INCLUDE_DIRS})
  include_directories(SYSTEM ${DOLFIN_3RD_PARTY_INCLUDE_DIRS})
endif()

if(Eigen)
  find_package(Eigen3 REQUIRED)
  include_directories(${EIGEN3_INCLUDE_DIR})
endif()

aux_source_directory(Eigen SRC_LIST)

include(CTest)
enable_testing()
foreach(TEST ${SRC_LIST})
  get_filename_component(TEST_NAME ${TEST} NAME_WE)
  get_filename_component(TEST_DIR ${TEST} DIRECTORY)
  string(REGEX REPLACE "/" "_" TEST_DIR ${TEST_DIR})
  set(TEST_UNIQUE_NAME "${TEST_DIR}_${TEST_NAME}")
  add_executable(${TEST_UNIQUE_NAME} ${TEST})
  target_link_libraries(${TEST_UNIQUE_NAME} ${LIBS})
  if(Kaskade)
    target_link_libraries(${TEST_UNIQUE_NAME} kaskade boost_system boost_timer boost_thread dunegrid dunecommon dunegeometry ugS3 ugS2 ugL3 ugS2 devS devX umfpack amd acml)
  endif()
  add_test(${TEST_UNIQUE_NAME} ${PROJECT_BINARY_DIR}/${TEST_UNIQUE_NAME})
endforeach()

