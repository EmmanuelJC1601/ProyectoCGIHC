#version 330 core
layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aNormal;
layout (location = 2) in vec2  aTexCoords;
layout (location = 3) in vec3  tangent;
layout (location = 4) in vec3  bitangent;

out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float radius;
uniform float height;

void main()
{
    // 1. Ángulo de la tangente al círculo.
    //    Posición:  ( cos(t),  sin(t) )
    //    Tangente:  (-sin(t),  cos(t) )  → ángulo = t + PI/2
    //
    //    El frente del camión es -Y, es decir en reposo apunta a (0,-1).
    //    Para que quede alineado con la tangente sumamos otro PI/2.
    //    Resultado: angle = time + PI  (equivale a time + 1.5708 + 1.5708)
    float angle = -time - 3.14159265; 

    // 2. Matriz de rotación 2D sobre el plano XY
    mat2 rot = mat2(
        cos(angle), -sin(angle),
        sin(angle),  cos(angle)
    );

    // 3. Rotar la malla local para que "mire" hacia la tangente
    vec3 localPos = aPos;
    localPos.xy = rot * localPos.xy;

    // 4. Trasladar al punto de la órbita circular
    vec4 PosL = vec4(localPos, 1.0);
    PosL.x += radius * cos(time);
    PosL.y += radius * sin(time);
    PosL.z += height;

    // 5. Rotar las normales con el mismo ángulo para iluminación correcta
    vec3 rotatedNormal = aNormal;
    rotatedNormal.xy = rot * rotatedNormal.xy;

    // 6. Transformación final
    gl_Position = projection * view * model * PosL;

    TexCoords = aTexCoords;
    Normal = mat3(transpose(inverse(model))) * rotatedNormal;
}