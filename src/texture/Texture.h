//
// Created by faliszewskii on 06.05.24.
//

#ifndef PUMAROBOT_TEXTURE_H
#define PUMAROBOT_TEXTURE_H


#include <string>

class Texture {
    unsigned int id{};

public:
    explicit Texture(const std::string& path);
    explicit Texture(float **data, unsigned int width, unsigned int height);
    explicit Texture(float ***data, unsigned int width, unsigned int height);

    void bind(int slot) const;

    void updateTexture(float*** data, unsigned int width, unsigned int height);
};


#endif //PUMAROBOT_TEXTURE_H
