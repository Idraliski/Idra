#type vertex
#version 460 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec3 v_TexCoords;

void main()
{
	v_TexCoords = a_Pos;
	vec4 pos = u_ViewProjection * vec4(a_Pos, 1.0);
	gl_Position = pos.xyww; // Set depth to 1.0
}  

#type fragment
#version 460 core

in vec3 v_TexCoords;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform samplerCube u_Skybox;

void main()
{
	FragColor = texture(u_Skybox, v_TexCoords);
}