#include "Mesh.h"
#include <fstream>
#include <string>

using namespace std;

Mesh::Mesh()
{
	m_vertices.clear();
	m_coordinates.clear();
	m_normals.clear();
	m_facenumber = 0;
}

Mesh::~Mesh()
{
	m_vertices.clear();
	m_coordinates.clear();
	m_normals.clear();
}

void Mesh::LoadMesh(const char* filename)
{
	fstream f(filename);
	char line[256];
	float f1, f2, f3;
	while (!f.getline(line,255).eof())
	{
		line[255] = '\0';
		if (line[0] == 'v')
		{
			if (sscanf(line, "v %f %f %f", &f1, &f2, &f3) == 3)
			{
				this->m_vertices.push_back(glm::vec3(f1, f2, f3));
			}
			else if (sscanf(line, "vt %f %f", &f1, &f2) == 2)
			{
				this->m_coordinates.push_back(glm::vec2(f1, f2));
			}
			else if (sscanf(line, "vn %f %f %f", &f1, &f2, &f3) == 3)
			{
				this->m_normals.push_back(glm::vec3(f1, f2, f3));
			}
		}
		else if (line[0] == 'f')
		{
			int i11, i12, i13, i21, i22, i23, i31, i32, i33, i41, i42, i43;
			if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &i11, &i12, &i13, &i21, &i22, &i23, &i31, &i32, &i33, &i41, &i42, &i43) == 12)
			{
				this->m_faces.push_back(glm::ivec3(i11, i12, i13));
				this->m_faces.push_back(glm::ivec3(i21, i22, i23));
				this->m_faces.push_back(glm::ivec3(i31, i32, i33));

				this->m_faces.push_back(glm::ivec3(i11, i12, i13));
				this->m_faces.push_back(glm::ivec3(i31, i32, i33));
				this->m_faces.push_back(glm::ivec3(i41, i42, i43));
			}
			else if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &i11, &i12, &i13, &i21, &i22, &i23, &i31, &i32, &i33) == 9)
			{
				this->m_faces.push_back(glm::ivec3(i11, i12, i13));
				this->m_faces.push_back(glm::ivec3(i21, i22, i23));
				this->m_faces.push_back(glm::ivec3(i31, i32, i33));
			}
			else if (sscanf(line, "f %d/%d %d/%d %d/%d", &i11, &i12, &i21, &i22, &i31, &i32) == 6)
			{
				this->m_faces.push_back(glm::ivec3(i11, i12, -1));
				this->m_faces.push_back(glm::ivec3(i21, i22, -1));
				this->m_faces.push_back(glm::ivec3(i31, i32, -1));
			}
			else if (sscanf(line, "f %d %d %d", &i11, &i21, &i31) == 3)
			{
				this->m_faces.push_back(glm::ivec3(i11, -1, -1));
				this->m_faces.push_back(glm::ivec3(i21, -1, -1));
				this->m_faces.push_back(glm::ivec3(i31, -1, -1));
			}
			else if (sscanf(line, "f %d//%d %d//%d %d//%d", &i11, &i13, &i21, &i23, &i31, &i33) == 6)
			{
				this->m_faces.push_back(glm::ivec3(i11, -1, i13));
				this->m_faces.push_back(glm::ivec3(i21, -1, i23));
				this->m_faces.push_back(glm::ivec3(i31, -1, i33));
			}
		}
	}

	m_facenumber = m_faces.size() / 3;
}


void SaveObj(std::vector<GLfloat> result)
{
	ofstream out("output.obj");
	for (int i = 0; i < result.size() / 3; i++)
	{
		out << "v " << result[3 * i] << " " << result[3 * i + 1] << " " << result[3 * i + 2] << std::endl;
		if(i % 3 == 2)
		out << "f " << i - 1 << " " << i + 1 << " " << i << std::endl;
	}
	out.close();
}