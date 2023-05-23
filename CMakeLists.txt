cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME_ONE sync_echo_server)
set(PROJECT_NAME_TWO multi_echo_server)
set(PROJECT_NAME_THREE async_echo_server)

project(${PROJECT_NAME_ONE} LANGUAGES CXX)
project(${PROJECT_NAME_TWO} LANGUAGES CXX)
project(${PROJECT_NAME_THREE} LANGUAGES CXX)

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


include( cmake/CompilerWarnings.cmake )


include_directories( include )
include_directories( src )

set( SRC ${CMAKE_SOURCE_DIR}/src )
set( INC ${CMAKE_SOURCE_DIR}/include )

set( COMMON_FILES 				${SRC}/Main.cpp
	 ${INC}/Exception.h 		${SRC}/Exception.cpp
	 ${INC}/Server.h 		 	${SRC}/Server.cpp
	 ${INC}/Handshake.h			${SRC}/Handshake.cpp
	 ${INC}/ThreadSafeQueue.h
	 ${INC}/CrossType.h
	 ${INC}/CrossFunctions.h
	 ${INC}/Utils.h )

add_executable( ${PROJECT_NAME_ONE} 			${COMMON_FILES}
				${INC}/SyncEchoServer.h			${SRC}/SyncEchoServer.cpp )

add_executable( ${PROJECT_NAME_TWO} 			${COMMON_FILES}
				${INC}/MultiEchoServer.h		${SRC}/MultiEchoServer.cpp )

add_executable( ${PROJECT_NAME_THREE} 			${COMMON_FILES}
				${INC}/AsyncEchoServer.h		${SRC}/AsyncEchoServer.cpp )

####################################################################

target_compile_definitions(${PROJECT_NAME_ONE} PRIVATE SYNC)
target_compile_definitions(${PROJECT_NAME_TWO} PRIVATE MULTI)
target_compile_definitions(${PROJECT_NAME_THREE} PRIVATE ASYNC)

find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
target_include_directories(${PROJECT_NAME_ONE} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME_ONE} Boost::boost)

target_include_directories(${PROJECT_NAME_TWO} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME_TWO} Boost::boost)

target_include_directories(${PROJECT_NAME_THREE} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME_THREE} Boost::boost)

find_package(OpenSSL REQUIRED)

target_link_libraries(${PROJECT_NAME_ONE} OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(${PROJECT_NAME_TWO} OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(${PROJECT_NAME_THREE} OpenSSL::SSL OpenSSL::Crypto)


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
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
		)

set_target_properties( ${PROJECT_NAME_THREE} PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
		)
####################################################################

# Include CMake setup
include(cmake/main-config.cmake)