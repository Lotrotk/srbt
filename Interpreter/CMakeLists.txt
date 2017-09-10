set(HEADERS
	Exception.hpp
	Keys.hpp
	Interpreter.hpp
	Statement.hpp
	Store.hpp
	FirstRound/Module.hpp
	FirstRound/Operators.hpp
	FirstRound/Properties.hpp
	FirstRound/Regex.hpp
)


set(SOURCES
	Declaration_interpreter.cpp
	Expression_interpreter.cpp
	Interpreter.cpp
	Store.cpp
	Value_interpreter.cpp
	FirstRound/Module.cpp
	FirstRound/Properties.cpp
	FirstRound/Regex.cpp
)

add_library(interpreter ${SOURCES} ${HEADERS})
target_link_libraries(interpreter tokenize)
