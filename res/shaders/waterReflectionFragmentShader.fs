#version 330 core
in vec3 Normal;
in vec3 Position;
out vec4 color;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{
    //vec3 I = normalize( viewPos - Position);
    //vec3 R = reflect(I, normalize(Normal));
    //color = texture(skybox, R);

    float ratio = 1.00f / 1.33f;
        vec3 I = normalize( viewPos - Position);

        vec3 Rreflect = reflect(I, normalize(Normal));
        vec3 Rrefract = refract(I, normalize(Normal), ratio);

        vec4 refl = texture(skybox, Rreflect);
        vec4 refr = texture(skybox, Rrefract);

        float rzero = pow((1.33 - 1.0)/(1.33 + 1.0), 2.0);
        float cosfi = abs(normalize(-normalize( viewPos - Position)).y);
        float frasnel = rzero + (1.0-rzero) * pow(1.0-cosfi, 5.0);

        color = vec4(vec3(mix(refr, refl, frasnel)), 0.7f);
}