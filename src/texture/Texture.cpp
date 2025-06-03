//
// Created by faliszewskii on 06.05.24.
//

#include "Texture.h"
#include "../Shader.h"
#include "common.h"

Texture::Texture(const std::string& path) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int width, height, nrChannels;
    unsigned char *data = TextureCommon::loadImage(path.c_str(), &width, &height, &nrChannels, 0);

    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            nrChannels==4 ? GL_SRGB_ALPHA : GL_SRGB,
            width,
            height,
            0,
            nrChannels==4 ? GL_RGBA : GL_RGB,
            GL_UNSIGNED_BYTE,
            data
    );
    glGenerateMipmap(id);
    TextureCommon::freeImage(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind(int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture::Texture (float *data, unsigned int width, unsigned int height)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Upload the initial data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void Texture::updateTexture (float* data, unsigned int width, unsigned int height)
{
    // Update the texture
    glBindTexture(GL_TEXTURE_2D, id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, data);
}

