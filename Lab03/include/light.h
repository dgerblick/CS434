#ifndef __LIGHTS_H__
#define __LIGHTS_H__

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace dng {

class Light {
public:
    Light();
    void setPos(glm::vec4 par) { lPos = par; };
    void setLa(glm::vec3 par) { la = par; };
    void setLs(glm::vec3 par) { ls = par; };
    void setLd(glm::vec3 par) { ld = par; };
    void setLaToShader(GLuint uniform) { laParameter = uniform; };
    void setLdToShader(GLuint uniform) { ldParameter = uniform; };
    void setLsToShader(GLuint uniform) { lsParameter = uniform; };
    void setLposToShader(GLuint uniform) { lPosParameter = uniform; };
    void setShaders();

private:
    glm::vec4 lPos;      // position
    glm::vec3 la;        // ambient
    glm::vec3 ld;        // diffuse
    glm::vec3 ls;        // specular
    GLuint laParameter;  // shader uniform variables
    GLuint ldParameter;
    GLuint lsParameter;
    GLuint lPosParameter;
};

}  // namespace dng

#endif  // __LIGHTS_H__