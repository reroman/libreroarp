cmake_minimum_required( VERSION 3.5 )
project( libreroarp CXX )
set( CMAKE_CXX_STANDARD 11 )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

#option( ENABLE_VENDORS "Habilita el uso de vendedores en direcciones MAC" ON )
option( BUILD_EXAMPLES "Compila código de ejemplo" ON )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )

set( MAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -O3" )
find_package( Boost 1.58 REQUIRED )
find_package( Sqlite3 )

add_subdirectory( lib )

if( BUILD_EXAMPLES )
	add_subdirectory( examples )
endif()
