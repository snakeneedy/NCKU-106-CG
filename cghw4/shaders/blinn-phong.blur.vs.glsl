#version 330
layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform vec2 cursorPos;
uniform vec2 screenSize;

out vec2 fTexcoord;
out vec3 fragPos;
out vec3 fNormal;

const float radius = 50.0;
const float offset = 1.0 / 150.0;
const float zoom = 0.5;

// Ref. http://stackoverflow.com/questions/21830340/understanding-glmlookat
mat4 lookAt(vec3 eye, vec3 center, vec3 up)
{
    vec3 X, Y, Z;
    mat4 Matrix = mat4(0.0);
    Z = normalize(eye - center);
    Y = up;
    X = cross(Y, Z);
    Y = cross(Z, X);
    X = normalize(X);
    Y = normalize(Y);
    Matrix[0][0] = X.x;
    Matrix[1][0] = X.y;
    Matrix[2][0] = X.z;
    Matrix[3][0] = -dot(X, eye);
    Matrix[0][1] = Y.x;
    Matrix[1][1] = Y.y;
    Matrix[2][1] = Y.z;
    Matrix[3][1] = -dot(Y, eye);
    Matrix[0][2] = Z.x;
    Matrix[1][2] = Z.y;
    Matrix[2][2] = Z.z;
    Matrix[3][2] = -dot(Z, eye);
    Matrix[0][3] = 0;
    Matrix[1][3] = 0;
    Matrix[2][3] = 0;
    Matrix[3][3] = 1.0;

    return Matrix;
}

void main()
{
    // float d = sqrt(pow(position.x * screenSize.x - cursorPos.x, 2.0) + pow(screenSize.y - position.y * screenSize.y - cursorPos.y, 2.0));
    vec3 _cameraPos = cameraPos;
    // if (d <= radius)
    // {
    //     _cameraPos = vec3(zoom) * _cameraPos;
    // }
    mat4 _view;
    _view = lookAt(_cameraPos, vec3(0.0), vec3(0.0, 1.0, 0.0));
    gl_Position = projection * _view * model * vec4(position, 1.0);
    
    fTexcoord = texcoord;
    fragPos = vec3(model * vec4(position, 1.0));
    fNormal = normalize(mat3(transpose(inverse(model)))*normal);
}
