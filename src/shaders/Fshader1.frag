#version 460 core


in vec2 UV;
in vec3 world_Position;
in vec3 camera_Normal;
in vec3 camera_EyeDirection;
in vec3 camera_LightDirection;

out vec4 colour;

uniform sampler2D textureSampler;
uniform mat4 ModelView;
uniform vec3 world_LightPos;

void main() {
    vec3 lightColour = vec3(1, 1, 1);
    float lightIntensity = 50.0f;

    vec3 matDiffuseColour = texture(textureSampler, UV).rgb;
    vec3 matAmbientColour = vec3(0.1, 0.1, 0.1) * matDiffuseColour;
    vec3 matSpecularColour = vec3(0.3, 0.3, 0.3);

    float distance = length(world_LightPos - world_Position);

    vec3 normal = normalize(camera_Normal);
    vec3 lightDirection = normalize(camera_LightDirection);

    float cosTheta = clamp(dot(normal, lightDirection), 0, 1);

    vec3 eyeDirection = normalize(camera_EyeDirection);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float cosAlpha = clamp(dot(eyeDirection, reflectionDirection), 0, 1);

    colour.rgb =
        // Ambient
        matAmbientColour +
        // Diffuse
        matDiffuseColour * lightColour * lightIntensity * cosTheta / (distance*distance) +
        // Specular
        matSpecularColour * lightColour * lightIntensity * pow(cosAlpha, 5) / (distance*distance);
    colour.a = 1;
}
