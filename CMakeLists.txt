cmake_minimum_required(VERSION 3.0)
#set cmake environment:
set(CMAKE_USE_RELATIVE_PATHS ON)

#build the project:
project (Test)

#set working directory:
set(MY_WORKING_DIR TestCI)

#output message:
message(STATUS "Welcome to Build Project ${CMAKE_PROJECT_NAME}.")

set (Test_VERSION_MAJOR 0)
set (Test_VERSION_MINOR 10)
set (Test_VERSION_PATCH 0)

message (STATUS "Configuring Test v${Test_VERSION_MAJOR}.${Test_VERSION_MINOR}.${Test_VERSION_PATCH}")

message(STATUS "The OS is ${CMAKE_SYSTEM_NAME}, version ${CMAKE_SYSTEM_VERSION}.")
message(STATUS "The working directory is ${CMAKE_BINARY_DIR}/${MY_WORKING_DIR}.")
message(STATUS "--- Start to call ${CMAKE_CURRENT_LIST_FILE} ---") 
message(STATUS "This current binary directory is ${CMAKE_CURRENT_BINARY_DIR}.")
message(STATUS "This current source directory is ${CMAKE_CURRENT_SOURCE_DIR}.")

message(STATUS "The C compiler is ${CMAKE_C_COMPILER_ID}")
message(STATUS "The CXX compiler is ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "The CMAKE_GENERATOR is ${CMAKE_GENERATOR}")
message(STATUS "The CMAKE_CXX_COMPILER_VERSION is ${CMAKE_CXX_COMPILER_VERSION}")

message(STATUS "The CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")
 
if(WIN32)
   #add_compile_options(/std:c++11)
else(WIN32)
   add_compile_options(-std=gnu++11)
   #add_compile_options(-std=c++11)
endif(WIN32)

#look for CMakeLists.txt in subdirectory <codes> and build an executable target:
add_subdirectory(codes ${MY_WORKING_DIR})

#exit:
message(STATUS "--- Exit ${CMAKE_CURRENT_LIST_FILE}! ---")
message(STATUS "Finished build project ${PROJECT_NAME}!")
