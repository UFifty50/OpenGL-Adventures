#version 460 core


// variables stored in format [space]_name
layout(location = 0) in vec2 screen_VertexPos;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;


void main()
{
    // map vetex position in clip space
    // map to [-1..1][-1..1]
    vec2 homoeneous_VertexPos = screen_VertexPos - vec2(400, 300);
    homoeneous_VertexPos /= vec2(400, 300);
    gl_Position = vec4(homoeneous_VertexPos, 0, 1);

    UV = vertexUV;    
}
