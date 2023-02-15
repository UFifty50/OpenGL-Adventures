#version 460 core

// variables stored in format [space]_name

layout(location = 0) in vec3 model_VertexPos;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 model_VertexNormal;

out vec2 UV;
out vec3 world_Position;
out vec3 camera_Normal;
out vec3 camera_EyeDirection;
out vec3 camera_LightDirection;

uniform mat4 ModelViewProjection;
uniform mat4 View;
uniform mat4 Model;
uniform vec3 world_LightPos;

void main() {
    gl_Position = ModelViewProjection * vec4(model_VertexPos, 1);

    world_Position = (Model * vec4(model_VertexPos, 1)).xyz;
    
    vec3 camera_vertexPos = (View * Model * vec4(model_VertexPos, 1)).xyz;
    camera_EyeDirection = vec3(0, 0, 0) - camera_vertexPos;

    vec3 camera_LightPos = (View * vec4(world_LightPos, 1)).xyz;
    camera_LightDirection = camera_LightPos + camera_EyeDirection;

    camera_Normal = (View * Model * vec4(model_VertexNormal, 0)).xyz;


    UV = vertexUV;
}
