#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position; // a for attribut for incoming
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Position; // v for var between the shaders; this goes out
out vec4 v_Color;		

void main()
{
	v_Position = a_Position;
	v_Color = a_Color;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position; // here the var from vertexShader comes in
in vec4 v_Color;

void main()
{
	color = vec4(v_Position, 1.0);
	color = v_Color;
}