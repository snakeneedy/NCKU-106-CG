#version 330
layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

out vec2 fTexcoord;
out vec4 resultLight;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    vec3 fragPos = vec3(model * vec4(position, 1.0));
    vec3 fNormal = normalize(mat3(transpose(inverse(model)))*normal);
    
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    // diffuse 
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 diffuse = max(dot(fNormal, lightDir), 0.0) * lightColor;
    // specular 
    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fNormal);
    vec3 specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 8) * lightColor;
    
    fTexcoord = texcoord;
    resultLight = vec4((ambient + diffuse + specular), 1.0);
}
