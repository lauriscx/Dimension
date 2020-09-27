struct BaseLight{
	float intensivity;//how intensive light are
	vec3 color;//r, g, b
};
struct DirectionLight {
	BaseLight light;
	vec3 direction;//x, y, z
};
struct PointLight {
	BaseLight baselight;
	vec3 position;//x, y, z
	
	float constant;
	float linear;
	float quadratic;
	
	float range;//distance light effect
};
struct SpotLight {
	PointLight pointlight;
	vec3 direction;//x, y, z
	
	float outerCutOff;
	float innerCutOff;
};


struct Material {
	vec4 color;
	vec2 specular;
};

vec3 Normal(sampler2D map, vec2 uv) {
	return normalize((2.0 * texture2D(map, uv) - 1.0).rgb);
}
float Attenuation(float constant, float linear, float qoudratic, float distance){
	return (1 / (constant + linear * distance + qoudratic * dot(distance, distance)));
}

vec3 CalcDirectionLight(DirectionLight light, Material material, vec3 normal, vec3 eyedirection, mat3 tangentspace){
	vec3 Dir = normalize(tangentspace * light.direction);

	float brightness = max(dot(normal, Dir), 0.0);
	//specular light
	float specular = 0;
	if(material.specular.y > 0.01 && material.specular.x > 0.01) {
		specular = dot(-eyedirection, reflect(Dir, normal));
		specular = pow(max(specular, 0.0), material.specular.y) * material.specular.x;
	}

	return light.light.color * light.light.intensivity * brightness + light.light.color * specular;
}

/*vec3 SpotLight(Light light1, vec3 direction, vec3 normal, vec3 eyeDirection, float attenuation, mat3 Tangentspace, Material material) {
	
	vec3 color = vec3(0, 0, 0);
	if(light1.cut.x > 0 && light1.cut.y > 0) {
		float theta = dot(direction, normalize(-(Tangentspace * light1.direction).xyz));
		float epsilon = (light1.cut.x - light1.cut.y);
		float intensity = clamp((theta - light1.cut.y) / epsilon, 0.0, 1.0);
		color = light1.color * intensity * dot(direction, normal) * attenuation;
		
		float specular = dot(-eyeDirection, reflect(direction, normal));
			specular = pow(max(specular, 0.0), material.specular.y) * material.specular.x;
			color += light1.color * specular * attenuation * intensity;
	}
	return color;
}*/


vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragment, vec3 eyedirection, mat3 tangentspace) {
	vec3 Dir = ((tangentspace * light.position) - fragment);
	//normal = normalize(tangentspace * normal);
	float Distance = length(Dir);
	float attenuation = Attenuation(light.constant, light.linear, light.quadratic, Distance);
	
	if(light.range < attenuation) {
		vec3 LightDir = normalize(Dir);
		//vec3 EyeDir = tangentspace * eyedirection;
		//point light
		float brightness = max(dot(normal, LightDir), 0.0);
		//specular light
		float specular = 0;
		if(material.specular.y > 0.01 && material.specular.x > 0.01) {
			specular = dot(-eyedirection, reflect(LightDir, normal));
			specular = pow(max(specular, 0.0), material.specular.y) * material.specular.x;
		}
		
		return light.baselight.color * light.baselight.intensivity * attenuation * brightness + light.baselight.color * specular * attenuation;
	} else {
		return vec3(0, 0, 0);
	}
}