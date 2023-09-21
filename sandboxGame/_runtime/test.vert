#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float iTime;
uniform vec2 noise;

out float currID;

out vec3 colorTest;
out vec2 coord;

void main() {
    int x = (gl_InstanceID) % 1000;
    int y = (gl_InstanceID) / 1000;
    vec3 aPosF = aPos;


    aPosF.x += x * 20.0f;
    aPosF.y += y * 20.0f;

    coord = aTex;

    currID = int(gl_InstanceID) % 3;
    currID = int(currID);

    gl_Position = proj * view * model * vec4(aPos, 1.0);
}