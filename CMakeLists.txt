cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME ECHO_SERVER)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

####################################################################

set(WARNINGS_AS_ERRORS OFF)

set(ENABLE_UBSan OFF)
set(ENABLE_ASAN OFF)
set(ENABLE_TSan OFF)
set(ENABLE_MSAN OFF)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

include(cmake/CompilerWarnings.cmake)


include_directories( include )
include_directories( src )

set( SRC ${CMAKE_SOURCE_DIR}/src )
set( INC ${CMAKE_SOURCE_DIR}/include )


add_executable( ${PROJECT_NAME} 		  ${SRC}/Main.cpp
				${INC}/Exception.h 		  ${SRC}/Exception.cpp
				${INC}/Server.h 		  ${SRC}/Server.cpp
				${INC}/Base.h
				${INC}/ThreadSafeQueue.h
				${INC}/CrossType.h )

####################################################################

set_target_properties( ${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        )

####################################################################

# Include CMake setup
include(cmake/main-config.cmake)