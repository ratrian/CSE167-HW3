#version 330 core

uniform float drawDiscoball;
uniform float normalColoring;
uniform float drawLightSource;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 lightAtten;

uniform vec3 cameraPos;
uniform samplerCube skybox;

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 posOutput;
in vec2 texcoordOutput;
in vec3 normalOutput;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

vec3 CalcPointLight(vec3 fragPos, vec3 normal, vec3 viewDir);

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    if (drawDiscoball == 1.0)
    {
        vec3 I = normalize(cameraPos - posOutput);
        vec3 R = reflect(I, normalize(normalOutput));
        fragColor = vec4(texture(skybox, R).rgb, 1.0);
    }
    else if (drawDiscoball == 0.0)
    {
        vec3 norm = normalize(normalOutput);

        if (drawLightSource == 1.0)
        {
            if (normalColoring == 1.0)
                fragColor = vec4(vec3(0.0), 1.0);
            else if (normalColoring == 0.0)
                fragColor = vec4(lightCol, 1.0);
        }
        else if (drawLightSource == 0.0)
        {
            if (normalColoring == 1.0)
            {
                norm = 0.5 * norm + 0.5;
                fragColor = vec4(norm, 1.0);
            }
            else if (normalColoring == 0.0)
            {
                vec3 viewDir = normalize(posOutput);
                vec3 result = CalcPointLight(posOutput, norm, viewDir);
                fragColor = vec4(result, 1.0);
            }
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
    float attenuation = lightAtten.x + lightAtten.y * distance + lightAtten.z * distance * distance;

    vec3 result;
    result.x = (lightCol.x / attenuation) * (ambient.x + diff * diffuse.x + spec * specular.x);
    result.y = (lightCol.y / attenuation) * (ambient.y + diff * diffuse.y + spec * specular.y);
    result.z = (lightCol.z / attenuation) * (ambient.z + diff * diffuse.y + spec * specular.z);
    return result;
}