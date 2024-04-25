cmake_minimum_required(VERSION 3.20)

project(OPaL4cIDTracker VERSION 0.0.1)

option(DEBUG "Build with debug messages" on)
option(STATIC "Build static executable" off)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_SOURCE_DIR "src")
set(PROJECT_HEADER_DIR "${PROJECT_SOURCE_DIR}/headers")
set(OPaL4cID_O4S_VERSION 1)

configure_file(${PROJECT_HEADER_DIR}/config.hpp.in config.hpp)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")

if(STATIC)
  set(wxBUILD_SHARED OFF)
  set(wxWidgets_USE_STATIC ON)
  set(BUILD_SHARED_LIBS OFF)
  set(BUILD_STATIC ON)
endif()

if(DEBUG)
  message("Debug build")
endif()

find_package(wxWidgets REQUIRED COMPONENTS gl core base OPTIONAL_COMPONENTS net)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

if(STATIC)
  set(LIBS "-static" ${wxWidgets_LIBRARIES})
else()
  set(LIBS ${wxWidgets_LIBRARIES})
endif()

if(DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
endif()

add_executable("OPaL4cID_Tracker" ${SOURCES})

target_include_directories("OPaL4cID_Tracker" PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries("OPaL4cID_Tracker" PUBLIC ${LIBS})
