#version 330 core

out vec4 color;
in vec2 UV;
in vec3 vPos;
uniform vec4 bound;

uniform sampler2D sampler;

void main() {
    if (vPos.x >= bound.x
     && vPos.x <= bound[2]
      && vPos.y >= bound[1]
       && vPos.y <= bound[3]) {
        color = texture( sampler, UV ).rgba;
    } else {
        color = vec4(0,0,0,1);
    }
}
