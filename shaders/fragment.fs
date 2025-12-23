#version 400 core

in vec3 worldPos;
in vec3 worldNormal;
in vec2 uv;

out vec4 FragColor;

uniform sampler2D texture0;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightRadius;

uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;


uniform vec3 cameraPos;
void main()
{
    vec3 texColor = texture(texture0, uv).rgb;

    vec3 normal = normalize(worldNormal);
    vec3 lightDirection = normalize(lightPos-worldPos);

    float distance = length(lightPos-worldPos);
    float d = 1 - distance / lightRadius;

    float attenuation = 1.0 / (1.0 + 0.5 * d + 0.25 * d * d);
    attenuation = clamp(d, 0.0, 1.0);

    float diffuse = max(dot(normal,lightDirection),0.0);//*vec3(1.0,1.0,1.0);

    vec3 ambient = ambientStrength * texColor;//*ambientLightColor

    vec3 viewDir = normalize(cameraPos - worldPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    //vec3 halfDir = normalize(lightDirection+viewDir)
    //float spec = pow(max(dot(normal, halfDir), 0.0), shininess);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * vec3(1.0);
    //specular *= attenuation;
    vec3 lighting = ambient +(diffuse * texColor + specular) * attenuation*lightColor;
    //vec3 lighting = ambient + ((diffuse+specular)*lightColor)/attenuation;

    //vec3 finalColor =  lighting; //+ambient;
    if(distance>lightRadius){
    lighting = ambient;
    }
    FragColor = vec4(lighting, 1.0);
}