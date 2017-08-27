#pragma once

#include <string>

namespace Utils
{
    class Named
    {
    public:
		Named(std::string const & inName) : _name(inName) {}
		
		std::string const & name() const { return _name; }

    private:
		std::string const _name;
    };
}
}
