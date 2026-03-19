#include "Utils/GLUtils.h"

#include "glad/gl.h"
#include "stb_image.h"
#include "Utils/Log.h"

namespace GLUtils {
	uint32_t LoadTexture(const char* path) {
		int width, height, channels;
		stbi_uc* data = stbi_load(path, &width, &height, &channels, 0);
		if (!data) {
			LOG_ERROR("Failed to load texture: {0}", path);
			return 0;
		}

		LOG_INFO("Loaded texture '{0}' (width: {1}, height: {2}, channels: {3})", path, width, height, channels);

		uint32_t texture;
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);

		GLint internalFormat = GL_RGB;
		if (channels == 4)
			internalFormat = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		return texture;
	}
}
