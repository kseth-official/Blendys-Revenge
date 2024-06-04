#version 330 core

layout ( location = 0 ) in vec3 vertex_position;
layout ( location = 4 ) in vec4 position;

uniform mat3 M_v;
uniform mat3 M_p;
uniform float particleSize;

out float lifetime_remaining;

void main()
{
   vec3 position_viewspace = position.xyz;
   position_viewspace.xy += particleSize * (vertex_position.xy - vec2(0.5)); 
   // [0, 1] to [position_viewspace - particleSize * 0.5, position_viewspace + particleSize * 0.5]
   lifetime_remaining = position.w;
   gl_Position = vec4(M_p * position_viewspace, 1.0);
};