#version 330
layout(location=0) out vec4 color;

uniform sampler2D uSampler;

in vec2 fTexcoord;
in vec4 resultLight;

void main()
{
    color = texture(uSampler, fTexcoord) * resultLight;
}
