#version 330 core
// This is a sample fragment shader.

uniform float normalColoring;
uniform float drawSphere;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 lightAtten;

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 posOutput;
in vec3 normalOutput;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

vec3 CalcPointLight(vec3 fragPos, vec3 normal, vec3 viewDir);

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    vec3 norm = normalize(normalOutput);

    if (drawSphere == 1.0) {
        if (normalColoring == 1.0)
            fragColor = vec4(vec3(0.0), 1.0);
        else if (normalColoring == 0.0)
            fragColor = vec4(lightCol, 1.0);
    } else if (drawSphere == 0.0) {
        if (normalColoring == 1.0) {
            norm = 0.5 * norm + 0.5;
            fragColor = vec4(norm, 1.0);
        }
        else if (normalColoring == 0.0) {
            vec3 viewDir = normalize(posOutput);
            vec3 result = CalcPointLight(posOutput, norm, viewDir);
            fragColor = vec4(result, 1.0);
        }
    }
}

// Calculates the color when using a point light.
vec3 CalcPointLight(vec3 fragPos, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(lightPos - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // Attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0f / (lightAtten.x + lightAtten.y * distance + lightAtten.z * distance * distance);

    return (ambient * lightCol.x + diffuse * diff * lightCol.y * attenuation + specular * spec * lightCol.z * attenuation);
}