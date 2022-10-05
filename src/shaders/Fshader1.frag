#version 460 core


in vec2 UV;
out vec3 colour;

uniform sampler2D textureSampler;

void main() {
    colour = texture(textureSampler, UV).rgb;
}
