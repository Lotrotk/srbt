set(HEADERS
	Exception.hpp
	Keys.hpp
	IdentifyType.hpp
	Interpreter.hpp
	Statement.hpp
	Store.hpp
)

set(SOURCES
	Interpreter.cpp
	IdentifyType.cpp
	Member_interpreter.cpp
	Store.cpp
	Value_interpreter.cpp
)

add_library(interpreter ${SOURCES} ${HEADERS})
target_link_libraries(interpreter tokenize)
