set(HEADERS
	Exception.hpp
	Keys.hpp
	Interpreter.hpp
)

set(SOURCES
	Interpreter.cpp
)

add_library(interpreter ${SOURCES} ${HEADERS})
target_link_libraries(interpreter tokenize)
