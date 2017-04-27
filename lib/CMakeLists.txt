add_library( reroarp
	ipv4addr.cpp
	hwaddr.cpp
	networkinterface.cpp
	arp.cpp
)

if( SQLITE3_FOUND )
	set( ENABLE_VENDORS TRUE )
	target_link_libraries( reroarp sqlite3 )
else()
	message( WARNING "Vendors are disabled" )
endif()

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
	"${CMAKE_CURRENT_BINARY_DIR}/config.hpp" )
include_directories( "${CMAKE_CURRENT_BINARY_DIR}" )

