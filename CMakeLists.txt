cmake_minimum_required(VERSION 3.8)

set( CMAKE_CXX_STANDARD 17 )
set( BOOST_SYNC_SERVER "boost_sync_server" )
set( BOOST_ASYNC_SERVER "boost_async_server" )
set( WEBSOCKET_SYNC_SERVER "websocket_sync_server" )
set( WEBSOCKET_MULTI_SERVER "websocket_multi_server" )

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


add_executable( ${BOOST_SYNC_SERVER} ${SRC}/BoostSyncServer.cpp)

add_executable( ${BOOST_ASYNC_SERVER} ${SRC}/BoostAsyncServer.cpp
        ${INC}/Listener.h       ${SRC}/Listener.cpp
        ${INC}/EchoWebSocket.h  ${SRC}/EchoWebSocket.cpp)

add_executable( ${WEBSOCKET_SYNC_SERVER} ${SRC}/WebSocketSyncServer.cpp)

add_executable( ${WEBSOCKET_MULTI_SERVER} ${SRC}/WebSocketMultithreadServer.cpp
                ${INC}/FunctionWrapper.h   ${INC}/ThreadSafeQueue.h
                ${INC}/StaticThreadPool.h   ${INC}/JoinThreads.h
                ${INC}/Utils.h)

set_target_properties( ${BOOST_SYNC_SERVER} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        )

set_target_properties( ${BOOST_ASYNC_SERVER} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
        )