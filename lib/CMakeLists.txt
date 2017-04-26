add_library( reroarp
	ipv4addr.cpp
	hwaddr.cpp
	networkinterface.cpp
)

if( ENABLE_VENDORS )
	find_package( Sqlite3 REQUIRED )
	target_link_libraries( reroarp sqlite3 )
endif()
