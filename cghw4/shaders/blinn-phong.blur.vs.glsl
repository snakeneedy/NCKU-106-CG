#version 330
layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 fTexcoord;
out vec3 fragPos;
out vec3 fNormal;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    fTexcoord = texcoord;
    fragPos = vec3(model * vec4(position, 1.0));
    fNormal = normalize(mat3(transpose(inverse(model)))*normal);
}
