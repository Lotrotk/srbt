SET(HEADERS
	Debug.hpp
	Enumerator.hpp
	Exception.hpp
    Macros.hpp
	Named.hpp
	Refarray.hpp
	Sharedpointer.hpp
    Strlen.hpp
)

SET( SOURCES
)

add_custom_target(utils SOURCES ${HEADERS} ${SOURCES})
