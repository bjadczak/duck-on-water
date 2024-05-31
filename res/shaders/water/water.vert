#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 localPos;
out vec3 worldPos;
out vec3 norm;
out vec2 texCoords;

void main()
{
    norm = aNormal;
    localPos = aPos;
    localPos.y = 0.0f;
    vec4 worldPosition = model * vec4(localPos, 1.0);
    worldPos = worldPosition.xyz;
    texCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}