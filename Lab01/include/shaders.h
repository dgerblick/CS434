#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <GL/glew.h>
#include <string>
#include <vector>

namespace dng {

std::string LoadShader(const std::string& strShaderFilename);
GLuint CreateShader(GLenum eShaderType, const std::string& strShaderFile);
GLuint CreateProgram(const std::vector<GLuint>& shaderList);

}  // namespace dng

#endif  // __SHADERS_H__