#version 330
layout(location=0) out vec4 color;

in vec2 fTexcoord;
in vec3 fragPos;
in vec3 fNormal;
in mat4 _gl_Position;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D uSampler;
uniform vec2 cursorPos;
uniform vec2 screenSize;

const float radius = 50.0;
const float offset = 1.0 / 150.0;
const float zoom = 0.5;

void main()
{
    float d = sqrt(pow(gl_FragCoord.x - cursorPos.x, 2.0) + pow(screenSize.y - gl_FragCoord.y - cursorPos.y, 2.0));

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
    
    if (d > radius)
    {
        // result
        color = texture(uSampler, fTexcoord) * vec4((ambient + diffuse + specular), 1.0);
    }
    else
    {
        // blur
        vec2 offsets[9] = vec2[](
            vec2(-offset,  offset),
            vec2(    0.0,  offset),
            vec2( offset,  offset),
            vec2(-offset,     0.0),
            vec2(    0.0,     0.0),
            vec2( offset,     0.0),
            vec2(-offset, -offset),
            vec2(    0.0, -offset),
            vec2( offset, -offset));

        float kernel[9] = float[](
            1, 2, 1,
            2, 4, 2,
            1, 2, 1);

        vec3 sampleTex[9];
        for (int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(uSampler, fTexcoord + offsets[i]));
        }

        vec3 col;
        for (int i = 0; i < 9; i++)
        {
            col += sampleTex[i] * (kernel[i] / 16.0);
        }

        // blur result
        color = vec4(col, 1.0) * vec4((ambient + diffuse + specular), 1.0);
    }
}

