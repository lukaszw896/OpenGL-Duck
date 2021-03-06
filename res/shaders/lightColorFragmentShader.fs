#version 330 core
out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    //vec3 ambient;
    //vec3 diffuse;
    //vec3 specular;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
          // Ambient
          //vec3 ambient  = light.ambient * material.ambient;
          vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

          // Diffuse
          vec3 norm = normalize(Normal);
          vec3 lightDir = normalize(lightPos - FragPos);
          float diff = max(dot(norm, lightDir), 0.0);
          //vec3 diffuse  = light.diffuse * (diff * material.diffuse);
          vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

          // Specular
          //V = normalize(viewPos - FragPos)
          vec3 viewDir = normalize(viewPos - FragPos);
          //R = reflect(-lightDir, norm)
          vec3 reflectDir = reflect(-lightDir, norm);

          float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
          vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));


          vec3 result = ambient + diffuse + specular;
          color = vec4(result, 1.0f);
}