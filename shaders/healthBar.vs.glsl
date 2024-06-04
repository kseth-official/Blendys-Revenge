#version 330 core

layout(location = 0) in vec2 in_position;
out vec2 fragTexCoord;

uniform mat3 projection;

void main()
{
    gl_Position = vec4((projection * vec3(in_position, 1.0)).xy, 0.0, 1.0);
    fragTexCoord = in_position * 0.5 + 0.5;
}
