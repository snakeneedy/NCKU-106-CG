#version 330
layout(location=0) out vec4 color;

in vec2 fTexcoord;
in vec3 fragPos;
in vec3 fNormal;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D uSampler;

void main()
{    
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    // diffuse 
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 diffuse = max(dot(fNormal, lightDir), 0.0) * lightColor;
    // specular 
    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    // vec3 reflectDir = reflect(-lightDir, fNormal);
    vec3 specular = specularStrength * pow(max(dot(fNormal, halfDir), 0.0), 4) * lightColor;
    
    color = texture(uSampler, fTexcoord) * vec4((ambient + diffuse + specular), 1.0);
}
