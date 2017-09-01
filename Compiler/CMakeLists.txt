set(HEADERS
	Conversion.hpp
	Module.hpp
	Properties.hpp
	Regex.hpp
)

set(SOURCES
	Conversion.cpp
	Module.cpp
	Properties.cpp
	Regex.cpp
)

add_library(compiler ${SOURCES} ${HEADERS})
