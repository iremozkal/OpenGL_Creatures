#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include "mat.h"

class Mesh
{
public:
	Mesh();
	void LoadObjModel(const char *filename);
	std::vector<vec3> returnVertices();
	std::vector<vec3> returnNormals();
private:
	std::vector < vec3 > out_vertices;
	std::vector < vec2 > out_uvs;
	std::vector < vec3 > out_normals;

};

