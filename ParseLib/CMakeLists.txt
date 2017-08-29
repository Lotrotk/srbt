set(HEADERS
	File.hpp
	Tree.hpp
)

set(SOURCES
	File.cpp
	main.cpp
	Tree.cpp
)

add_library(parselib ${SOURCES} ${HEADERS})
add_executable(parselib_test main.cpp)
target_link_libraries(parselib_test parselib)
