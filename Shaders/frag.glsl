#version 330 core

out vec4 color;
in vec2 UV;

uniform sampler2D sampler;

void main() {
    color = texture( sampler, UV ).rgba;
}
