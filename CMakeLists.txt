cmake_minimum_required( VERSION 3.5 )
project( libreroarp CXX )
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_BUILD_TYPE Release )

option( BUILD_EXAMPLES "Compila códigos de ejemplo" ON )

set( CMAKE_CXX_FLAGS_RELEASE
	"${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -O3" )
find_package( Boost 1.58 REQUIRED )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )

add_library( reroarp
	lib/ipv4addr.cpp
	lib/hwaddr.cpp
	lib/networkinterface.cpp
	lib/arp.cpp
)

if( BUILD_EXAMPLES )
	add_subdirectory( examples )
endif()
