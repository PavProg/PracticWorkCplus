#version 330 core

in vec3 vUV;

uniform sampler2D u_Texture;
uniform vec4 u_Tint;

out vec4 FragColor;

void main() {
	vec4 texColor = texture(u_Texture, vUV);
	FragColor = texColor * u_Tint;
}