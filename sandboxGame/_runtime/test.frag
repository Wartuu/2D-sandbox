#version 430

out vec4 o;

uniform sampler2D tx0;

in vec3 colorTest;
in vec2 coord;
in float currID;

void main() {
    //o = vec4(colorTest, 1.0);
    o = texture(tx0, coord);    
}