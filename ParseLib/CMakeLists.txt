set(HEADERS
	File.hpp
	Tree.hpp
)

set(SOURCES
	File.cpp
	Tree.cpp
)

add_library(parselib ${SOURCES} ${HEADERS})
