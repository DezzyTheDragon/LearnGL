#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;


void main()
{
	FragPos = vec3(u_Model * vec4(position, 1.0));
	Normal = normal;
	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 u_Color;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;


void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColor;

	vec3 result = (ambient + diffuse) * u_Color;
	color = vec4(result, 1.0);
};