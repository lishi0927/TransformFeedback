#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Mesh {
public:
	Mesh();
	~Mesh();

	void LoadMesh(const char* filename);

	int m_facenumber;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_coordinates;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::ivec3> m_faces;
};

void SaveObj(std::vector<GLfloat> result);

#endif
