set(HEADERS
	Exception.hpp
	Keys.hpp
	Interpreter.hpp
	Statement.hpp
	Store.hpp
	FirstRound/Module.hpp
	FirstRound/Properties.hpp
	FirstRound/Regex.hpp
)


set(SOURCES
	Interpreter.cpp
	Member_interpreter.cpp
	Store.cpp
	Value_interpreter.cpp
	FirstRound/Module.cpp
	FirstRound/Properties.cpp
	FirstRound/Regex.cpp
)

add_library(interpreter ${SOURCES} ${HEADERS})
target_link_libraries(interpreter tokenize)
