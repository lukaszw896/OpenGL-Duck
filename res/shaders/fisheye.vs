#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model *  vec4(position, 1.0f);
    vec4 tmp_point = view*model*vec4(position,1.0f);
    gl_Position.xy = tmp_point.xy / (length(tmp_point.xyz)/1.0);
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}