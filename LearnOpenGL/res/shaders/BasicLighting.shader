#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_MVP;
uniform mat4 u_Model;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = u_MVP * vec4(position, 1.0);
	FragPos = vec3(u_Model * vec4(position, 1.0));
	Normal = normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_Color;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;

in vec3 FragPos;
in vec3 Normal;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColor;

	vec3 result = u_Color * (ambient + diffuse);
	color = vec4(result, 1.0);
};