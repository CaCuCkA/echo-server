cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME ECHO_SERVER)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set( BOOST_SYNC_SERVER "boost_sync_server" )
set( BOOST_ASYNC_SERVER "boost_async_server" )
set( WEBSOCKET_SYNC_SERVER "websocket_sync_server" )
set( WEBSOCKET_MULTI_SERVER "websocket_multi_server" )

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

####################################################################
include_directories( include )
include_directories( src )

set( SRC ${CMAKE_SOURCE_DIR}/src )
set( INC ${CMAKE_SOURCE_DIR}/include )
set (ALL_TARGETS ${BOOST_SYNC_SERVER} ${BOOST_ASYNC_SERVER}
                 ${WEBSOCKET_SYNC_SERVER} ${WEBSOCKET_MULTI_SERVER} )

####################################################################

add_executable( ${BOOST_SYNC_SERVER} ${SRC}/BoostSyncServer.cpp )

add_executable( ${BOOST_ASYNC_SERVER} ${SRC}/BoostAsyncServer.cpp
        ${INC}/Listener.h       ${SRC}/Listener.cpp
        ${INC}/EchoWebSocket.h  ${SRC}/EchoWebSocket.cpp )

add_executable( ${WEBSOCKET_SYNC_SERVER} ${SRC}/WebSocketSyncServer.cpp
                ${INC}/TCPHandshaker.h   ${SRC}/TCPHandshaker.cpp )

add_executable( ${WEBSOCKET_MULTI_SERVER} ${SRC}/WebSocketMultithreadServer.cpp
                ${INC}/FunctionWrapper.h   ${INC}/ThreadSafeQueue.h
                ${INC}/StaticThreadPool.h   ${INC}/JoinThreads.h
                ${INC}/TCPHandshaker.h   ${SRC}/TCPHandshaker.cpp
                ${INC}/Utils.h )

####################################################################

find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
if( NOT Boost_FOUND )
    message( FATAL_ERROR "Could not find boost" )
endif()
include_directories( ${Boost_INCLUDE_DIRS} )

find_package( OpenSSL REQUIRED )
if ( NOT OPENSSL_FOUND )
    message( FATAL_ERROR "Could not find openssl" )
endif()
target_link_libraries( ${WEBSOCKET_SYNC_SERVER} OpenSSL::SSL OpenSSL::Crypto )
target_link_libraries( ${WEBSOCKET_MULTI_SERVER} OpenSSL::SSL OpenSSL::Crypto )

####################################################################

set_target_properties( ${WEBSOCKET_SYNC_SERVER} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        )

####################################################################

# Include CMake setup
include(cmake/main-config.cmake)