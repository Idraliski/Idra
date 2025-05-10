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