#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <GL/glew.h>
#include <string>
#include <vector>

namespace dng::shaders {

std::string loadShader(const std::string& strShaderFilename);
GLuint createShader(GLenum eShaderType, const std::string& strShaderFile);
GLuint createProgram(const std::vector<GLuint>& shaderList);

struct Params {
    GLint modelParameter;       // modeling matrix
    GLint modelViewNParameter;  // modeliview for normals
    GLint viewParameter;        // viewing matrix
    GLint projParameter;        // projection matrix
    // material
    GLint kaParameter;  // ambient material
    GLint kdParameter;  // diffuse material
    GLint ksParameter;  // specular material
    GLint shParameter;  // shinenness material
};

}  // namespace dng::shaders

#endif  // __SHADERS_H__