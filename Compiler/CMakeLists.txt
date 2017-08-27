set(HEADERS
	ConstantProperties.hpp
	Conversion.hpp
	forwarddeclare.hpp
	Module.hpp
	MutableProperties.hpp
	Operators.hpp
	Operators_Unary.hpp
	Properties.hpp
    PropertyNames.hpp
	ReferencedProperties.hpp
	Regex.hpp
	Struct.hpp
)

set(SOURCES
	ConstantProperties.cpp
	Conversion.cpp
	Module.cpp
	MutableProperties.cpp
	Operators.cpp
	Operators_Unary.cpp
	Properties.cpp
    PropertyNames.cpp
	ReferencedProperties.cpp
	Regex.cpp
	Struct.cpp
)

add_library(compiler ${SOURCES} ${HEADERS})
add_executable(compiler_test main.cpp)
target_link_libraries(compiler_test compiler)
