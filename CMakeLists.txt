cmake_minimum_required(VERSION 3.11.0)

project(Socket-Server-Client)

# Build type
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Debug' as none was specified.")
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif ()



# Compiler flags
if (CMAKE_COMPILER_IS_GNUCC)
set (CMAKE_C_FLAGS "-D_REETRANT -Wall -Wextra -pedantic -Wno-long-long")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -O0")
elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG -O3 -fno-strict-aliasing")
endif ()
endif (CMAKE_COMPILER_IS_GNUCC)

####################### extra lib ############################
# Google logging Library
SET(WITH_GFLAGS OFF CACHE BOOL "Use gflags")
SET(BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
IF(WIN32)
  ADD_DEFINITIONS( -D GOOGLE_GLOG_DLL_DECL= )
ENDIF()
add_subdirectory(ext/glog)
SET_PROPERTY(TARGET glog logging_unittest demangle_unittest utilities_unittest stl_logging_unittest PROPERTY FOLDER "ext")
INCLUDE_DIRECTORIES (
  ext/glog/src
  ${CMAKE_BINARY_DIR}/ext/glog
)

##filesystem support, for compatible gcc version below 4
link_libraries(stdc++fs)

# postgresql connect
####################### compiler params ############################
# Annoying compiler-specific details
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null")
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  FIND_PROGRAM(XIAR xiar)
  IF(XIAR)
    SET(CMAKE_AR "${XIAR}")
  ENDIF(XIAR)
  MARK_AS_ADVANCED(XIAR)

  FIND_PROGRAM(XILD xild)
  IF(XILD)
    SET(CMAKE_LINKER "${XILD}")
  ENDIF(XILD)
  MARK_AS_ADVANCED(XILD)

  # ICC will default to -fp-model fast=1, which performs value-unsafe optimizations which will
  # cause pbrt_test to fail. For safety, -fp-model precise is explicitly set here by default.
  set(FP_MODEL "precise" CACHE STRING "The floating point model to compile with.")
  set_property(CACHE FP_MODEL PROPERTY STRINGS "precise" "fast=1" "fast=2")

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fp-model ${FP_MODEL}")
ENDIF()

# A non-exhaustive but pretty representative set..
# Note that we work-around shoddy c++17 support in MSVC2013
# (constexpr, etc.), so don't test for that stuff here
SET (PBRT_CXX17_FEATURES
  cxx_auto_type
  cxx_explicit_conversions
  cxx_lambdas
  cxx_nullptr
  cxx_range_for
  cxx_static_assert
)

INCLUDE_DIRECTORIES(src/)

INCLUDE_DIRECTORIES (
  ext/glog/src
  ext/xmlParser
)

FILE(GLOB COMMON_LIB
  src/common/*.cpp
  src/common/*.h
)

SET(SOCKET_CORE_HEADS
	src/socket/socket.h
)

SET(SOCKET_CORE_SOURCES
	src/socket/socket.cpp
)

FILE(GLOB XML_PARSER
  ext/xmlParser/*.cpp
  ext/xmlParser/*.h
)


SET(SOCKET_SOURCE
	src/server/server.h
  src/server/server.cpp
  src/client/client.h
	src/client/client.cpp
)

ADD_LIBRARY(SOCKET_LIB STATIC
	${SOCKET_CORE_HEADS}
	${SOCKET_CORE_SOURCES}
	${SOCKET_SOURCE}
  ${COMMON_LIB}
)

SET(ALL_SOCKET_LIBS
	SOCKET_LIB
	glog
  ${XML_PARSER}
)

add_executable(server src/server/main.cpp)
TARGET_COMPILE_FEATURES (server PRIVATE ${PBRT_CXX17_FEATURES} )
TARGET_LINK_LIBRARIES (server ${ALL_SOCKET_LIBS})

add_executable(client src/client/main.cpp)
TARGET_COMPILE_FEATURES (client PRIVATE ${PBRT_CXX17_FEATURES} )
TARGET_LINK_LIBRARIES (client ${ALL_SOCKET_LIBS} )


# Unit test
include_directories(tests/)
FILE (GLOB SOCKET_TEST
  tests/*.cpp
  tests/*.h
  tests/gtest/*.cc
  tests/generate_bash/bash.cpp
)
add_executable (socket_test ${SOCKET_TEST} )
TARGET_COMPILE_FEATURES (socket_test PRIVATE ${PBRT_CXX17_FEATURES} )
TARGET_LINK_LIBRARIES (socket_test ${ALL_SOCKET_LIBS} )
