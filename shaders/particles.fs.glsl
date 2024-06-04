#version 330 core

uniform float lifetime;
uniform vec3 startColor;
uniform vec3 endColor;

in float lifetime_remaining;

out vec4 fragColor;

// Finds the proportion of lifetime used to lifetime to get a value between [0,1] for interpolation
float calculateParameter() {
	return (lifetime - lifetime_remaining) / lifetime;
}

void main()
{
   fragColor = mix(vec4(startColor, 1.0), vec4(endColor, 0.0), calculateParameter());
}