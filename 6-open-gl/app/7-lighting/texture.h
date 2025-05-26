#ifndef CPPPROJECTTEMPLATE_TEXTURE_H
#define CPPPROJECTTEMPLATE_TEXTURE_H

enum TextureType {
    JPG,
    PNG
};

struct OpenGlTexture {
    unsigned int id;

    OpenGlTexture(const char* _sourcePath, TextureType textureType) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(_sourcePath, &width, &height, &nrChannels, 0);
        if (data) {
            GLint textureFormat = textureType == TextureType::JPG ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture\n";
        }
        stbi_image_free(data);
    }

    void use(GLint position) {
        glActiveTexture(position);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void clean() {
        glDeleteTextures(1, &id);
    }
};

#endif //CPPPROJECTTEMPLATE_TEXTURE_H
