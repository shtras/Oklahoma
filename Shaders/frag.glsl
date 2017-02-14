#version 330 core

out vec3 color;
in vec2 UV;

uniform sampler2D sampler;

void main() {
    color = texture( sampler, UV ).rgb;
}
