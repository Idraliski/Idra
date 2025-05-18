#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;
out vec3 v_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	v_Color = a_Color;
	v_Position = a_Position;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec3 v_Position;

void main()
{
	color = v_Color;
	//color = vec4(v_Position * 0.5 + 0.5, 1.0); //static color based on position
	//color = vec4(0.8, 0.2, 0.3, 1.0); //static uniform color

}