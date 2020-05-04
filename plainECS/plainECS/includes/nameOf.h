#pragma once

#include <string>
#include <typeinfo>

/*
	Returns a name string of given datatype.
*/
template <typename T>
static std::string __nameof()
{
	size_t start = 0;
	size_t index = 0;

	// line is formated as "class <namespaces>::NAME_OF_CLASS"
	// Note that <namespaces> is representing multiple occurrences of
	// namespaces, such as in Custom::Math::Vector::... and so on.
	std::string line = typeid(T).name();

	// .find("::", start) here returns the index of the first occurrence of ':'
	// after the position in the argument "start".
	// .find() returns std::string::npos if there are no more occurrences.
	while ((index = line.find("::", start)) != std::string::npos)
	{
		// Start at index after both colons "::"
		start = index + 2;
	}

	// read from position "start" to end of "line"
	return line.substr(start, std::string::npos);
}