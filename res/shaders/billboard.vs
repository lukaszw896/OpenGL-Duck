#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int perpendicular;
uniform float scale;

out vec2 TexCoords;

void main()
{
mat4 myModel =view * model;
    // First colunm.

    float myScale = 1.0;
    if(perpendicular == 1){
    myScale = scale;
    }

    myModel[0][0] = myScale;
    myModel[0][1] = 0.0;
    myModel[0][2] = 0.0;
    if(perpendicular == 1)
    {
        myModel[1][0] = 0.0;
        myModel[1][1] = myScale;
        myModel[1][2] = 0.0;
    }

    // Thrid colunm.
    myModel[2][0] = 0.0;
    myModel[2][1] = 0.0;
    myModel[2][2] = myScale;

    gl_Position = projection *  myModel * vec4(position, 1.0f);
    TexCoords = texCoords;
}