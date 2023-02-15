#version 460 core


in vec2 UV;

out vec4 colour;

uniform sampler2D Text2DTextureSampler;


void main()
{
    colour = texture(Text2DTextureSampler, UV);
}
