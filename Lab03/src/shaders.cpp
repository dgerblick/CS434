#include <shaders.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace dng::shaders {

void shaderLog(GLint shader, GLenum eShaderType) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar* strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

    const char* strShaderType = NULL;
    switch (eShaderType) {
    case GL_VERTEX_SHADER:
        strShaderType = "vertex";
        break;
    case GL_GEOMETRY_SHADER_EXT:
        strShaderType = "geometry";
        break;
    case GL_FRAGMENT_SHADER:
        strShaderType = "fragment";
        break;
    case GL_LINK_STATUS:
        strShaderType = "link";
        break;
    }
    fprintf(stderr, "Compile or link failure in %s shader:\n%s\n", strShaderType, strInfoLog);
    fprintf(stderr, "Press ENTER to exit\n");
    getchar();
    delete[] strInfoLog;
    exit(-1);
}

std::string findFile(const std::string& strFilename) {
    std::ifstream testFile(strFilename.c_str());
    if (testFile.is_open())
        return strFilename;
    else {
        std::cout << "Could not find the file " << strFilename << '\n';
        return std::string();
    }
}

std::string loadShader(const std::string& strShaderFilename) {
    std::string strFilename = findFile(strShaderFilename);
    std::ifstream shaderFile(strFilename.c_str());
    std::stringstream shaderData;
    shaderData << shaderFile.rdbuf();
    shaderFile.close();

    return shaderData.str();
}

GLuint createShader(GLenum eShaderType, const std::string& strShader) {
    GLuint shader = glCreateShader(eShaderType);
    const char* strData = strShader.c_str();
    glShaderSource(shader, 1, &strData, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        shaderLog(shader, eShaderType);
    } else {
        std::cout << "--------------------Shader---------------------\n"
                  << strData << '\n'
                  << "------------------compiled OK------------------\n";
    }
    return shader;
}

GLuint createProgram(const std::vector<GLuint>& shaderList) {
    GLuint program = glCreateProgram();

    for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glAttachShader(program, shaderList[iLoop]);

    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        shaderLog(GL_LINK_STATUS, program);
    //{
    //	GLint infoLogLength;
    //	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    //	GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    //	glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
    //	fprintf(stderr, "Linker failure: %s\n", strInfoLog);
    //	delete[] strInfoLog;
    //}

    for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glDetachShader(program, shaderList[iLoop]);

    return program;
}

}  // namespace dng::shaders
