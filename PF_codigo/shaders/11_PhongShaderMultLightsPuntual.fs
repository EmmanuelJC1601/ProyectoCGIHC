#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;

uniform vec4 MaterialAmbientColor;
uniform vec4 MaterialDiffuseColor;
uniform vec4 MaterialSpecularColor;
uniform float transparency;

#define MAX_LIGHTS 20
uniform int numLights;

struct Light {
    vec3 Position;
    vec3 Direction;

    vec4 Color;
    vec4 Power;

    int alphaIndex;

    float distance;
    float cutOff;
    float outerCutOff;

    int type; // 0: Punto, 1: Foco, 2: Direccional
};

uniform Light allLights[MAX_LIGHTS];

vec4 ApplyLight(Light light, vec3 N, vec3 E)
{
    vec3 L;
    float attenuation = 1.0;
    float spotFactor = 1.0;

    // --- 1. DIRECCIÓN Y ATENUACIÓN ---
    if (light.type == 2)
    {
        // Luz direccional
        L = normalize(-light.Direction);
        attenuation = light.Power.x;
    }
    else
    {
        // Luces posicionales
        L = normalize(light.Position - FragPos);

        float d = length(light.Position - FragPos);
        attenuation = light.Power.x / (d + 1.0);

        // Spotlight
        if (light.type == 1)
        {
            float theta = dot(normalize(-L), normalize(light.Direction));

            float epsilon = light.cutOff - light.outerCutOff;

            spotFactor = clamp(
                (theta - light.outerCutOff) / epsilon,
                0.0,
                1.0
            );
        }
    }

    // --- 2. MODELO PHONG ---

    // Ambiental
    vec4 K_a = MaterialAmbientColor * light.Color;

    // Difusa
    float cosTheta = max(dot(N, L), 0.0);

    vec4 K_d =
        MaterialDiffuseColor *
        light.Color *
        cosTheta;

    // Especular
    vec3 R = reflect(-L, N);

    float cosAlpha = max(dot(E, R), 0.0);

    vec4 K_s =
        MaterialSpecularColor *
        light.Color *
        pow(cosAlpha, float(light.alphaIndex));

    // Resultado final
    return (K_a + (K_d + K_s) * spotFactor) * attenuation;
}

void main()
{
    vec3 n = normalize(Normal);
    vec3 e = normalize(viewPos - FragPos);

    vec4 totalLighting = vec4(0.0);

    for (int i = 0; i < numLights; ++i)
    {
        totalLighting += ApplyLight(allLights[i], n, e);
    }

    vec4 texel = texture(texture_diffuse1, TexCoords);

    // Override emisivo para láser
    if (
        MaterialAmbientColor.r > 1.0 ||
        MaterialAmbientColor.g > 1.0 ||
        MaterialAmbientColor.b > 1.0
    )
    {
        FragColor = vec4(
            MaterialAmbientColor.rgb,
            transparency
        );
    }
    else
    {
        FragColor = texel * totalLighting;
        FragColor.a = transparency;
    }
}