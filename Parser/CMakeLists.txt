set(HEADERS
	Exception.hpp
	Keys.hpp
	Parser.hpp
)

set(SOURCES
	Parser.cpp
)

add_library(parser ${SOURCES} ${HEADERS})
target_link_libraries(parser parselib)
