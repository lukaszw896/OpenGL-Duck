#version 330 core
out vec4 color;

struct Material {
    sampler2D diffuse;
    //vec3 ambient;
    //vec3 diffuse;
    //vec3 specular;
    sampler2D specular;
    float shininess;
};

uniform Material material;

in vec2 TexCoords;

void main()
{
          vec3 result = vec3(1.0f,1.0f,1.0f) * vec3(texture(material.diffuse, TexCoords));
          float alpha = 1.0f;
          if(result[0] < 0.1f)
          {
            alpha = 0.0f;
          }
          color = vec4(result, alpha);
}