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
uniform float radiusY; 
uniform float radiusZ; 
uniform float horizontalSpeed;
uniform float startX;

void main()
{
    float dx = horizontalSpeed;
    float dy = -radiusY * sin(time);
    float dz =  radiusZ * cos(time);

    float angleY = -atan(dz, dx);
    float angleZ = atan(dy, dx);

    mat3 rotY = mat3(
        cos(angleY), 0.0, sin(angleY),
        0.0, 1.0, 0.0,
        -sin(angleY), 0.0, cos(angleY)
    );

    mat3 rotZ = mat3(
        cos(angleZ), -sin(angleZ), 0.0,
        sin(angleZ),  cos(angleZ), 0.0,
        0.0, 0.0, 1.0
    );

    mat3 combinedRot = rotY * rotZ;

    vec3 localPos = combinedRot * aPos;

    vec3 trajectory;
    trajectory.x = startX + (horizontalSpeed * time);
    trajectory.y = radiusY * cos(time);
    trajectory.z = radiusZ * sin(time);

    vec3 finalPos = trajectory + localPos;

    gl_Position = projection * view * model * vec4(finalPos, 1.0);

    TexCoords = aTexCoords;
    Normal = mat3(transpose(inverse(model))) * (combinedRot * aNormal);
}