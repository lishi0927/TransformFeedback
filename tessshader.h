#ifndef BEZIERSHADER_H
#define BEZIERSHADER_H

#include "shaderuniform.h"
#include <glm/glm.hpp>

class TessShader : public ShaderUniform {
public:
	virtual bool Init();

	void SetWV(const glm::mat4 &MVMatrix);

	void SetProjectionMatrix(const glm::mat4 &ProjectionMatrix);

	void SetDisTex(unsigned int TextureUnit);

	void SetColorTex(unsigned int TextureUnit);

	void SetNormalTex(unsigned int TextureUnit);

	void SetCameraPos(const glm::vec3 &camera);

	void SetWorldMatrix(const glm::mat4 &ProjectionMatrix);

private:
	GLuint m_WVLocation;
	GLuint m_ProjectionMatrixLocation;
	GLuint m_distexLocation;
	GLuint m_colorLocation;
	GLuint m_normalLocation;
	GLuint m_cameraposLocation;
	GLuint m_WMLocation;
};

class OriShader : public ShaderUniform {
public:
	virtual bool Init();

	void SetWVP(const glm::mat4 &MVMatrix);

	void SetColorTex(unsigned int TextureUnit);

	void SetCameraPos(const glm::vec3 &camera);

	void SetWorldMatrix(const glm::mat4 &ProjectionMatrix);
private:
	GLuint m_WVPLocation;
	GLuint m_colorLocation;
	GLuint m_cameraposLocation;
	GLuint m_WMLocation;
};

#endif
