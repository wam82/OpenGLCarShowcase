#version 330

in vec3 vs_position;
in vec2 vs_texture_coordinate;
in vec3 vs_normal;

out vec4 fs_color;

uniform vec3 lightPosition0;
uniform vec3 cameraPosition;
uniform vec3 modifiedColor;
uniform vec3 lightColor;
uniform float opacity;

uniform bool hasIridescence;
uniform sampler2D iridescentTexture;

void main(){
    float ka = 0.1;
    float kd = 0.6;
    float ks = 1.0;
    float shininess = 2;

    //Ambient light
    vec3 ambientLight = vec3(1.f) * ka;

    //Diffuse light
    vec3 positionToLight = normalize(lightPosition0 - vs_position);
    vec3 diffuseColor = lightColor;
    float diffuse = clamp(dot(positionToLight, vs_normal), 0, 1);
    vec3 diffuseLight = diffuseColor * diffuse *kd;

    //Specular light
    vec3 lightToPosition = normalize(vs_position - lightPosition0);
    vec3 reflectedDirection = normalize(reflect(lightToPosition, normalize(vs_normal)));
    vec3 positionToView = normalize(cameraPosition - vs_position);
    float specularConstant = pow(max(dot(positionToView, reflectedDirection), 0), shininess);
    vec3 specularColor = lightColor * specularConstant * ks;

    vec3 iridescentColor = vec3(1.0);
    if(hasIridescence){
        iridescentColor = texture(iridescentTexture, vs_texture_coordinate).rgb;
    }

    fs_color = vec4(modifiedColor * iridescentColor, opacity) * (vec4(ambientLight, 1.f) + vec4(diffuseLight, 1.f) + vec4(specularColor, 1.f));
}