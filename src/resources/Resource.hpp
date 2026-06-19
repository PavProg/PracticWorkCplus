#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include <filesystem>
#include <atomic>

enum class ResourceState { Loading, Ready, Failed };

template<typename T>
struct Resource {
	std::string path;
	T data;
	std::filesystem::file_time_type lastModified{};
	std::atomic<ResourceState> state{ ResourceState::Ready };
};


#endif