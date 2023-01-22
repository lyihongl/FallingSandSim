#version 430 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aOffset;
out vec3 oColor;
uniform vec3 color;
uniform mat4 model;
void main() {
    // vec4 temp = model*vec4(aPos, 0.0, 1.0);
    // gl_Position = vec4(temp.x + aOffset.x, temp.y + aOffset.y, 0.0, 1.0);
    gl_Position = model*vec4(aPos+aOffset, 0.0, 1.0);
    oColor = color;
}