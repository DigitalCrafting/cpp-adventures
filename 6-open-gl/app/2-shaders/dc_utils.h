#ifndef CPPPROJECTTEMPLATE_DC_UTILS_H
#define CPPPROJECTTEMPLATE_DC_UTILS_H

#include "glad/gl.h"
#include "GLFW/glfw3.h"

struct OpenGlShader {
    unsigned int id;

    OpenGlShader(const char *_source, GLenum _type) {
        int success;
        char infoLog[512];
        id = glCreateShader(_type);
        glShaderSource(id, 1, &_source, NULL);
        glCompileShader(id);
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << (_type == GL_VERTEX_SHADER ? "SHADER" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
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

    OpenGlProgram( const char* vertexShader, const char *fragmentShader) {
        id = glCreateProgram();
        OpenGlShader vs{vertexShader, GL_VERTEX_SHADER};
        OpenGlShader fs{fragmentShader, GL_FRAGMENT_SHADER};
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

#endif //CPPPROJECTTEMPLATE_DC_UTILS_H
