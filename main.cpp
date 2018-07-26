#include "Mesh.h"
#include <windows.h>

#include <iostream>
#include <vector>
// Include GLEW
#include <GL/glew.h>
#include <GL/gl.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

#include "camera.h"
#include "tessshader.h"
#include "texture.h"

#include "transformshader.h"

bool keys[1024];
Camera m_camera;
TessShader m_shader;
Mesh m_basemesh;
GLuint disTex, NormalTex;
GLuint vertexbuffer, uvbuffer, transformbuffer;

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> basenormals;
std::vector<GLfloat> feedback;

TransformShader m_transform;

unsigned int m_currVB = 0;

int verticesnum = 0;

GLuint query;


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 720

#define TextureWidth 512
#define TextureHeight 512

#define MAXVERTICES 16

void TransformFeedback()
{
	m_basemesh.LoadMesh("model/ninjaHead_Low.obj");
	disTex = loadDDS("texture/dis.dds");
	NormalTex = loadDDS("texture/normal.dds");

	verticesnum = m_basemesh.m_facenumber * 3;
	vertices.reserve(MAXVERTICES * verticesnum);

	for (int i = 0; i < m_basemesh.m_facenumber; i++)
	{
		glm::vec2 vert_coord1 = m_basemesh.m_coordinates[m_basemesh.m_faces[i * 3].g - 1];
		glm::vec2 vert_coord2 = m_basemesh.m_coordinates[m_basemesh.m_faces[i * 3 + 1].g - 1];
		glm::vec2 vert_coord3 = m_basemesh.m_coordinates[m_basemesh.m_faces[i * 3 + 2].g - 1];

		glm::vec3 vert_pos1 = m_basemesh.m_vertices[m_basemesh.m_faces[i * 3].r - 1];
		glm::vec3 vert_pos2 = m_basemesh.m_vertices[m_basemesh.m_faces[i * 3 + 1].r - 1];
		glm::vec3 vert_pos3 = m_basemesh.m_vertices[m_basemesh.m_faces[i * 3 + 2].r - 1];

		vertices.push_back(vert_pos1);
		vertices.push_back(vert_pos2);
		vertices.push_back(vert_pos3);

		uvs.push_back(vert_coord1);
		uvs.push_back(vert_coord2);
		uvs.push_back(vert_coord3);

		glm::vec3 normal = glm::normalize(glm::cross(vert_pos2 - vert_pos1, vert_pos3 - vert_pos1));

		if (m_basemesh.m_normals.size() == 0)
		{
			basenormals.push_back(normal);
			basenormals.push_back(normal);
			basenormals.push_back(normal);
		}
		else
		{
			basenormals.push_back(m_basemesh.m_normals[m_basemesh.m_faces[i * 3].b - 1]);
			basenormals.push_back(m_basemesh.m_normals[m_basemesh.m_faces[i * 3 + 1].b - 1]);
			basenormals.push_back(m_basemesh.m_normals[m_basemesh.m_faces[i * 3 + 2].b - 1]);
		}
	}

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vertexbuffer);
	glGenTransformFeedbacks(1, &transformbuffer);

	
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, MAXVERTICES * vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vertexbuffer);
	

}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		keys[key] = true;
	else
		if (action == GLFW_RELEASE)
			keys[key] = false;

	if (keys[GLFW_KEY_ESCAPE])
		exit(0);
}

void mousecallback(GLFWwindow* window, double xpos, double ypos) {
	if (xpos != 0 && ypos != 0)
		m_camera.SetViewByMouse(xpos, ypos);

	glfwSetCursorPos(window, 0, 0);
}

void init()
{
	vertices.clear();
	uvs.clear();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	TransformFeedback();

	m_camera.SetPerspective(glm::radians(45.0f), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01, 100000);
	//m_camera.PositionCamera(-400, 1000, 1800, glm::radians(0.0f), glm::radians(30.0f));
	m_camera.PositionCamera(25, 180, 45, glm::radians(0.0f), glm::radians(0.0f));

	
	GLint MaxPatchVertices = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
	printf("Max supported patch vertices %d\n", MaxPatchVertices);
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	m_transform.Init();

	glGenQueries(1, &query);
}


void TransformRender()
{
	GLenum err = glGetError();
	m_transform.Enable();
	m_transform.SetDisTex(0);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, disTex);

	err = glGetError();

	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformbuffer);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);

	glBeginTransformFeedback(GL_TRIANGLES);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);


	glDrawArrays(GL_PATCHES, 0, verticesnum);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glEndTransformFeedback();
	glUseProgram(0);

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

	GLuint primitives;
	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitives);

	glDisable(GL_RASTERIZER_DISCARD);

	err = glGetError();
	feedback.resize(primitives * 3 * 3);

	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedback.size() * sizeof(GLfloat), feedback.data());


	SaveObj(feedback);
}


int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bezier Patch", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
							 // Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mousecallback);

	//vsync

	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

	//wglSwapIntervalEXT(1);
	wglSwapIntervalEXT(0);

	memset(keys, 0, sizeof(keys));

	init();

	GLfloat lastTime = glfwGetTime();
	GLfloat currentTime;

	glfwSetCursorPos(window, 1024 / 2, 720 / 2);

	TransformRender();

	do {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		currentTime = glfwGetTime();

		m_camera.CameraKeyMove(keys, currentTime - lastTime);

		lastTime = currentTime;

		// Swap buffers
		glfwSwapBuffers(window);

	} // Check if the ESC key was pressed or the window was closed
	while (glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}