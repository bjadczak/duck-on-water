//
// Created by faliszewskii on 06.05.24.
//

#include "Texture.h"
#include "../Shader.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb/stb_image.h"
//
//Texture::Texture(const std::string& path) {
//    glGenTextures(1, &id);
//    glBindTexture(GL_TEXTURE_2D, id);
//
//    int width, height, nrChannels;
//    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
//
//    glTexImage2D(
//            GL_TEXTURE_2D,
//            0,
//            nrChannels==4 ? GL_SRGB_ALPHA : GL_SRGB,
//            width,
//            height,
//            0,
//            nrChannels==4 ? GL_RGBA : GL_RGB,
//            GL_UNSIGNED_BYTE,
//            data
//    );
//    glGenerateMipmap(id);
//    stbi_image_free(data);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//}

void Texture::bind(int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture::Texture (float *data, unsigned int width, unsigned int height)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

//    // Allocate a contiguous block of memory
//    float* contiguousData = new float[width * height * 4]; // 4 channels for RGBA
//
//    // Copy the data to the contiguous block
//    for (int y = 0; y < height; ++y)
//    {
//        for (int x = 0; x < width; ++x)
//        {
//            contiguousData[(y * width + x) * 4 + 0] = data[y][x][0]; // Red
//            contiguousData[(y * width + x) * 4 + 1] = data[y][x][1]; // Green
//            contiguousData[(y * width + x) * 4 + 2] = data[y][x][2]; // Blue
//            contiguousData[(y * width + x) * 4 + 3] = data[y][x][3]; // Alpha
//        }
//    }
    // Upload the initial data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    delete[] contiguousData;

}

void Texture::updateTexture (float* data, unsigned int width, unsigned int height)
{

//    // Allocate a contiguous block of memory
//    float* contiguousData = new float[width * height * 4]; // 4 channels for RGBA
//
//    // Copy the data to the contiguous block
//    for (int y = 0; y < height; ++y)
//    {
//        for (int x = 0; x < width; ++x)
//        {
//            contiguousData[(y * width + x) * 4 + 0] = data[y][x][0]; // Red
//            contiguousData[(y * width + x) * 4 + 1] = data[y][x][1]; // Green
//            contiguousData[(y * width + x) * 4 + 2] = data[y][x][2]; // Blue
//            contiguousData[(y * width + x) * 4 + 3] = data[y][x][3]; // Alpha
//        }
//    }
    // Update the texture
    glBindTexture(GL_TEXTURE_2D, id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, data);

//    // Free the contiguous block of memory
//    delete[] contiguousData;
}

