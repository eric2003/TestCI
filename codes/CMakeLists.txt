﻿#. output message:
message(STATUS "") 
message(STATUS "--- Enter subdirectory, Start to call ${CMAKE_CURRENT_LIST_FILE} ---") 
message(STATUS "This current binary directory is ${CMAKE_CURRENT_BINARY_DIR}.")
message(STATUS "This current source directory is ${CMAKE_CURRENT_SOURCE_DIR}.")

message (STATUS "Installation directory: ${CMAKE_INSTALL_PREFIX}")
message (STATUS "Include path: ${CMAKE_INCLUDE_PATH}")
message (STATUS "Library path: ${CMAKE_LIBRARY_PATH}")
message (STATUS "Prefix path: ${CMAKE_PREFIX_PATH}")
message (STATUS "Module path: ${CMAKE_MODULE_PATH}")
message (STATUS "Build type is: ${CMAKE_BUILD_TYPE}")
message (STATUS "System name: ${CMAKE_SYSTEM_NAME}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if (CMAKE_GENERATOR MATCHES "Visual Studio 16 2019" )
        set(VS_COMPILER vs2019)
    elseif (CMAKE_GENERATOR MATCHES "Visual Studio 15 2017" )
        set(VS_COMPILER vs2017)
    elseif (CMAKE_GENERATOR MATCHES "Visual Studio 14 2015" )
        set(VS_COMPILER vs2015)
    elseif (CMAKE_GENERATOR MATCHES "Visual Studio 12 2013" )
        set(VS_COMPILER vs2013)
    elseif (CMAKE_GENERATOR MATCHES "Visual Studio 11 2012" )
        set(VS_COMPILER vs2012)
    elseif (CMAKE_GENERATOR MATCHES "Visual Studio 10 2010" )
        set(VS_COMPILER vs2010)
    else()
        set(VS_COMPILER vs2019)
    endif(CMAKE_GENERATOR MATCHES "Visual Studio 16 2019" )
	message(STATUS "The  current VS_COMPILER is ${VS_COMPILER}")
endif()

# add compiler specific flags
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(STATUS "Changing stdlib for Clang")
  set(COMPILER_SPEC_FLAGS "-stdlib=libc++")
endif()

#message (STATUS "COMPILER_SPEC_FLAGS: " ${COMPILER_SPEC_FLAGS})

# set additional c++ flags for all build types
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
   set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_SPEC_FLAGS}")
   # release build type
   set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMPILER_SPEC_FLAGS} /O2 /DNDEBUG")
   # debug build type
   set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMPILER_SPEC_FLAGS} /Wall /Od /PROFILE")
   # create profile build type
   set (CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} ${COMPILER_SPEC_FLAGS} /PROFILE /DNDEBUG")
else()
   set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_SPEC_FLAGS} -Wall -pedantic")
   # release build type
   set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMPILER_SPEC_FLAGS} -Wall -pedantic -march=native -DNDEBUG")
   # debug build type
   set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMPILER_SPEC_FLAGS} -Wall -pedantic -O0 -ggdb -pg --coverage")
   # create profile build type
   set (CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} ${COMPILER_SPEC_FLAGS} -pg -DNDEBUG")
endif()

# set compiler flags based on release type
if (CMAKE_BUILD_TYPE MATCHES "release")
   message (STATUS "C++ flags: " ${CMAKE_CXX_FLAGS_RELEASE})
elseif (CMAKE_BUILD_TYPE MATCHES "debug")
   message (STATUS "C++ flags: " ${CMAKE_CXX_FLAGS_DEBUG})
elseif (CMAKE_BUILD_TYPE MATCHES "profile")
   message (STATUS "C++ flags: " ${CMAKE_CXX_FLAGS_PROFILE})
elseif (CMAKE_BUILD_TYPE MATCHES "relwithdebinfo")
   message (STATUS "C++ flags: " ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
elseif (CMAKE_BUILD_TYPE MATCHES "minsizerel")
   message (STATUS "C++ flags: " ${CMAKE_CXX_FLAGS_MINSIZEREL})
else ()
   message (STATUS "C++ flags: " ${CMAKE_CXX_FLAGS})
endif ()

message(STATUS "The CMAKE_C_COMPILER is ${CMAKE_C_COMPILER}")
message(STATUS "The CMAKE_CXX_COMPILER is ${CMAKE_CXX_COMPILER}")
message(STATUS "The CMAKE_C_FLAGS is ${CMAKE_C_FLAGS}")
message(STATUS "The CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")

file(GLOB headerFile "*.h" )
file(GLOB srcFile "*.cpp" )
file(GLOB csrcFile "*.c" )
file(GLOB hppFile "*.hpp" )
message( STATUS ${srcFile} )
message( STATUS ${headerFile} )
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set( METIS_INCLUDE "C:/Program Files (x86)/METIS/include/")
    set( METIS_LIB_DIR "C:/Program Files (x86)/METIS/lib/")
    set( METIS_LIB "metis")
else()
    set( METIS_INCLUDE "usr/local/include/")
    set( METIS_LIB_DIR "usr/local/lib/")
    set( METIS_LIB "metis")
endif()

message( STATUS "The METIS_INCLUDE is ${METIS_INCLUDE}." )
include_directories("${METIS_INCLUDE}")

source_group("include" FILES ${headerFile})
source_group("src" FILES ${srcFile})
source_group("cfile" FILES ${csrcFile})
list(APPEND HEAD_LIST ${headerFile})
list(APPEND SRC_LIST ${srcFile} ${csrcFile} )

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    link_directories("${PROJECT_SOURCE_DIR}/lib/windows/" )
    link_directories("${MPI_DIR}")
    link_directories("${METIS_LIB_DIR}")
else()
    link_directories("${MPI_DIR}")
    link_directories("${METIS_LIB_DIR}")
endif()

message( STATUS "The METIS_INCLUDE is ${METIS_INCLUDE}." )
message( STATUS "The METIS_LIB_DIR is ${METIS_LIB_DIR}." )

# determine if MPI_DIR has been set
if (MPI_DIR)
   message (STATUS "Looking for MPI here: ${MPI_DIR}")
   set (CMAKE_PREFIX_PATH ${MPI_DIR})
endif ()

find_package (MPI REQUIRED)
include_directories (SYSTEM "${MPI_INCLUDE_PATH}")

message( STATUS "MPI_DIR is ${MPI_DIR}" )
message( STATUS "MPI_INCLUDE_PATH is ${MPI_INCLUDE_PATH}" )
message( STATUS "MPI_C_LIBRARIES is ${MPI_C_LIBRARIES}" )



#. set executable name:
set(EXECUTABLE_TARGET_NAME "${CMAKE_PROJECT_NAME}")
message(STATUS "The executable name is ${EXECUTABLE_TARGET_NAME}.")

#. add an executable to the project using the specified source files and header files:
add_executable(${EXECUTABLE_TARGET_NAME} ${SRC_LIST} ${HEAD_LIST}) 

target_link_libraries (${EXECUTABLE_TARGET_NAME} ${MPI_C_LIBRARIES} )
target_link_libraries (${EXECUTABLE_TARGET_NAME} ${METIS_LIB} )


install (TARGETS ${EXECUTABLE_TARGET_NAME} RUNTIME DESTINATION bin)
    
#3. exit:          
message(STATUS "--- Exit ${CMAKE_CURRENT_LIST_FILE}! ---")
 