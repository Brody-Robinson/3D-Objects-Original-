
#include "glm/common.hpp"
#include "glm/matrix.hpp"

#include <math.h>
#include <iostream>

#include "Torus.h"

#define M_PI 3.14159265358979323846f

Torus::Torus(float r, float R, int nr, int nR)
	:_r(r), _R(R), _nr(nr), _nR(nR)
{
	// create geometry 
	
}

Torus::~Torus()
{

}



void Torus::_createTorus()
{
	// clear memory allocated for vertex data 
	_tangents.clear();

	// A torus is given by the paramteric equations:
	// x = (R + r cos(v))cos(u)
	// y = (R + r cos(v))sin(u)
	// z = r sin(v)
	// u, v => [0, 2pi]


	float du = 2 * M_PI / _nR;
	float dv = 2 * M_PI / _nr;

	for (size_t i = 0; i < _nR; i++) {

		float u = i * du;

		for (size_t j = 0; j <= _nr; j++) {

			float v = (j % _nr) * dv;

			for (size_t k = 0; k < 2; k++)
			{
				float uu = u + k * du;
				// compute vertex
				float x = (_R + _r * cos(v)) * cos(uu);
				float y = (_R + _r * cos(v)) * sin(uu);
				float z = _r * sin(v);

				// add vertex 
				_vertices.push_back(x);
				_vertices.push_back(y);
				_vertices.push_back(z);

				// compute normal 
				float nx = cos(v) * cos(uu);
				float ny = cos(v) * sin(uu);
				float nz = sin(v);

				// add normal 
				_normals.push_back(nx);
				_normals.push_back(ny);
				_normals.push_back(nz);

				// compute texture coords
				float tx = uu / (2 * M_PI);
				float ty = v / (2 * M_PI);

				// add tex coords
				_texCoords.push_back(tx);
				_texCoords.push_back(ty);

				// std::cout << tx << "," << ty << std::endl;

				// add tangent vector
				// T = d(S)/du 
				// S(u) is the circle at constant v
				glm::vec3 tg(-(_R + _r * cos(v)) * sin(uu),
					(_R + _r * cos(v)) * cos(uu),
					0.0f
				);
				tg = glm::normalize(tg);
				_tangents.push_back(tg.x);
				_tangents.push_back(tg.y);
				_tangents.push_back(tg.z);

				//std::cout << tg.x << "," << tg.y << "," << tg.z << std::endl;
			}

			// incr angle
			v += dv;

		}
	}

	vertexCount = _vertices.size() / 3;
}

