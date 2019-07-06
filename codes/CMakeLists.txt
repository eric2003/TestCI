#. output message:
message(STATUS "") 
message(STATUS "--- Enter subdirectory, Start to call ${CMAKE_CURRENT_LIST_FILE} ---") 
message(STATUS "This current binary directory is ${CMAKE_CURRENT_BINARY_DIR}.")
message(STATUS "This current source directory is ${CMAKE_CURRENT_SOURCE_DIR}.")

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

if(WIN32)
message(STATUS "VS_COMPILER is ${VS_COMPILER}")
endif(WIN32)

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

source_group("include" FILES ${headerFile})
source_group("src" FILES ${srcFile})
source_group("cfile" FILES ${csrcFile})
list(APPEND HEAD_LIST ${headerFile})
list(APPEND SRC_LIST ${srcFile} ${csrcFile} )

#. set executable name:
set(EXECUTABLE_TARGET_NAME "${CMAKE_PROJECT_NAME}")
message(STATUS "The executable name is ${EXECUTABLE_TARGET_NAME}.")

if(WIN32)
    link_directories("${PROJECT_SOURCE_DIR}/lib/windows/")
else(WIN32)
endif(WIN32)

#. add an executable to the project using the specified source files and header files:
add_executable(${EXECUTABLE_TARGET_NAME} ${SRC_LIST} ${HEAD_LIST}) 

                  
#3. exit:          
message(STATUS "--- Exit ${CMAKE_CURRENT_LIST_FILE}! ---")
 