set(HEADERS
	Exception.hpp
	Keys.hpp
	Interpreter.hpp
	Statement.hpp
)

set(SOURCES
	Interpreter.cpp
	Member.cpp
)

add_library(interpreter ${SOURCES} ${HEADERS})
target_link_libraries(interpreter tokenize)
