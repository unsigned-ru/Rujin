#include "RujinPCH.h"
#include "Texture.h"

rujin::Texture::Texture(GLuint textureId, const glm::ivec2& size)
	: m_TextureId(textureId)
	, m_Size(size)
{
}
rujin::Texture::~Texture()
{
	glDeleteTextures(1, &m_TextureId);
}
