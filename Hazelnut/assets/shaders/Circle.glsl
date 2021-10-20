//////////////
// Hazel 2D //
//////////////
//
// Renderer 2D Circle Shader
//

#type vertex
#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in float a_Thickness;
layout(location = 2) in vec2 a_LocalPosition;
layout(location = 3) in vec4 a_Color;
layout(location = 4) in int a_EntityID;

uniform mat4 u_ViewProjection;	

out vec2 v_LocalPosition;
out float v_Thickness;
out vec4 v_Color;
flat out int v_EntityID;

void main()
{
	v_LocalPosition = a_LocalPosition;
	v_Thickness = a_Thickness;
	v_Color = a_Color;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

in vec2 v_LocalPosition;
in float v_Thickness;
in vec4 v_Color;
flat in int v_EntityID;

void main()
{
	float fade = 0.01;
	float dist = sqrt(dot(v_LocalPosition, v_LocalPosition));
	float t = v_Thickness;
	if (dist > 1.0 || dist < 1.0 - t - fade)
		discard;

	float alpha = 1.0 - smoothstep(1.0f - fade, 1.0f, dist);
	alpha *= smoothstep(1.0 - t - fade, 1.0 - t, dist);
	o_Color = v_Color;
	o_Color.a = v_Color.a * alpha;

	o_EntityID = v_EntityID;
}