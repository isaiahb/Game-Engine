// Fragment shader:
// ================
#version 330 core
struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};
/* Note: because we now use a material struct again you want to change your
 mesh class to bind all the textures using material.texture_diffuseN instead of
 texture_diffuseN. */

struct PointLight {
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 2

in vec3 fragPosition;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform vec3 light_pos;
uniform vec3 camera_position;

// Function prototypes
vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 getColor();
vec4 rgb(float r, float g, float b) { return vec4(r/255, g/255, b/255, 1);}

void main()
{
	vec3 result;
	vec3 viewDir = normalize(viewPos - fragPosition);
	vec3 norm = normalize(Normal);
	
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], material, norm, fragPosition, viewDir);
	
	vec4 light = rgb(0.5, 0.5, 0.5);
	color = vec4(vec3(texture(material.texture_specular1, TexCoords)), 1);
	//color = getColor();

}

vec4 getColor() {
	vec4 white = rgb(236, 240, 241); //light color
	//vec3 light_pos = vec3(0, 0, 0);
	//vec3 camera_position = vec3(0, 0, 0);
	
	//diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light_pos - vec3(fragPosition));
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * white;
	
	//specular lighting
	diff = diff > 0 ? 1 : 0;
	
	float specularStrength = 0.8f;
	vec3 viewDir = normalize(camera_position - vec3(fragPosition));
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 111110);
	vec4 specular = specularStrength * spec * white * diff;
	
	//calculating color
	vec4 totalLight = (vec4(0.2, 0.2, 0.2, 1) + diffuse + specular);
	vec4 texturePixel = texture(material.texture_specular1, TexCoords);
	
	return texturePixel * totalLight;

}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// Combine results
	vec3 ambient = light.ambient * vec3(texture(mat.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(mat.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(mat.texture_specular1, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
