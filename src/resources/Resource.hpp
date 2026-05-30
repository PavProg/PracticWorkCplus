#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include <filesystem>

template<typename T>
struct Resource {
	std::string path;
	T data;
	std::filesystem::file_time_type lastModified{};
};


#endif