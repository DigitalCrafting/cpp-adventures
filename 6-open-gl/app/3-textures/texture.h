#ifndef CPPPROJECTTEMPLATE_TEXTURE_H
#define CPPPROJECTTEMPLATE_TEXTURE_H

#include "stb_image.h"

struct OpenGlTexture {
    unsigned int id;

    OpenGlTexture(const char* _sourcePath) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(_sourcePath, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture\n";
        }
        stbi_image_free(data);
    }

    void use() {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void clean() {
        glDeleteTextures(1, &id);
    }
};

#endif //CPPPROJECTTEMPLATE_TEXTURE_H
