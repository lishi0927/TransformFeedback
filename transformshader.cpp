#include "shader.h"
#include "transformshader.h"

bool TransformShader::Init()
{
	shader = LoadTessShaders("shader/transformvert.glsl","shader/transformcs.glsl","shader/transformes.glsl", "shader/transformfrag.glsl");

	//m_WVPLocation = glGetUniformLocation(shader, "gWVPMatrix");

	//m_colorLocation = glGetUniformLocation(shader, "colortex");

	m_distexLocation = glGetUniformLocation(shader, "distex");

	return true;
}


void TransformShader::SetWVP(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, &WVP[0][0]);
}

void TransformShader::SetColorTex(unsigned int TextureUnit)
{
	glUniform1i(m_colorLocation, TextureUnit);
}

void TransformShader::SetDisTex(unsigned int TextureUnit)
{
	glUniform1i(m_distexLocation, TextureUnit);
}