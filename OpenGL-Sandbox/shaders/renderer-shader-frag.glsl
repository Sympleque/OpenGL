#version 460 core

layout(location = 0) out vec4 o_Color;

in vec4 vf_Tint;

void main()
{
	o_Color = vf_Tint;
}
