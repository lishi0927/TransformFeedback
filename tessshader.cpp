#include "shader.h"
#include "tessshader.h"


bool TessShader::Init()
{
	shader = LoadTessShaders("shader/vert.glsl", "shader/tess_cs.glsl", "shader/tess_es.glsl", "shader/frag.glsl");
	
	m_WVLocation = glGetUniformLocation(shader, "gWVMatrix");
	m_ProjectionMatrixLocation = glGetUniformLocation(shader, "gProjMatrix");
	m_distexLocation = glGetUniformLocation(shader, "distex");
	m_colorLocation = glGetUniformLocation(shader, "colortex");
	m_normalLocation = glGetUniformLocation(shader, "normaltex");
	m_cameraposLocation = glGetUniformLocation(shader, "cameraPos");
	m_WMLocation = glGetUniformLocation(shader, "gWorldMatrix");

	return true;
}

void TessShader::SetWV(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_WVLocation, 1, GL_FALSE, &WVP[0][0]);
}

void TessShader::SetProjectionMatrix(const glm::mat4 &WorldMatrix)
{
	glUniformMatrix4fv(m_ProjectionMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
}

void TessShader::SetColorTex(unsigned int TextureUnit)
{
	glUniform1i(m_colorLocation, TextureUnit);
}

void TessShader::SetNormalTex(unsigned int TextureUnit)
{
	glUniform1i(m_normalLocation, TextureUnit);
}

void TessShader::SetDisTex(unsigned int TextureUnit)
{
	glUniform1i(m_distexLocation, TextureUnit);
}

void TessShader::SetWorldMatrix(const glm::mat4 &WorldMatrix)
{
	glUniformMatrix4fv(m_WMLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
}

void TessShader::SetCameraPos(const glm::vec3 &camera)
{
	glUniform3f(m_cameraposLocation, camera.x,camera.y,camera.z);
}

bool OriShader::Init()
{
	shader = LoadShaders("shader/orivert.glsl", "shader/orifrag.glsl");

	m_WVPLocation = glGetUniformLocation(shader, "gWVPMatrix");

	m_colorLocation = glGetUniformLocation(shader, "colortex");

	m_cameraposLocation = glGetUniformLocation(shader, "cameraPos");

	m_WMLocation = glGetUniformLocation(shader, "gWorldMatrix");
	
	return true;
}

void OriShader::SetWVP(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, &WVP[0][0]);
}

void OriShader::SetColorTex(unsigned int TextureUnit)
{
	glUniform1i(m_colorLocation, TextureUnit);
}

void OriShader::SetWorldMatrix(const glm::mat4 &WorldMatrix)
{
	glUniformMatrix4fv(m_WMLocation, 1, GL_FALSE, &WorldMatrix[0][0]);
}

void OriShader::SetCameraPos(const glm::vec3 &camera)
{
	glUniform3f(m_cameraposLocation, camera.x, camera.y, camera.z);
}