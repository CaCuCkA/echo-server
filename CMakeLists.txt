cmake_minimum_required(VERSION 3.8)

set( CMAKE_CXX_STANDARD 17 )
set( PROJECT_NAME_ONE "sync_server" )
set ( PROJECT_NAME_TWO "async_server" )

project( ${PROJECT_NAME_ONE} LANGUAGES CXX )

find_package( Boost 1.71.0 COMPONENTS program_options REQUIRED )

if( NOT Boost_FOUND )
    message( FATAL_ERROR "Could not find boost" )
endif()

set( SRC ${CMAKE_SOURCE_DIR}/src )
set( INC ${CMAKE_SOURCE_DIR}/include )


include_directories( include )
include_directories( src )
include_directories( ${Boost_INCLUDE_DIRS} )


add_executable( ${PROJECT_NAME_ONE} ${SRC}/SyncServer.cpp)

add_executable( ${PROJECT_NAME_TWO} ${SRC}/AsyncServer.cpp
        ${INC}/Listener.h       ${SRC}/Listener.cpp
        ${INC}/EchoWebSocket.h  ${SRC}/EchoWebSocket.cpp)

set_target_properties( ${PROJECT_NAME_ONE} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        )

set_target_properties( ${PROJECT_NAME_TWO} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
        )