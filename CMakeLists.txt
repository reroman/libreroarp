cmake_minimum_required( VERSION 3.5 )
project( libreroarp CXX )
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_BUILD_TYPE Release )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" )

option( BUILD_EXAMPLES "Compila códigos de ejemplo" ON )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )

set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -O3" )
find_package( Boost 1.58 REQUIRED )
find_package( Sqlite3 )

add_subdirectory( lib )

if( BUILD_EXAMPLES )
	add_subdirectory( examples )
endif()
