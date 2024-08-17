#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

class Torus
{

public:

	// constructor, etc.
	Torus() {}
	Torus(float r, float R, int nr, int nR);
	~Torus();

	// METHODS 
	inline int getCount() { return vertexCount; }
	void _createTorus();

	inline vector<float> getVertices() { return _vertices; }
	inline unsigned int getVerticesSize() { return _vertices.size() * sizeof(float); }
	
	inline vector<float> getNormals() { return _normals; }
	inline unsigned int getNormalsSize() { return _normals.size() * sizeof(float); }

	inline vector<float> GetTex() { return _texCoords; }
	inline unsigned int getTexSize() { return _texCoords.size() * sizeof(float); }
private:
	// DATA 

	// torus parameters
	float _r = 0.5f;
	float _R = 1.0f;
	int _nr = 16;
	int _nR = 16;

	unsigned int vertexCount;
	// tangent 
	vector<float> _tangents;
	vector<float> _vertices;
	vector<float> _normals;
	vector<float> _texCoords;
};

