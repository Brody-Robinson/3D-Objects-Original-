#pragma once
#include "error.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Light.h"
#include "torus.h"
#include "cylinder.h"

#include "renderer/opengleShader.h"
#include "renderer/buffers.h"
#include "renderer//vertexArray.h"
#include "renderer/opengltexture.h"

#include <iostream>

#include <string>

using std::string;
string lightVertex = R"(
	#version 400 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

	uniform mat4 model;
	uniform mat4 projection;
	uniform mat4 view;


out vec3 Normal;
out vec2 TexCoords;
out vec4 ModelPosition;

void main()
{
	vec4 modelPosition =  view * model * vec4(vertex, 1.0);

	ModelPosition = model * vec4(vertex, 1.0);
	Normal = vec4(transpose(inverse(model)) * vec4(normals, 1.0f)).xyz;

	
	TexCoords = texCoords.xy;

	gl_Position = projection * modelPosition;
}


)";


string lightFragment = R"(
	#version 400 core

struct Light
{
	vec3 position;
	vec3 color;
	vec3 direction;
	vec3 spotLightDirection;

	bool isDirection; // used to note if its a directional light 
	bool isPoint; // used to note if its a point light 
	bool isSpot; // used to note if its a spot light 

	float spotexponent;
	float cutoff;
	float innercutoff;
	float intensity;

	float kc; float kl; float kq;

	float ambientIntensity;

	float specularIntensity;
};


uniform Light[3] light;//the reason why thsi code doesnt work make sure to fix it when next ur editing
uniform vec3 viewPosition;


in vec3 Normal;
in vec2 TexCoords;
in vec4 ModelPosition;

out vec4 FragmentColor;

uniform sampler2D _MainTex;
uniform sampler2D texture_2;
uniform vec3 color;

vec3 DirectionslightFactor(vec3 normal, int index);
vec3 PointLightFactor(vec3 normal, int index);
vec3 spotLightFactor(vec3 normal, int index);

void main()
{
	vec3 result;
	vec3 result2;

	result = (spotLightFactor(Normal, 0));
	result2 = (spotLightFactor(Normal, 1));

	if(color != vec3(0.0f))
	{
		FragmentColor = vec4(color, 1.0f)  * vec4(result + result2, 1.0f);
	}
	else
	{
		vec4 f =  (texture(_MainTex, TexCoords.st) * 0.8f) + ( texture(texture_2, TexCoords.st) * 0.2f);
		FragmentColor =  f  * vec4(result, 1.0f);
	}

}


//..........................................................................................
vec3 PointLightFactor(vec3 normal, int index)
{
	if (light[index].isPoint)
	{
		vec3 lightDirection = light[index].position - ModelPosition.xyz;

		vec3 ambient = light[index].ambientIntensity * vec3(1.0f);

		vec3 norm = normal.xyz;

		float d = length(lightDirection);

		float attenuation =	1.0f /	(light[index].kc +
								   light[index].kl * d +
                                   light[index].kq * d * d);

		vec3 N = normalize(norm);
		vec3 L = normalize(lightDirection);

		vec3 diffuse = max(dot(N, L), 0.0) * light[index].intensity * light[index].color * attenuation;

		vec3 V = normalize(viewPosition - ModelPosition.xyz);
		vec3 R = reflect(-L, N);

		vec3 specular = pow(max(dot(V, R), 0.0), 4.0f) * light[index].color * attenuation;

		vec3 result = (ambient + diffuse + specular);
		return result;
	}
	else
	{
		return vec3(0.0f);
	}
}

//.......................................................
vec3 spotLightFactor(vec3 normal, int index)
{
	if(light[index].isSpot)
	{
		vec3 lightDirection = light[index].position - ModelPosition.xyz;

		vec3 spotLight = light[index].spotLightDirection;

		float d = length(lightDirection);

		float attenuation =	1.0f /	(light[index].kc +
								   light[index].kl * d +
                                   light[index].kq * d * d);

		vec3 ambient = light[index].ambientIntensity * vec3(1.0f);

		vec3 norm = normal.xyz;

		vec3 N = normalize(norm);
		vec3 L = normalize(lightDirection);
		vec3 S = normalize(spotLight);

		float theta = max(dot(-S, L), 0.0);
		
		if(theta > light[index].cutoff)
		{
			float spotattenuation = pow(smoothstep(light[index].cutoff, light[index].innercutoff, theta), light[index].spotexponent);
			float final = attenuation * spotattenuation;	

			vec3 diffuse = max(dot(N, L), 0.0) * light[index].intensity * light[index].color * final ;

			vec3 V = normalize(viewPosition - ModelPosition.xyz);
			vec3 R = reflect(L, N);

			vec3 specular = pow(max(dot(V, R), 0.0), light[index].specularIntensity) * light[index].color * final;

			vec3 result = (ambient + diffuse + specular);
			return result;
		}
		else
		{
			return ambient;
		}
	}
	else
	{
		return vec3(0.0f);
	}
}
)";



vertexBuffer PlaneBuffer;
vertexArray PlaneVao;
openglShader Shader1;

////////floor data///////////////////////////////////
vertexBuffer floorBuffer;
vertexArray floorVao;
_2DTex floorTexture;

unsigned int fpsize = 0;
unsigned int fsize2 = 0;
unsigned int fsize3 = 0;

int countf = 0;


Cylinder cylinder;

Cylinder cylinder_2;

Torus obj;

vertexArray VAO;
vertexBuffer vbo;
IndexBuffer ibo;

vertexArray VAO_2;
vertexBuffer vbo_2;
IndexBuffer ibo_2;


// Load data in VBO (Vertex vertexBuffer Object) and return the vbo's id
GLuint loadDataInBuffers()
{
	cylinder = Cylinder(1.0f, 0.0f, 0.9f, 36);

	obj = Torus(0.2f, 1.0f, 36.0f, 36.0f);
	obj._createTorus();
	std::vector<float>  vertex = obj.getVertices();
	std::vector<float>  normals = obj.getNormals();
	std::vector<float>  Tex = obj.GetTex();

	GLuint vboId;

	//set the vertices
	vbo = vertexBuffer( const_cast<float*>(cylinder.getVertices()),
		cylinder.getVertexSize() + obj.getVerticesSize()
		+ cylinder.getNormalSize() + obj.getNormalsSize() +
		cylinder.getTexCoordSize() + obj.getTexSize());

	vbo.Bind();
	vbo.updateBuffer(&(vertex[0]), obj.getVerticesSize(), cylinder.getVertexSize());
	
	//sets the normals
	vbo.updateBuffer(const_cast<float*>(cylinder.getNormals()),
		cylinder.getNormalSize(), cylinder.getVertexSize() + obj.getVerticesSize());

	vbo.updateBuffer(&(normals[0]), obj.getNormalsSize(),
		cylinder.getVertexSize() + cylinder.getNormalSize() + obj.getVerticesSize());

	//set the texcoords
	vbo.updateBuffer(const_cast<float*>(cylinder.getTexCoords()),
		cylinder.getTexCoordSize(), cylinder.getVertexSize() + obj.getVerticesSize() + obj.getNormalsSize() + cylinder.getNormalSize());

	vbo.updateBuffer(&(Tex[0]), obj.getTexSize(),
		cylinder.getVertexSize() + cylinder.getNormalSize() + obj.getVerticesSize() + obj.getNormalsSize() + cylinder.getTexCoordSize());


	//sets second cylinder
	cylinder_2 = Cylinder(1.0f, 1.0f, 0.9f, 36);

	vbo_2 = vertexBuffer(const_cast<float*>(cylinder_2.getVertices()),
		cylinder_2.getVertexSize() + cylinder_2.getNormalSize() + cylinder_2.getTexCoordSize());

	vbo_2.updateBuffer(const_cast<float*>(cylinder_2.getNormals()),
		cylinder_2.getNormalSize(), cylinder_2.getVertexSize());

	vbo_2.updateBuffer(const_cast<float*>(cylinder_2.getTexCoords()),
		cylinder_2.getTexCoordSize(), cylinder_2.getVertexSize() + cylinder_2.getNormalSize());


	return 0;
}

GLuint loadIndexes()
{
	ibo = IndexBuffer(const_cast<unsigned int*>(cylinder.getIndices()), cylinder.getIndexSize());
	ibo.Bind();

	ibo_2 = IndexBuffer(const_cast<unsigned int*>(cylinder_2.getIndices()), cylinder_2.getIndexSize());
	ibo_2.Bind();

	ibo.Bind();


	return 0;
}

inline GLuint loadCube()
{
	GLfloat vertices[] = {
		// box vertex	//normals
		// coordinates		
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    0.0f, 0.0f,//0
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    1.0f, 0.0f,//1
		 0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    1.0f, 1.0f,//2
		 0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    1.0f, 1.0f,//2
		-0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    0.0f, 1.0f,//3
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    0.0f, 0.0f,//0


		 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,  0.0f,    0.0f, 0.0f,//1
		 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,  0.0f,    1.0f, 0.0f,//4
		 0.5f,  0.5f, -0.5f,    1.0f, 0.0f,  0.0f,    1.0f, 1.0f,//6
		 0.5f,  0.5f, -0.5f,    1.0f, 0.0f,  0.0f,    1.0f, 1.0f,//6
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,  0.0f,    0.0f, 1.0f,//2
		 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,  0.0f,    0.0f, 0.0f,//1


		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    0.0f, 0.0f,


		-0.5f, -0.5f, -0.5f,   -1.0f, 0.0f,  0.0f,    0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,   -1.0f, 0.0f,  0.0f,    1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   -1.0f, 0.0f,  0.0f,    1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   -1.0f, 0.0f,  0.0f,    1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,   -1.0f, 0.0f,  0.0f,    0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   -1.0f, 0.0f,  0.0f,    0.0f, 0.0f,


		-0.5f, -0.5f,  0.5f,    0.0f,-1.0f,  0.0f,    0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f,-1.0f,  0.0f,    1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    0.0f,-1.0f,  0.0f,    1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    0.0f,-1.0f,  0.0f,    1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,-1.0f,  0.0f,    0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,-1.0f,  0.0f,    0.0f, 0.0f,


		-0.5f,  0.5f,  0.5f,    0.0f, 1.0f,  0.0f,    0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f, 1.0f,  0.0f,    1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f, 1.0f,  0.0f,    1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,    0.0f, 1.0f,  0.0f,    1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,  0.0f,    0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 1.0f,  0.0f,    0.0f, 0.0f,

	};

	PlaneBuffer = vertexBuffer(vertices, sizeof(vertices));
	PlaneBuffer.Bind();
	return PlaneBuffer.getRendererID();
}




// Initialize and put everything together
inline void init()
{
	// clear the framebuffer each frame with black color
	glClearColor(0, 0, 0.0f, 0);
	
	loadCube();
	loadDataInBuffers();
	loadIndexes();

	VAO = vertexArray(true);

	//create shader
	Shader1 = openglShader(lightVertex, lightFragment);
	Shader1.Bind();

	floorTexture = _2DTex("asset/floor.jpeg", "floor");
	floorTexture.bindTextureToSlot(1); 

	BufferLayout generalLayout;

	floorVao = vertexArray(true);
	floorVao.Bind();

	generalLayout.flush();
	generalLayout.push(3, DataType::FLOAT, false, 0);
	generalLayout.push(3, DataType::FLOAT, false, (const void*)fpsize);
	generalLayout.push(2, DataType::FLOAT, false, (const void*)(fpsize + fsize2));

	floorVao.addBuffer(generalLayout, floorBuffer);

	PlaneVao = vertexArray(true);
	PlaneVao.Bind();
	PlaneBuffer.Bind();

	generalLayout.flush();
	generalLayout.push(3, DataType::FLOAT, false);
	generalLayout.push(3, DataType::FLOAT, false);
	generalLayout.push(2, DataType::FLOAT, false);


	PlaneVao.addBuffer(generalLayout, PlaneBuffer);


	VAO.Bind();
	vbo.Bind();
	generalLayout.flush();
	generalLayout.push(3, DataType::FLOAT, false, 0);
	generalLayout.push(3, DataType::FLOAT, false, (const void*)(cylinder.getVertexSize() + obj.getVerticesSize()));
	generalLayout.push(2, DataType::FLOAT, false, (const void*)(cylinder.getVertexSize() + obj.getVerticesSize() + obj.getNormalsSize() + cylinder.getNormalSize()));

	VAO.addBuffer(generalLayout, vbo);


	VAO_2 = vertexArray(true);
	VAO_2.Bind();
	vbo_2.Bind();
	generalLayout.flush();
	generalLayout.push(3, DataType::FLOAT, false, 0);
	generalLayout.push(3, DataType::FLOAT, false, (const void*)(cylinder_2.getVertexSize()));
	generalLayout.push(2, DataType::FLOAT, false, (const void*)(cylinder_2.getVertexSize() + cylinder_2.getNormalSize()));


	VAO_2.addBuffer(generalLayout, vbo_2);

	
	GLcall(glDisable(GL_CULL_FACE));

    GLcall(glEnable(GL_DEPTH_TEST));
	GLcall(glDepthFunc(GL_LEQUAL));
	GLcall(glDepthRange(0.0f, 1.0f));
	GLcall(glDepthMask(1));


	//GLcall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)); 
}



