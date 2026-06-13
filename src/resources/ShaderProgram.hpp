#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <string>

struct GPUShader {
	unsigned int programId = 0;
};

struct ShaderSources {
	std::string vertex;
	std::string fragment;
};

struct ShaderProgram {
	ShaderSources sources;
	GPUShader gpu;
};

#endif