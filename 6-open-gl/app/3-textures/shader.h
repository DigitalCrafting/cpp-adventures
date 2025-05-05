#ifndef CPPPROJECTTEMPLATE_SHADER_H
#define CPPPROJECTTEMPLATE_SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>
#include "glad/gl.h"
#include "GLFW/glfw3.h"

std::string readFile(const char *path) {
    std::string content;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        content = stream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::FILE::COULD_NOT_READ_FILE::" << path << "\n";
    }

    return content;
}

struct OpenGlShader {
    unsigned int id;

    OpenGlShader(const char *_sourcePath, GLenum _type) {
        int success;
        char infoLog[512];
        id = glCreateShader(_type);
        std::string source = readFile(_sourcePath);
        const char* contentAsCharP = source.c_str();
        glShaderSource(id, 1, &contentAsCharP, NULL);
        glCompileShader(id);
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << (_type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
                      << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    /*
     * Production ready code should use RAII for cleanup
     *
     * ~OpenGlShader() {
     *     glDeleteShader(id);
     * }
    */

    void clean() const {
        glDeleteShader(id);
    }
};

struct OpenGlProgram {
    unsigned int id;

    OpenGlProgram(const char *vertexShaderPath, const char *fragmentShaderPath) {
        id = glCreateProgram();
        OpenGlShader vs{vertexShaderPath, GL_VERTEX_SHADER};
        OpenGlShader fs{fragmentShaderPath, GL_FRAGMENT_SHADER};
        glAttachShader(id, vs.id);
        glAttachShader(id, fs.id);
        glLinkProgram(id);

        int success;
        char infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        vs.clean();
        fs.clean();
    }

    void use() const {
        glUseProgram(id);
    }

    void clean() const {
        glDeleteProgram(id);
    }
};

#endif //CPPPROJECTTEMPLATE_SHADER_H
