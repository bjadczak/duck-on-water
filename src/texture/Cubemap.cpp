//
// Created by Bartek Jadczak on 29/05/2024.
//

#include "Cubemap.h"
#include "../Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <random>

Cubemap::Cubemap (
        std::vector<std::string> faces
)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);


    for (unsigned int i = 0; i < faces.size(); i++)
    {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
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
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
//        const static unsigned int SKYBOX_SIZE = 500;
//        std::mt19937 prng(std::random_device{}());
//        std::uniform_real_distribution<float> dist(0, 1);
//        std::uniform_real_distribution<float> dist1(-0.15, 0);
//        std::uniform_real_distribution<float> dist2(-0.05, 0);
//
//        float arr[4*SKYBOX_SIZE*SKYBOX_SIZE];
//
//        for (int i=0;i<SKYBOX_SIZE*SKYBOX_SIZE;i++) {
//            arr[i*4+3] = 1;
//            auto rnd = dist(prng);
//            if(rnd > 0.999) {
//                arr[i*4+0] = 1 + dist2(prng);
//                arr[i*4+2] = 1 + dist2(prng);
//                arr[i*4+1] = std::min(std::min(1 + dist1(prng), arr[i*4+0]), arr[i*4+2]);
//            }
//            else if(rnd > 0.995) {
//                arr[i*4+0] = 0.9f + dist1(prng);
//                arr[i*4+2] = 0.9f + dist1(prng);
//                arr[i*4+1] = std::min(std::min(0.9f + dist1(prng), arr[i*4+0]), arr[i*4+2]);
//            } else if(rnd > 0.98) {
//                arr[i*4+0] = 0.6f + dist1(prng);
//                arr[i*4+2] = 0.6f + dist1(prng);
//                arr[i*4+1] = std::min(std::min(0.6f + dist1(prng), arr[i*4+0]), arr[i*4+2]);
//            } else {
//                arr[i*4+0] = 0;
//                arr[i*4+1] = 0;
//                arr[i*4+2] = 0;
//            }
//        }
//        glTexImage2D(
//                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
//                0,
//                GL_SRGB_ALPHA,
//                SKYBOX_SIZE,
//                SKYBOX_SIZE,
//                0,
//                GL_RGBA,
//                GL_FLOAT,
//                arr
//                    );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::bind (int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}
