set(HEADERS
	ConstantProperties.hpp
	Conversion.hpp
	Module.hpp
	MutableProperties.hpp
	Operators.hpp
	Operators_Unary.hpp
	Properties.hpp
    PropertyNames.hpp
	ReferencedProperties.hpp
	Regex.hpp
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
)

add_library(compiler ${SOURCES} ${HEADERS})
