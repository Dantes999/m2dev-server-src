file(GLOB_RECURSE DB_SOURCES "*.h" "*.c" "*.cpp")
add_executable(db ${DB_SOURCES})
target_compile_definitions(db PRIVATE GIT_DESCRIBE="${GIT_DESCRIBE_VERSION}")
target_link_libraries(db 
	common 
	libgame 
	libpoly 
	libsql 
	libthecore 
	liblua
	# external
	mariadbclient
	cryptopp-static
)
if (WIN32)
	target_link_libraries(db ws2_32)
else()
	if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
		target_link_libraries(db pthread md)
	else()
		target_link_libraries(db pthread)
	endif()
endif()

# Remove -lmd flag on non-FreeBSD systems (it's only available on BSD)
if(NOT CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" AND NOT WIN32)
	if(TARGET md_filter)
		target_link_libraries(db PRIVATE md_filter)
	endif()
endif()
