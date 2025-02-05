#version 330 core

struct Material
{

	sampler2D diffuse ;
	sampler2D specular;
	float shininess;

};


struct Lighting
{

vec3 direction;
vec3 position;
float Cutoff;
float outerCutoff ;

	vec3 ambient ;
	vec3 diffuse ;
	vec3 specular;

    float constant;
    float linear;
      float quadratic;


};

out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


uniform vec3 viewPos;

uniform Material material;
uniform Lighting light;



void main()
{
	  // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // Diffuse
    vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    float theta = dot(lightDir,normalize(-light.direction));

    float epsilon = (light.Cutoff -light.outerCutoff);
    float intensity = clamp((theta-light.outerCutoff )/ epsilon,0.0f,1.0f);

    diffuse*= intensity;
    specular *= intensity;

    //Attenuation
    float distance = length(light.position - FragPos);
    float attenuation =   1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

       ambient  *=attenuation;
 diffuse  *= attenuation;
    specular *= attenuation;

    color = vec4(ambient + diffuse + specular, 1.0f);

}




