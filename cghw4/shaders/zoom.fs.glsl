#version 330 core
in vec2 fTexcoord;
out vec4 color;

uniform sampler2D uSampler;
uniform vec2 cursorPos;
uniform vec2 screenSize;
uniform int zoomLevel;
uniform mat3 gauss;

const float radius = 50.0;
const float offset = 1.0 / 150.0;
// const float zoom = 0.25;

// float distanceOf (vec2 A, vec2 B)
// {
//     return sqrt(pow(A.x - B.x, 2.0) + pow(A.y - B.y, 2.0));
// }

void main()
{
    float fZoomLevel = zoomLevel;
    
    float d = sqrt(pow(gl_FragCoord.x - cursorPos.x, 2.0) + pow(screenSize.y - gl_FragCoord.y - cursorPos.y, 2.0));
    if (d > radius)
    {
        // result
        color = texture(uSampler, fTexcoord);
        // color = texture(uSampler, fTexcoord) * vec4((ambient + diffuse + specular), 1.0);
    }
    else
    {
        float zoom = 1.0 / float(zoomLevel);
        vec2 _cursorPos;
        _cursorPos.x = cursorPos.x / screenSize.x;
        _cursorPos.y = cursorPos.y / screenSize.y;
        _cursorPos.y = 1.0 - _cursorPos.y;
        // zoom
        vec2 _fTexcoord = _cursorPos + (fTexcoord - _cursorPos) * zoom;


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

        // float kernel[9] = float[](
        //     1, 2, 1,
        //     2, 4, 2,
        //     1, 2, 1);
        float kernel[9] = float[](
            gauss[0][0], gauss[0][1], gauss[0][2],
            gauss[1][0], gauss[1][1], gauss[1][2],
            gauss[2][0], gauss[2][1], gauss[2][2]);
        float kernelSum = 0.0;
        for (int i = 0; i < 9; i++)
            kernelSum += kernel[i];


        vec3 sampleTex[9];
        for (int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(uSampler, _fTexcoord.xy + offsets[i]));
        }

        vec3 col;
        for (int i = 0; i < 9; i++)
        {
            // col += sampleTex[i] * (kernel[i] / 16.0);
            col += sampleTex[i] * (kernel[i] / kernelSum);
        }

        // blur result
        color = vec4(col, 1.0);
    }
}
