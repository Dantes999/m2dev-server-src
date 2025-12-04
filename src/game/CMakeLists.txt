file(GLOB_RECURSE GAME_SOURCES "*.h" "*.c" "*.cpp")
add_executable(game ${GAME_SOURCES})
target_compile_definitions(game PRIVATE GIT_DESCRIBE="${GIT_DESCRIBE_VERSION}")
target_link_libraries(game 
	common 
	-Wl,--start-group
	libgame 
	libpoly 
	libsql 
	libthecore 
	liblua
	-Wl,--end-group
	# external
	mariadbclient
	cryptopp-static
)
if (WIN32)
	target_link_libraries(game ws2_32)
else()
	if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
		target_link_libraries(game pthread md)
	else()
		target_link_libraries(game pthread)
	endif()
endif()
