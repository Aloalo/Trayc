
layout(location = 0) in vec4 position_os;

uniform mat4 depthMVP;

void main() {
    gl_Position =  depthMVP * position_os;
}