#ifndef TRANSFORMSHADER_H
#define TRANSFORMSHADER_H

#include "shaderuniform.h"
#include <glm/glm.hpp>

class TransformShader : public ShaderUniform {
public:
	virtual bool Init();

	void SetWVP(const glm::mat4 &MVMatrix);

	void SetColorTex(unsigned int TextureUnit);

	void SetDisTex(unsigned int TextureUnit);

private:
	GLuint m_WVPLocation;
	GLuint m_colorLocation;
	GLuint m_distexLocation;
};

#endif
