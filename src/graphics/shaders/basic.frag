#version 330 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec3 gPosition;

uniform vec4 colour;
uniform vec3 light_pos;
uniform vec3 camera_position;
uniform sampler2D ourTexture;

in DATA
{
    vec4 position;
    vec4 color;
    vec4 ambient;
    vec3 normal;
    vec2 texCoord;
}
fragment;

vec4 rgb(float r, float g, float b) {
	return vec4(r/255, g/255, b/255, 1);
}

void main() {
    vec4 white = rgb(236, 240, 241); //light color
	vec4 purple = rgb(142, 68, 173);
    vec4 orange = vec4(0.5, 0.2, 0.2, 1);
	vec4 midnightBlue = rgb(44, 62, 80);
	
    //diffuse lighting
    vec3 norm = normalize(fragment.normal);
    vec3 lightDir = normalize(light_pos - vec3(fragment.position));
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * white * 0.5;
	
	//direction light
	vec3 surfaceToLight = normalize(vec3(50, 1000, 50));
	diff = dot(norm, surfaceToLight);
	vec4 sun = diff * white  * 0.7f;
	
    //specular lighting
    diff = diff > 0 ? 1 : 0;
    
    float specularStrength = 0.8f;
    vec3 viewDir = normalize(camera_position - vec3(fragment.position));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = specularStrength * spec * white * diff;
    
    //calculating color
	vec4 totalLight = (fragment.ambient + sun + diffuse);//  + specular);
	vec4 texturePixel = texture(ourTexture, fragment.texCoord);
	
	color = midnightBlue * totalLight;
    //color = texturePixel * totalLight;
	//gPosition = vec3(fragment.position);

}



