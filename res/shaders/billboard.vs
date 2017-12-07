#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
mat4 myModel =view * model;
    // First colunm.
    myModel[0][0] = 1.0;
    myModel[0][1] = 0.0;
    myModel[0][2] = 0.0;


    // Thrid colunm.
    myModel[2][0] = 0.0;
    myModel[2][1] = 0.0;
    myModel[2][2] = 1.0;
    gl_Position = projection *  myModel * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    //inverse is a costly operation
    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoords;
}