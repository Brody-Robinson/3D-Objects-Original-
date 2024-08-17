#version 400 core

in vec3 Position;
in vec2 Tex_coord;
in vec3 normals;
in vec4 ModelPosition;

out vec4 color;

uniform sampler2D map;
uniform float Height;

struct pointlight
{
	vec3 specular;
	vec3 diffuse;
	vec3 position;
	vec3 ambient;
};

uniform pointlight pointLight;
uniform vec3 eye; 

float interpol(float min, float max, float curr)
{
    float Difference = curr - min;

    return Difference / (max - min);
}

void main()
{
   
        vec3 lightDirection = pointLight.position - ModelPosition.xyz;

		vec3 ambient = pointLight.ambient; 

		vec3 norm = normals.xyz;

		vec3 N = normalize(norm);
		vec3 L = normalize(lightDirection);

		vec3 diffuse = max(dot(N, L), 0.0) * pointLight.diffuse;

		vec3 V = normalize(eye - ModelPosition.xyz);
		vec3 R = reflect(-L, N);

		vec3 specular = pow(max(dot(V, R), 0.0), 64.0f) * pointLight.specular;

		vec3 result_norm = (ambient + diffuse);
    

		color = ( texture(map, Tex_coord) *  vec4(result_norm, 1.0f) );
		//color = vec4(normals, 1.0f);
}